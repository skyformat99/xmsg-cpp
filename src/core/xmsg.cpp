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

#include <xmsg/xmsg.h>

#include <xmsg/context.h>
#include <xmsg/connection_pool.h>
#include <xmsg/util.h>

#include "connection_driver.h"
#include "registration_driver.h"
#ifdef __APPLE__
#include "thread_local.h"
#endif

#include <random>

namespace xmsg {

/// \cond HIDDEN_SYMBOLS
struct xMsg::Impl {

    Impl(const std::string& name,
         const ProxyAddress& proxy_addr,
         const RegAddress& reg_addr)
      : name{name},
        id{core::encode_identity(proxy_addr.host(), name)},
        default_proxy_addr{proxy_addr},
        default_reg_addr{reg_addr}
    { }

    ConnectionPool* con_pool()
    {
#ifdef __APPLE__
        using tls = detail::ThreadLocal<ConnectionPool>;
        return tls::getThreadInstance();
#else
        static thread_local ConnectionPool pool{};
        return &pool;
#endif
    }

    std::string name;
    std::string id;
    ProxyAddress default_proxy_addr;
    RegAddress default_reg_addr;
};


class ScopedSubscription final
{
public:
    ScopedSubscription(detail::ProxyDriver& connection, const Topic& topic)
      : connection_{connection}, topic_{topic}, poller_{connection_.sub_socket()}
    {
        connection_.subscribe(topic_);
        util::sleep(10);
    }

    ~ScopedSubscription()
    {
        connection_.unsubscribe(topic_);
    }

    bool poll(int timeout)
    {
        return poller_.poll(timeout);
    }

private:
    detail::ProxyDriver& connection_;
    Topic topic_;
    core::BasicPoller poller_;
};
/// \endcond


xMsg::xMsg(const std::string& name)
  : xMsg{name, {}, {}}
{ }


xMsg::xMsg(const std::string& name,
           const RegAddress& default_registrar)
  : xMsg{name, {}, default_registrar}
{ }


xMsg::xMsg(const std::string& name,
           const ProxyAddress& default_proxy,
           const RegAddress& default_registrar)
  : xmsg_{new Impl{name, default_proxy, default_registrar}}
{ }


xMsg::xMsg(xMsg &&) = default;
xMsg& xMsg::operator=(xMsg &&) = default;

xMsg::~xMsg() = default;


ProxyConnection xMsg::connect()
{
    return xmsg_->con_pool()->get_connection(xmsg_->default_proxy_addr);
}


ProxyConnection xMsg::connect(const ProxyAddress& addr)
{
    return xmsg_->con_pool()->get_connection(addr);
}


void xMsg::set_connection_setup(std::unique_ptr<ConnectionSetup> setup)
{
    return xmsg_->con_pool()->set_default_setup(std::move(setup));
}


void xMsg::publish(ProxyConnection& connection, Message& msg)
{
    connection->send(msg);
}


Message xMsg::sync_publish(ProxyConnection& connection,
                           Message& msg,
                           int timeout)
{
    auto return_addr = core::get_unique_replyto(xmsg_->id);
    msg.meta_->set_replyto(return_addr);

    auto sub = ScopedSubscription{*connection, Topic::raw(return_addr)};
    connection->send(msg);

    const auto dt = 10;
    auto t = 0;
    while (t < timeout) {
        if (sub.poll(dt)) {
            auto msg = connection->recv();
            return msg;
        }
        t += dt;
    }

    throw std::runtime_error("xMsg-Error: no response for time_out = " +
                             std::to_string(timeout) + " milli sec.");
}


std::unique_ptr<Subscription>
xMsg::subscribe(const Topic& topic,
                ProxyConnection&& connection,
                std::function<void(Message&)> callback)
{
    return std::unique_ptr<Subscription>{
            new Subscription{topic, connection.release(), std::move(callback)}
    };
}


void xMsg::unsubscribe(std::unique_ptr<Subscription> handler)
{
    handler->stop();
}


void xMsg::register_as_publisher(const Topic& topic,
                                 const std::string& description)
{
    register_as_publisher(xmsg_->default_reg_addr, topic, description);
}


void xMsg::register_as_publisher(const RegAddress& addr,
                                 const Topic& topic,
                                 const std::string& description)
{
    auto driver = xmsg_->con_pool()->get_connection(addr);
    auto proxy = xmsg_->default_proxy_addr;
    auto data = registration::create(xmsg_->name, description,
                                     proxy.host(), proxy.pub_port(),
                                     topic, true);
    data.set_description(description);

    driver->add(data, true);
}


void xMsg::register_as_subscriber(const Topic& topic,
                                  const std::string& description)
{
    register_as_subscriber(xmsg_->default_reg_addr, topic, description);
}


void xMsg::register_as_subscriber(const RegAddress& addr,
                                  const Topic& topic,
                                  const std::string& description)
{
    auto driver = xmsg_->con_pool()->get_connection(addr);
    auto proxy = xmsg_->default_proxy_addr;
    auto data = registration::create(xmsg_->name, description,
                                     proxy.host(), proxy.sub_port(),
                                     topic, false);
    data.set_description(description);

    driver->add(data, false);
}


void xMsg::deregister_as_publisher(const Topic& topic)
{
    deregister_as_publisher(xmsg_->default_reg_addr, topic);
}


void xMsg::deregister_as_publisher(const RegAddress& addr, const Topic& topic)
{
    auto driver = xmsg_->con_pool()->get_connection(addr);
    auto proxy = xmsg_->default_proxy_addr;
    auto data = registration::create(xmsg_->name, "",
                                     proxy.host(), proxy.pub_port(),
                                     topic, true);
    driver->remove(data, true);
}



void xMsg::deregister_as_subscriber(const Topic& topic)
{
    deregister_as_subscriber(xmsg_->default_reg_addr, topic);
}


void xMsg::deregister_as_subscriber(const RegAddress& addr, const Topic& topic)
{
    auto driver = xmsg_->con_pool()->get_connection(addr);
    auto proxy = xmsg_->default_proxy_addr;
    auto data = registration::create(xmsg_->name, "",
                                     proxy.host(), proxy.sub_port(),
                                     topic, false);
    driver->remove(data, false);
}


RegDataSet xMsg::find_publishers(const Topic& topic)
{
    return find_publishers(xmsg_->default_reg_addr, topic);
}


RegDataSet xMsg::find_publishers(const RegAddress& addr, const Topic& topic)
{
    auto driver = xmsg_->con_pool()->get_connection(addr);
    auto proxy = xmsg_->default_proxy_addr;
    auto data = registration::create(xmsg_->name, "",
                                     proxy.host(), proxy.pub_port(),
                                     topic, true);
    return driver->find(data, true);
}


RegDataSet xMsg::find_subscribers(const Topic& topic)
{
    return find_subscribers(xmsg_->default_reg_addr, topic);
}


RegDataSet xMsg::find_subscribers(const RegAddress& addr, const Topic& topic)
{
    auto driver = xmsg_->con_pool()->get_connection(addr);
    auto proxy = xmsg_->default_proxy_addr;
    auto data = registration::create(xmsg_->name, "",
                                     proxy.host(), proxy.sub_port(),
                                     topic, false);
    return driver->find(data, false);
}


const std::string& xMsg::name()
{
    return xmsg_->name;
}


const RegAddress& xMsg::default_registrar()
{
    return xmsg_->default_reg_addr;
}


const ProxyAddress& xMsg::default_proxy()
{
    return xmsg_->default_proxy_addr;
}

} // end namespace xmsg
