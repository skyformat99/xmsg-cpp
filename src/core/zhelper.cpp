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

#include "zhelper.h"

#include <xmsg/util.h>

#include <atomic>
#include <random>
#include <sstream>

namespace {

std::random_device rd;
std::mt19937_64 rng{rd()};

// language identifier (Java:1, C++:2, Python:3)
const auto cpp_id = 2;

// replyTo generation: format is "ret:<id>:2[dddddd]"
const auto rt_seq_size = 1'000'000;
const auto rt_seq_base = cpp_id * rt_seq_size;
auto rt_gen = std::uniform_int_distribution<std::uint_fast32_t>{0, 999'999};
std::atomic_uint_fast32_t rt_seq{rt_gen(rng)};

// ID generation: format is 9 digits: 2[ppp][ddddd]
auto id_gen = std::uniform_int_distribution<int>{0, 99};
const auto ip_hash = std::hash<std::string>{}(xmsg::util::localhost());
const auto id_prefix = cpp_id * 100'000'000 + (ip_hash % 1000) * 100'000;

}


namespace xmsg {
namespace core {

std::string get_unique_replyto(const std::string& subject)
{
    auto id = ++rt_seq % rt_seq_size + rt_seq_base;
    return "ret:" + subject + ":" + std::to_string(id);
}


void set_unique_replyto(std::uint_fast32_t value)
{
    rt_seq = value;
}


std::string encode_identity(const std::string& address, const std::string& name)
{
    std::uniform_int_distribution<int> hash_gen{0, 99};
    std::string id = address + "#" + name + "#" + std::to_string(hash_gen(rng));
    std::stringstream ss;
    ss << std::hex << std::hash<std::string>{}(id);
    return ss.str().substr(0, 8);
}


std::string get_random_id()
{
    return std::to_string(id_prefix + id_gen(rng));
}

} // end namespace util

} // end namespace xmsg
