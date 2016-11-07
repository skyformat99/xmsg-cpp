/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#ifndef XMSG_TEST_PROXY_THREAD_H_
#define XMSG_TEST_PROXY_THREAD_H_

#include <xmsg/constants.h>
#include <xmsg/proxy.h>
#include <xmsg/util.h>

#include <iostream>
#include <string>
#include <thread>

namespace xmsg {

namespace test {

class ProxyThread
{
public:
    ProxyThread()
    {
        try {
            proxy_.start();
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        xmsg::util::sleep(100);
    }

    ~ProxyThread()
    {
        proxy_.stop();
    }

private:
    xmsg::sys::Proxy proxy_{{}};
};

} // end namespace test
} // end namespace xmsg

#endif
