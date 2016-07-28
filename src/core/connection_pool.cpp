/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */
/*
 * Copyright (C) 2015. Jefferson Lab, xMsg framework (JLAB). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and not-for-profit purposes,
 * without fee and without a signed licensing agreement.
 *
 * Contact Vardan Gyurjyan
 * Department of Experimental Nuclear Physics, Jefferson Lab.
 *
 * IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF
 * THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE CLARA SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
 * HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO PROVIDE MAINTENANCE,
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <xmsg/connection_pool.h>

#include <xmsg/context.h>

#include "connection_driver.h"
#include "registration_driver.h"

#include <queue>
#include <unordered_map>


namespace xmsg {

namespace detail {

void ProxyDriverDeleter::operator()(ProxyDriver *p)
{
    delete p;
}

void RegDriverDeleter::operator()(RegDriver *p)
{
    delete p;
}

} // end namespace detail


template <typename A, typename U>
class ConnectionPool::ConnectionCache {
    using ConnectionQueue = std::queue<U>;
    using ConnectionMap = std::unordered_map<A, ConnectionQueue>;

public:
    U get(const A& addr)
    {
        U up;
        auto it = cache_.find(addr);
        if (it != cache_.end()) {
            auto& q = it->second;
            if (!q.empty()) {
                up = std::move(q.front());
                q.pop();
            }
        }
        return up;
    }

    void set(const A& addr, U&& con)
    {
        auto it = cache_.find(addr);
        if (it == cache_.end()) {
            auto qp = cache_.emplace(addr, ConnectionQueue{});
            it = qp.first;
        }
        it->second.push(std::move(con));
    }

private:
    ConnectionMap cache_;
};


ConnectionPool::ConnectionPool()
  : ctx_{Context::instance()->ctx_},
    default_setup_{std::make_shared<ConnectionSetup>()},
    proxy_cache_{std::make_unique<ProxyDriverCache>()},
    reg_cache_{std::make_unique<RegDriverCache>()}
{ }


ConnectionPool::ConnectionPool(std::shared_ptr<zmq::context_t> ctx)
  : ctx_{std::move(ctx)},
    default_setup_{std::make_shared<ConnectionSetup>()},
    proxy_cache_{std::make_unique<ProxyDriverCache>()},
    reg_cache_{std::make_unique<RegDriverCache>()}
{ }

ConnectionPool::ConnectionPool(ConnectionPool&&) = default;
ConnectionPool& ConnectionPool::operator=(ConnectionPool&&) = default;

ConnectionPool::~ConnectionPool() = default;


ProxyConnection ConnectionPool::get_connection(const ProxyAddress& addr)
{
    auto con = proxy_cache_->get(addr);
    if (!con) {
        con = create_connection(addr, SetupSharedPtr{default_setup_});
    }
    auto del = [this](detail::ProxyDriverPtr&& c) {
        proxy_cache_->set(c->address(), std::move(c));
    };
    return { ProxyAddress{addr}, std::move(con), std::move(del) };
}


ProxyConnection ConnectionPool::get_connection(const ProxyAddress& addr,
                                             SetupPtr&& setup)
{
    auto con = proxy_cache_->get(addr);
    if (!con) {
        con = create_connection(addr, SetupSharedPtr{std::move(setup)});
    }
    auto del = [this](detail::ProxyDriverPtr&& c) {
        proxy_cache_->set(c->address(), std::move(c));
    };
    return { ProxyAddress{addr}, std::move(con), std::move(del) };
}


void ConnectionPool::set_default_setup(SetupPtr&& setup)
{
    default_setup_ = std::move(setup);
}


RegConnection ConnectionPool::get_connection(const RegAddress& addr)
{
    auto con = reg_cache_->get(addr);
    if (!con) {
        con = create_connection(addr);
    }
    auto del = [this](detail::RegDriverPtr&& c) {
        reg_cache_->set(c->address(), std::move(c));
    };
    return { RegAddress{addr}, std::move(con), std::move(del) };
}


detail::ProxyDriverPtr ConnectionPool::create_connection(const ProxyAddress& addr,
                                                         SetupSharedPtr&& setup)
{
    auto con = detail::ProxyDriverPtr{new detail::ProxyDriver(*ctx_, addr, std::move(setup))};
    con->connect();
    return con;
}


detail::RegDriverPtr ConnectionPool::create_connection(const RegAddress& addr)
{
    return detail::RegDriverPtr{new detail::RegDriver(*ctx_, addr)};
}

} // end namespace xmsg
