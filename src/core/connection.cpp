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

#include "connection.h"

#include "address.h"
#include "constants.h"
#include "connection_impl.h"
#include "util.h"

#include "zmq.hpp"
#include "zhelper.h"

#include <array>

namespace xmsg {

Connection::Connection(std::unique_ptr<Connection::Impl>&& impl)
  : con_{std::move(impl)}
{ }

Connection::Connection(Connection&&) = default;
Connection& Connection::operator=(Connection&&) = default;

Connection::~Connection() = default;


void Connection::connect()
{
    con_->setup->pre_connection(con_->pub);
    con_->setup->pre_connection(con_->sub);

    core::connect(con_->pub, con_->addr.host, con_->addr.pub_port);
    core::connect(con_->sub, con_->addr.host, con_->addr.sub_port);

    con_->setup->post_connection();
}


void Connection::send(Message& msg)
{
    const auto& t = msg.topic().str();
    const auto& m = msg.meta()->SerializeAsString();
    const auto& d = msg.data();

    con_->pub.send(t.data(), t.size(), ZMQ_SNDMORE);
    con_->pub.send(m.data(), m.size(), ZMQ_SNDMORE);
    con_->pub.send(d.data(), d.size(), 0);
}


Message Connection::recv()
{
    auto multi_msg = core::recv_msg<3>(con_->sub);

    auto topic = core::to_string(multi_msg[0]);
    auto meta = proto::make_meta();
    meta->ParseFromArray(multi_msg[1].data(), multi_msg[1].size());
    auto data = core::to_bytes(multi_msg[2]);

    return {Topic::raw(topic), std::move(meta), std::move(data)};
}


void Connection::subscribe(const Topic& topic)
{
    auto& str = topic.str();
    con_->sub.setsockopt(ZMQ_SUBSCRIBE, str.data(), str.size());
}


void Connection::unsubscribe(const Topic& topic)
{
    auto& str = topic.str();
    con_->sub.setsockopt(ZMQ_UNSUBSCRIBE, str.data(), str.size());
}


const ProxyAddress& Connection::address()
{
    return con_->addr;
}

} // end namespace xmsg
