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

#include "subscription.h"

#include "connection.h"
#include "connection_impl.h"
#include "message.h"
#include "util.h"

#include "meta.pb.h"

#include <array>
#include <iostream>
#include <vector>

namespace xmsg {

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
    util::sleep(100);
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
    using Items = std::array<zmq::pollitem_t, 1>;

    auto & sub = connection_->con_->sub;
    auto items = Items{ {(void*) sub, 0, ZMQ_POLLIN, 0} };

    const int timeout = 100;

    while (is_alive_.load()) {
        try {
            zmq::poll(items.data(), 1, timeout);
            if (items[0].revents & ZMQ_POLLIN) {
                auto msg = connection_->recv();
                handler_(msg);
            }
        } catch (zmq::error_t& e) {
            // TODO handle reconnect
            std::cerr << e.what() << std::endl;
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}


void Subscription::stop()
{
    is_alive_ = false;
    connection_->unsubscribe(topic_);
    thread_.join();
}

} // end namespace xmsg
