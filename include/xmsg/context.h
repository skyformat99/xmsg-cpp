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

#ifndef XMSG_CORE_CONTEXT_H_
#define XMSG_CORE_CONTEXT_H_

#include <memory>

namespace zmq {
class context_t;
} // end namespace zmq

namespace xmsg {

/**
 * Singleton class that provides unique 0MQ context for entire JVM process.
 */
class Context
{
public:
    /**
     * Returns the global singleton context.
     */
    static Context* instance();

    /**
     * Sets the size of the 0MQ thread pool to handle I/O operations.
     */
    void set_io_threads(int threads);

    /**
     * Gets the size of the 0MQ thread pool to handle I/O operations.
     */
    int io_threads();

    /**
     * Sets the maximum number of sockets allowed on the context.
     */
    void set_max_sockets(int sockets);

    /**
     * Gets the maximum number of sockets allowed on the context.
     */
    int max_sockets();

private:
    Context();

    Context(const Context&) = delete;
    Context(Context&&) = delete;

    Context& operator=(const Context&) = delete;
    Context& operator=(Context&&) = delete;

    ~Context();

private:
    std::shared_ptr<zmq::context_t> ctx_;

    friend class ConnectionPool;
};

} // end namespace xmsg

#endif // XMSG_CORE_CONTEXT_H_
