/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#ifndef XMSG_TEST_PROXY_THREAD_H_
#define XMSG_TEST_PROXY_THREAD_H_

#include "constants.h"
#include "util.h"

#include "zmq.hpp"

#include <iostream>
#include <string>
#include <thread>

namespace xmsg {

namespace test {

class ProxyThread {
public:
    ProxyThread()
    {
        thread_ = std::thread {[&]() {
            try {
                auto port = constants::default_port;
                auto in = create_socket(port, zmq::socket_type::xsub);
                auto out = create_socket(port + 1, zmq::socket_type::xpub);
                zmq::proxy((void*) in, (void*) out, NULL);
            } catch (zmq::error_t& e) {
                if (e.num() != ETERM) {
                    std::cerr << e.what() << std::endl;
                }
            } catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
        }};
        xmsg::util::sleep(100);
    }

    void stop()
    {
        ctx_.close();
        thread_.join();
    }

private:
    zmq::socket_t create_socket(int port, zmq::socket_type type)
    {
        auto out = zmq::socket_t{ctx_, type};
        out.setsockopt(ZMQ_RCVHWM, 0);
        out.setsockopt(ZMQ_SNDHWM, 0);
        out.bind("tcp://*:" + std::to_string(port));
        return out;
    }

private:
    zmq::context_t ctx_{};
    std::thread thread_{};

};


} // end namespace test
} // end namespace xmsg

#endif
