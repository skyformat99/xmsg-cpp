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

#include "util.h"

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <chrono>
#include <netdb.h>
#include <regex>
#include <system_error>
#include <thread>


namespace {

std::vector<std::string> localhost_addrs;

}


namespace xmsg {

namespace util {

std::string localhost()
{
    return to_host_addr("localhost");
}


const std::vector<std::string>& get_localhost_addrs()
{
    if (localhost_addrs.empty()) {
        update_localhost_addrs();
    }
    return localhost_addrs;
}


void update_localhost_addrs()
{
    struct ifaddrs* ifl = nullptr;
    getifaddrs(&ifl);

    std::vector<std::string> tmp_addrs;

    for (struct ifaddrs* ifa = ifl; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) {
            void* s_addr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addr_buf[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, s_addr, addr_buf, INET_ADDRSTRLEN);

            auto addr = std::string{addr_buf};
            if (addr.compare(0, 3, "127") != 0) {
                tmp_addrs.push_back(std::move(addr));
            }
        }
    }
    std::swap(tmp_addrs, localhost_addrs);
    if (ifl != nullptr) {
        freeifaddrs(ifl);
    }
}


std::string to_host_addr(const std::string& hostname)
{
    if (is_ipaddr(hostname)) {
        return { hostname };
    }
    if (hostname == "localhost") {
        return get_localhost_addrs()[0];
    } else {
        struct hostent* h = gethostbyname(hostname.data());
        if (h == nullptr) {
            throw std::system_error{EFAULT, std::system_category()};
        }
        return { inet_ntoa(*((struct in_addr*) h->h_addr)) };
    }
}


bool is_ipaddr(const std::string& hostname) {
    auto r = std::regex{"^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                            "(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"};
    return std::regex_match(hostname, r);
}


void validate_ipaddr(const std::string& address) {
    if (!is_ipaddr(address)) {
        throw std::invalid_argument{"invalid IP address: " + address};
    }
}


void sleep(long millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

} // end namespace util

} // end namespace xmsg
