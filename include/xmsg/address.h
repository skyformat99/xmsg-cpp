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

#ifndef XMSG_CORE_ADDRESS_H_
#define XMSG_CORE_ADDRESS_H_

#include <string>
#include <tuple>

namespace xmsg {

/**
 * The network address of an %xMsg proxy.
 * Proxies connect and dispatch messages between %xMsg actors running on
 * a cloud of nodes. By default, proxies use localhost IP as its address,
 * the \ref constants::default_port as the PUB port, and PUB+1 as the SUB
 * port.
 */
class ProxyAddress final
{
public:
    /// Creates an address using localhost and default ports
    ProxyAddress();

    /// Creates an address using provided host and default ports
    ProxyAddress(const std::string& host);

    /// Creates an address using provided host and PUB port,
    /// with default SUB port
    ProxyAddress(const std::string& host, int pub_port);

    /// Creates an address using provided host and ports
    ProxyAddress(const std::string& host, int pub_port, int sub_port);

    const std::string host;     ///< The host IP of the proxy
    const int pub_port;         ///< The publication port of the proxy
    const int sub_port;         ///< The subscription port of the proxy
};


/**
 * The network address of an %xMsg registrar service.
 * Registration services allow discoverability of running %xMsg actors.
 * By default, registrar services use localhost IP as its address,
 * and \ref constants::registrar_port as the listening port.
 */
class RegAddress final
{
public:
    /// Creates an address using localhost and default port
    RegAddress();

    /// Creates an address using provided host and default port
    RegAddress(const std::string& host);

    /// Creates an address using provided host and port
    RegAddress(const std::string& host, int port);

    const std::string host;     ///< The host IP of the registrar service
    const int port;             ///< The listening port of the registrar service
};


inline bool operator==(const ProxyAddress& lhs, const ProxyAddress& rhs)
{
    return std::tie(lhs.host, lhs.pub_port, lhs.sub_port)
        == std::tie(rhs.host, rhs.pub_port, rhs.sub_port);
}

inline bool operator!=(const ProxyAddress& lhs, const ProxyAddress& rhs)
{
    return !(lhs == rhs);
}


inline bool operator==(const RegAddress& lhs, const RegAddress& rhs)
{
    return std::tie(lhs.host, lhs.port) == std::tie(rhs.host, rhs.port);
}

inline bool operator!=(const RegAddress& lhs, const RegAddress& rhs)
{
    return !(lhs == rhs);
}

} // end namespace xmsg

#endif // XMSG_CORE_ADDRESS_H_
