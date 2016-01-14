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

#include "subscription.h"

#include "connection.h"
#include "connection_impl.h"
#include "message.h"
#include "util.h"

#include "meta.pb.h"

#include "zhelper.h"

#include <array>
#include <iostream>
#include <vector>

namespace xmsg {

/**
 * \class Subscription
 *
 * A subscription object uses a \ref Connection "connection" to receive
 * \ref Message "messages" of the interested \ref Topic "topic",
 * and calls a user action on every message.
 *
 * When the subscription is constructed, the connection will be subscribed to
 * the topic, and a background thread will be started polling the connection for
 * received messages. For every message, the user-provide callback will be
 * executed.
 *
 * When the subscription is destroyed, the background thread will be stopped
 * and the connection will be unsubscribed from the topic.
 *
 * Creation and destruction of subscriptions are controlled by the xMsg actor.
 */

Subscription::Subscription(const Topic& topic,
                           ConnectionWrapperPtr connection,
                           Callback handler)
    : topic_{topic},
      connection_{std::move(connection)},
      handler_{std::move(handler)},
      thread_{},
      is_alive_{false}
{
    connection_->subscribe(topic_);
    thread_ = std::thread{&Subscription::run, this};
    is_alive_.store(true);
}


Subscription::~Subscription()
{
    if (is_alive_) {
        stop();
    }
}


void Subscription::run()
{
    auto poller = core::BasicPoller{connection_->con_->sub};
    const int timeout = 100;
    while (is_alive_.load()) {
        try {
            if (poller.poll(timeout)) {
                auto msg = connection_->recv();
                handler_(msg);
            }
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}


void Subscription::stop()
{
    is_alive_ = false;
    thread_.join();
    connection_->unsubscribe(topic_);
}

} // end namespace xmsg
