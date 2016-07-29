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

#include <xmsg/proxy.h>

#include <xmsg/constants.h>

#include "zhelper.h"

#include <iostream>
#include <mutex>

namespace {

zmq::socket_t create_socket(zmq::context_t& ctx, zmq::socket_type type)
{
    auto out = zmq::socket_t{ctx, type};
    out.setsockopt(ZMQ_RCVHWM, 0);
    out.setsockopt(ZMQ_SNDHWM, 0);
    return out;
}

std::mutex mtx;

}


namespace xmsg {
namespace sys {

Proxy::Proxy(zmq::context_t&& ctx, const ProxyAddress& addr)
  : ctx_{std::move(ctx)}, addr_{addr}, is_alive_{false}
{ }


Proxy::Proxy(const ProxyAddress& addr)
    : Proxy({}, addr)
{ }


Proxy::~Proxy()
{
    if (is_alive_.load()) {
        stop();
    }
}


void Proxy::start()
{
    is_alive_ = true;
    ctrl_ = std::thread{&Proxy::control, this};

    try {
        auto in = create_socket(ctx_, zmq::socket_type::xsub);
        auto out = create_socket(ctx_, zmq::socket_type::xpub);

        core::bind(in, addr_.pub_port());
        core::bind(out, addr_.sub_port());

        zmq::proxy((void*) in, (void*) out, NULL);
    } catch (const zmq::error_t& e) {
        if (e.num() != ETERM) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cerr << "Proxy sockets: " << e.what() << std::endl;
        }
    } catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cerr << e.what() << std::endl;
    }
}


void Proxy::control()
{
    auto control = create_socket(ctx_, zmq::socket_type::sub);
    auto publisher = create_socket(ctx_, zmq::socket_type::pub);
    auto router = create_socket(ctx_, zmq::socket_type::router);

    try {
        core::connect(control, addr_.host(), addr_.sub_port());
        core::connect(publisher, addr_.host(), addr_.pub_port());
        core::bind(router, addr_.pub_port() + 2);
    } catch (const zmq::error_t& e) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cerr << "Control socket: " << e.what() << std::endl;
        return;
    }

    auto ctrl = constants::ctrl_topic;
    control.setsockopt(ZMQ_SUBSCRIBE, ctrl.data(), ctrl.size());
    router.setsockopt(ZMQ_ROUTER_HANDOVER, 1);

    while (is_alive_.load()) {
        try {
            auto in_msg = core::recv_msg<3>(control);

            auto type = core::to_string(in_msg[1]);
            auto id = core::to_string(in_msg[2]);

            if (type == constants::ctrl_connect) {
                router.send(id.data(), id.size(), ZMQ_SNDMORE);
                router.send(type.data(), type.size(), 0);
            } else if (type == constants::ctrl_subscribe) {
                publisher.send(id.data(), id.size(), ZMQ_SNDMORE);
                publisher.send(type.data(), type.size(), 0);
            } else if (type == constants::ctrl_reply) {
                router.send(id.data(), id.size(), ZMQ_SNDMORE);
                router.send(type.data(), type.size(), 0);
            }
        } catch (const zmq::error_t& ex) {
            if (ex.num() != ETERM) {
                std::lock_guard<std::mutex> lock(mtx);
                std::cerr << ex.what() << std::endl;
            }
        } catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(mtx);
            std::cerr << e.what() << std::endl;
        }
    }
}


void Proxy::stop()
{
    is_alive_ = false;
    ctx_.close();
    ctrl_.join();
}

} // end namespace sys
} // end namespace xmsg
