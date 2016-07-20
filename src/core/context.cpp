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

#include <xmsg/context.h>

#include <xmsg/third_party/zmq.hpp>

namespace xmsg {

Context::Context()
  : ctx_{std::make_shared<zmq::context_t>()}
{
    // nop
}

Context::~Context() = default;


Context* Context::instance()
{
    static Context ctx{};
    return &ctx;
}


void Context::set_io_threads(int threads)
{
    int rc = zmq_ctx_set((void*) *ctx_, ZMQ_IO_THREADS, threads);
    if (rc < 0) {
        throw zmq::error_t{};
    }
}


int Context::io_threads()
{
    int rc = zmq_ctx_get((void*) *ctx_, ZMQ_IO_THREADS);
    if (rc < 0) {
        throw zmq::error_t{};
    }
    return rc;
}


void Context::set_max_sockets(int sockets)
{
    int rc = zmq_ctx_set((void*) *ctx_, ZMQ_MAX_SOCKETS, sockets);
    if (rc < 0) {
        throw zmq::error_t{};
    }
}


int Context::max_sockets()
{
    int rc = zmq_ctx_get((void*) *ctx_, ZMQ_MAX_SOCKETS);
    if (rc < 0) {
        throw zmq::error_t{};
    }
    return rc;
}

} // end namespace xmsg
