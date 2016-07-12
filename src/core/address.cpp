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

#include <xmsg/address.h>

#include <xmsg/constants.h>
#include <xmsg/util.h>

#include <cstdint>
#include <sstream>
#include <stdexcept>

namespace {
constexpr std::uint16_t privileged_ports = 1024;
constexpr std::uint16_t max_port_number = UINT16_MAX;

inline bool invalid_port(int port)
{
    return port < privileged_ports || port > max_port_number;
}

inline int default_sub_port(int pub_port)
{
    return pub_port + 1;
}
} // end empty namespace


namespace xmsg {

ProxyAddress::ProxyAddress()
  : ProxyAddress{util::localhost()}
{ }


ProxyAddress::ProxyAddress(const std::string& host)
  : ProxyAddress{host, constants::default_port}
{ }


ProxyAddress::ProxyAddress(const std::string& host, int pub_port)
  : ProxyAddress(host, pub_port, default_sub_port(pub_port))
{ }


ProxyAddress::ProxyAddress(const std::string& host, int pub_port, int sub_port)
    : host{util::to_host_addr(host)},
      pub_port{pub_port},
      sub_port{sub_port}
{
    if (invalid_port(pub_port)) {
        throw std::invalid_argument{"invalid pub port: " + std::to_string(pub_port)};
    };
    if (invalid_port(sub_port)) {
        throw std::invalid_argument{"invalid sub port: " + std::to_string(sub_port)};
    };
}


RegAddress::RegAddress()
 : RegAddress{util::localhost(), constants::registrar_port}
{ }


RegAddress::RegAddress(const std::string& host)
  : RegAddress{host, constants::registrar_port}
{ }


RegAddress::RegAddress(const std::string& host, int port)
  : host{util::to_host_addr(host)},
    port{port}
{
    if (invalid_port(port)) {
        throw std::invalid_argument{"invalid reg port: " + std::to_string(port)};
    };
}

} // end namespace xmsg
