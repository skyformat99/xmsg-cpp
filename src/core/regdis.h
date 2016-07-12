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

#ifndef XMSG_CORE_REG_HPP_
#define XMSG_CORE_REG_HPP_

#include <xmsg/address.h>
#include <xmsg/proto/registration.h>
#include <xmsg/topic.h>

#include "zmq.hpp"

#include <array>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace xmsg {

namespace registration {

using RequestMsg = std::array<zmq::message_t, 3>;
using ResponseMsg = std::vector<zmq::message_t>;

class Request final
{
public:
    Request(std::string topic,
            std::string sender,
            const proto::Registration& data);

    Request(std::string topic,
            std::string sender,
            std::string text);

    Request(const RequestMsg& msg);

    RequestMsg msg();

    const std::string& topic() const { return topic_; };

    const std::string& sender() const { return sender_; };

    const std::string& text() const { return data_; }

    proto::Registration data() const;

private:
    std::string topic_;
    std::string sender_;
    std::string data_;
};


class Response final
{
public:
    Response(std::string topic, std::string sender);

    Response(std::string topic, std::string sender, RegDataSet data);

    Response(std::string topic, std::string sender, std::string error_msg);

    Response(const ResponseMsg& msg);

    ResponseMsg msg();

    const std::string& topic() const { return topic_; };

    const std::string& sender() const { return sender_; };

    const std::string& status() const { return status_; }

    const RegDataSet& data() const { return data_; }

    RegDataSet& data() { return data_; }

private:
    static constexpr size_t n_fields = 3;

    std::string topic_;
    std::string sender_;
    std::string status_;
    RegDataSet data_;
};


class Driver
{
public:
    Driver(zmq::context_t& ctx, RegAddress addr);
    virtual ~Driver() = default;

    void add(const proto::Registration& data, bool is_publisher);

    void remove(const proto::Registration& data, bool is_publisher);
    void remove_all(const std::string& sender, const std::string& host);

    RegDataSet find(const proto::Registration& data, bool is_publisher);

private:
    virtual Response request(Request&, int);

private:
    zmq::context_t& ctx_;
    RegAddress addr_;
    zmq::socket_t socket_;
};


bool operator==(const Request& lhs, const Request& rhs);
bool operator!=(const Request& lhs, const Request& rhs);

bool operator==(const Response& lhs, const Response& rhs);
bool operator!=(const Response& lhs, const Response& rhs);


proto::Registration create(const std::string& name,
                           const std::string& description,
                           const std::string& host,
                           int port,
                           const Topic& topic,
                           bool is_publisher);

} // end namespace registration

} // end namespace xmsg

#endif // XMSG_CORE_REG_H_
