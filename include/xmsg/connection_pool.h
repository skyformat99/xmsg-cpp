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

#ifndef XMSG_CORE_CONNECTION_POOL_H_
#define XMSG_CORE_CONNECTION_POOL_H_

#include <xmsg/connection.h>

#include <memory>

namespace zmq {
class context_t;
} // end namespace zmq

namespace xmsg {

class ConnectionSetup;

using SetupPtr = std::unique_ptr<ConnectionSetup>;
using SetupSharedPtr = std::shared_ptr<ConnectionSetup>;

class ConnectionPool {
public:
    ConnectionPool();
    ConnectionPool(std::shared_ptr<zmq::context_t> ctx);

    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;

    ConnectionPool(ConnectionPool&&);
    ConnectionPool& operator=(ConnectionPool&&);

    virtual ~ConnectionPool();

public:
    ProxyConnection get_connection(const ProxyAddress& addr);
    ProxyConnection get_connection(const ProxyAddress& addr, SetupPtr&& setup);
    void set_default_setup(SetupPtr&& setup);

    RegConnection get_connection(const RegAddress& addr);

private:
    virtual detail::ProxyDriverPtr create_connection(const ProxyAddress& addr,
                                                     SetupSharedPtr&& setup);
    virtual detail::RegDriverPtr create_connection(const RegAddress& addr);

private:
    std::shared_ptr<zmq::context_t> ctx_;
    SetupSharedPtr default_setup_;

    template<typename A, typename U>
    class ConnectionCache;

    using ProxyDriverCache = ConnectionCache<ProxyAddress, detail::ProxyDriverPtr>;
    using RegDriverCache = ConnectionCache<RegAddress, detail::RegDriverPtr>;

    std::unique_ptr<ProxyDriverCache> proxy_cache_;
    std::unique_ptr<RegDriverCache> reg_cache_;
};

} // end namespace xmsg

#endif // XMSG_CORE_CONNECTION_POOL_H_
