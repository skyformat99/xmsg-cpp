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

#include "connection.h"

#include "address.h"
#include "constants.h"
#include "connection_impl.h"
#include "util.h"

#include "zmq.hpp"

namespace xmsg {

Connection::Connection(std::unique_ptr<Connection::Impl>&& impl)
  : con_{std::move(impl)}
{ }

Connection::Connection(Connection&&) = default;
Connection& Connection::operator=(Connection&&) = default;

Connection::~Connection() = default;


void Connection::connect()
{
    auto pub_port = std::to_string(con_->addr.pub_port);
    auto sub_port = std::to_string(con_->addr.sub_port);

    con_->pub.connect("tcp://" + con_->addr.host + ":" + pub_port);
    con_->sub.connect("tcp://" + con_->addr.host + ":" + sub_port);
}


const ProxyAddress& Connection::address()
{
    return con_->addr;
}

} // end namespace xmsg
