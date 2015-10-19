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

#ifndef XMSG_CORE_SUBSCRIPTION_H_
#define XMSG_CORE_SUBSCRIPTION_H_

#include "topic.h"

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>

namespace xmsg {

class Connection;
class Message;
class xMsg;

class Subscription final
{
public:
    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;

    Subscription(Subscription&&) = delete;
    Subscription& operator=(Subscription&&) = delete;

    ~Subscription();

private:
    using Callback = std::function<void(Message&)>;
    using ConnectionDeleter = std::function<void(Connection*)>;
    using ConnectionWrapperPtr = std::unique_ptr<Connection, ConnectionDeleter>;

    Subscription(const Topic& topic,
                 ConnectionWrapperPtr connection,
                 Callback handler);

    void run();
    void stop();

private:
    friend xMsg;

    Topic topic_;
    ConnectionWrapperPtr connection_;
    std::function<void(Message&)> handler_;

    std::thread thread_;
    std::atomic_bool is_alive_;
};

}; // end namespace xmsg

#endif // XMSG_CORE_SUBSCRIPTION_H_
