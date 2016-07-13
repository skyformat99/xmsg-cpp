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

#ifndef XMSG_CORE_ADVANCED_H_
#define XMSG_CORE_ADVANCED_H_

#include <xmsg/third_party/zmq.hpp>

namespace xmsg {

/**
 * Advanced setup of a connection to an %xMsg proxy.
 *
 * This class can be used to customize the internal sockets of a new Connection
 * created by the xMsg actor.
 */
class ConnectionSetup
{
public:
    virtual ~ConnectionSetup() { }

    /**
     * Configures the socket before it is connected.
     * This method will be called for both pub/sub sockets.
     * It should be used to set options on the socket.
     *
     * Leave empty if no configuration is required.
     *
     * \see <a href="http://api.zeromq.org/3-2:zmq-setsockopt">zmq_setsockopt</a>
     */
    virtual void pre_connection(zmq::socket_t& socket);

    /**
     * Runs after the two sockets have been connected.
     * This method can be used to run some action after connecting the sockets.
     * For example, sleep a while to give time to the sockets to be actually
     * connected internally.
     *
     * Leave empty if no action is required.
     */
    virtual void post_connection();
};

} // end namespace xmsg

#endif // XMSG_CORE_ADVANCED_H_
