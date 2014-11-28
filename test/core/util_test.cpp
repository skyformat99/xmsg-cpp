/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "util.h"

#include "gmock/gmock.h"

#include <iostream>

using namespace testing;
using namespace xmsg;


AssertionResult is_ip(const std::string& address) {
    return util::is_ipaddr(address)
        ? AssertionSuccess() << address << " is a valid IP"
        : AssertionFailure() << address << " is not a valid IP";
}


TEST(ip_utils, check_valid_ips) {
    std::string addrs[] = {
        "1.1.1.1",
        "255.255.255.255",
        "192.168.1.1",
        "10.10.1.1",
        "132.254.111.10",
        "26.10.2.10",
        "127.0.0.1",
    };

    for (auto& addr : addrs) {
        ASSERT_TRUE(is_ip(addr));
    }
}


TEST(ip_utils, check_invalid_ips) {
    std::string addrs[] = {
        "10.10.10",
        "10.10",
        "10",
        "a.a.a.a",
        "10.10.10.a",
        "10.10.10.256",
        "222.222.2.999",
        "999.10.10.20",
        "2222.22.22.22",
        "22.2222.22.2",
    };

    for (auto& addr : addrs) {
        ASSERT_FALSE(is_ip(addr));
    }
}


TEST(ip_utils, check_only_ip_v4) {
    std::string addrs[] = {
        "2001:cdba:0000:0000:0000:0000:3257:9652",
        "2001:cdba:0:0:0:0:3257:9652",
        "2001:cdba::3257:9652",
    };

    for (auto& addr : addrs) {
        ASSERT_FALSE(is_ip(addr));
    }
}



int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
