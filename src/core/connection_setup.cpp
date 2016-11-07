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

#include <xmsg/connection_setup.h>

#include <zmq.h>

#include <stdexcept>

namespace xmsg {

void SocketSetup::set_option(int opt, const void* val, size_t val_len)
{
    int rc = zmq_setsockopt(socket_, opt, val, val_len);
    if (rc != 0) {
        throw new std::runtime_error{zmq_strerror(errno)};
    }
}


void SocketSetup::get_option(int opt, void* val, size_t* val_len) const
{
    int rc = zmq_getsockopt(socket_, opt, val, val_len);
    if (rc != 0) {
        throw new std::runtime_error{zmq_strerror(errno)};
    }
}


int SocketSetup::type() const
{
    return get_option<int>(ZMQ_TYPE);
}


void ConnectionSetup::pre_connection(SocketSetup& socket)
{
    socket.set_option(ZMQ_RCVHWM, 0);
    socket.set_option(ZMQ_SNDHWM, 0);
}


void ConnectionSetup::post_connection()
{
    // nothing
}

} // end namespace xmsg
