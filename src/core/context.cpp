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

#include "zhelper.h"


#include <iostream>

namespace xmsg {

Context::Context()
  : impl_{std::make_unique<detail::Context>()}
{
    // nop
}


Context::~Context() = default;


std::shared_ptr<Context> Context::instance()
{
    static auto ctx = std::shared_ptr<Context>(new Context{});
    return ctx;
}

std::unique_ptr<Context> Context::create()
{
    return std::unique_ptr<Context>{new Context()};
}


void Context::set_io_threads(int threads)
{
    impl_->set_option(ZMQ_IO_THREADS, threads);
}


int Context::io_threads()
{
    return impl_->get_option(ZMQ_IO_THREADS);
}


void Context::set_max_sockets(int sockets)
{
    impl_->set_option(ZMQ_MAX_SOCKETS, sockets);
}


int Context::max_sockets()
{
    return impl_->get_option(ZMQ_MAX_SOCKETS);
}

} // end namespace xmsg
