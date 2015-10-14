/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */
/*
 * Copyright (C) 2015. Jefferson Lab, xMsg framework (JLAB). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and not-for-profit purposes,
 * without fee and without a signed licensing agreement.
 *
 * Author Vardan Gyurjyan
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

#ifndef XMSG_CORE_XMSG_H_
#define XMSG_CORE_XMSG_H_

#include "address.h"
#include "connection.h"
#include "message.h"
#include "registration.h"
#include "subscription.h"
#include "topic.h"

#include <functional>
#include <memory>
#include <string>
#include <set>

namespace xmsg {

class ConnectionPool;
class ConnectionSetup;


/**
 * xMsg actor.
 */
class xMsg
{
public:
    explicit xMsg(const std::string& name);

    explicit xMsg(const std::string& name,
                  const RegAddress& default_registrar);

    explicit xMsg(const std::string& name,
                  const ProxyAddress& default_proxy,
                  const RegAddress& default_registrar);

    explicit xMsg(const std::string& name,
                  const ProxyAddress& default_proxy,
                  const RegAddress& default_registrar,
                  std::shared_ptr<ConnectionPool> connection_pool);

    xMsg(const xMsg&) = delete;
    xMsg& operator=(const xMsg&) = delete;

    xMsg(xMsg&&);
    xMsg& operator=(xMsg&&);

    virtual ~xMsg();

public:
    std::unique_ptr<Connection> connect();
    std::unique_ptr<Connection> connect(const ProxyAddress& addr);
    void set_connection_setup(std::unique_ptr<ConnectionSetup> setup);

    void release(std::unique_ptr<Connection>&& connection);

public:
    void publish(std::unique_ptr<Connection>& connection, Message& msg);
    Message sync_publish(std::unique_ptr<Connection>&, Message& msg, int timeout);

    std::unique_ptr<Subscription>
    subscribe(const Topic& topic,
              std::unique_ptr<Connection> connection,
              std::function<void(Message&)> callback);

    void unsubscribe(std::unique_ptr<Subscription> handler);

public:
    void register_as_publisher(const Topic& topic,
                               const std::string& description);
    void register_as_publisher(const RegAddress& addr,
                               const Topic& topic,
                               const std::string& description);

    void register_as_subscriber(const Topic& topic,
                                const std::string& description);
    void register_as_subscriber(const RegAddress& addr,
                                const Topic& topic,
                                const std::string& description);

    void remove_as_publisher(const Topic& topic);
    void remove_as_publisher(const RegAddress& addr, const Topic& topic);

    void remove_as_subscriber(const Topic& topic);
    void remove_as_subscriber(const RegAddress& addr, const Topic& topic);

    RegDataSet find_publishers(const Topic& topic);
    RegDataSet find_publishers(const RegAddress& addr, const Topic& topic);

    RegDataSet find_subscribers(const Topic& topic);
    RegDataSet find_subscribers(const RegAddress& addr, const Topic& topic);

public:
    const std::string& name();

    const RegAddress& default_registrar();

    const ProxyAddress& default_proxy();

private:
    struct Impl;
    std::unique_ptr<Impl> xmsg_;
};

} // end namespace xmsg

#endif // XMSG_CORE_XMSG_H_
