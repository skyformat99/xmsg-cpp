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

#include "constants.h"
#include "util.h"
#include "zmq.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    try {
        auto port = xmsg::constants::default_port;
        auto ctx = zmq::context_t{};

        auto in = zmq::socket_t{ctx, zmq::socket_type::xsub};
        in.setsockopt(ZMQ_RCVHWM, 0);
        in.setsockopt(ZMQ_SNDHWM, 0);
        in.bind("tcp://*:" + std::to_string(port));

        auto out = zmq::socket_t{ctx, zmq::socket_type::xpub};
        out.setsockopt(ZMQ_RCVHWM, 0);
        out.setsockopt(ZMQ_SNDHWM, 0);
        out.bind("tcp://*:" + std::to_string(port + 1));

        zmq::proxy((void*) in, (void*) out, NULL);

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
