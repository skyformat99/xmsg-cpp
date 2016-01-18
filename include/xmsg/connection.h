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

#ifndef XMSG_CORE_CONNECTION_H_
#define XMSG_CORE_CONNECTION_H_

#include "message.h"

#include <memory>

namespace xmsg {

class ProxyAddress;

/**
 * The standard pub/sub connection to an %xMsg proxy.
 * Contains ProxyAddress object and two 0MQ sockets for publishing and
 * subscribing %xMsg messages respectfully.
 */
class Connection final {
public:
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&);
    Connection& operator=(Connection&&);
    ~Connection();

public:
    /// Returns the address of the connected proxy
    const ProxyAddress& address();

private:
    /// Connects the internal sockets to the proxy
    void connect();

    /// Sends the message through the proxy
    void send(Message& msg);
    /// Receives a message through the proxy
    Message recv();

    /// Subscribes to messages of the given topic through the proxy
    void subscribe(const Topic& topic);
    /// Unsubscribes to messages of the given topic through the proxy
    void unsubscribe(const Topic& topic);

private:
    struct Impl;
    std::unique_ptr<Impl> con_;
    Connection(std::unique_ptr<Impl>&&);

    friend class ConnectionPool;
    friend class xMsg;
    friend class Subscription;
    friend class ScopedSubscription;
};

} // end namespace xmsg

#endif // XMSG_CORE_CONNECTION_H_
