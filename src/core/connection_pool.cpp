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

#include "connection_impl.h"
#include "regdis.h"

namespace xmsg {

ConnectionPool::ConnectionPool()
  : ctx_{Context::instance()->ctx_},
    default_setup_{std::make_shared<ConnectionSetup>()}
{ }


ConnectionPool::ConnectionPool(std::shared_ptr<zmq::context_t> ctx)
  : ctx_{std::move(ctx)},
    default_setup_{std::make_shared<ConnectionSetup>()}
{ }


ConnectionPtr ConnectionPool::get_connection(const ProxyAddress& addr)
{
    return create_connection(addr, SetupSharedPtr{default_setup_});
}


ConnectionPtr ConnectionPool::get_connection(const ProxyAddress& addr,
                                             SetupPtr&& setup)
{
    return create_connection(addr, SetupSharedPtr{std::move(setup)});
}


void ConnectionPool::set_default_setup(SetupPtr&& setup)
{
    default_setup_ = std::move(setup);
}


DriverPtr ConnectionPool::get_connection(const RegAddress& addr)
{
    return create_connection(addr);
}


void ConnectionPool::release_connection(ConnectionPtr&& con)
{
    // TODO: put back in pool instead of destroy
    auto c = std::move(con);
}


void ConnectionPool::release_connection(DriverPtr&& con)
{
    // TODO: put back in pool instead of destroy
    auto c = std::move(con);
}


ConnectionPtr ConnectionPool::create_connection(const ProxyAddress& addr,
                                                SetupSharedPtr&& setup)
{
    auto con_imp = std::make_unique<Connection::Impl>(*ctx_, addr, std::move(setup));
    auto con = ConnectionPtr{new Connection{std::move(con_imp)}};
    con->connect();
    return con;
}


DriverPtr ConnectionPool::create_connection(const RegAddress& addr)
{
    return std::make_unique<registration::Driver>(*ctx_, addr);
}

} // end namespace xmsg
