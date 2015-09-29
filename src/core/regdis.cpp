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

#include "regdis.h"

#include "constants.h"

#include <iostream>
#include <tuple>

namespace {

auto make_tie(const xmsg::proto::Registration& reg, int& port, int& owner)
{
    port = reg.port();
    owner = int{reg.ownertype()};
    return std::tie(reg.name(),
                    reg.domain(),
                    reg.subject(),
                    reg.type(),
                    reg.host(),
                    port,
                    owner,
                    reg.description());
}

}


namespace xmsg {

namespace proto {

bool CompareRegistration::operator()(const Registration& lhs,
                                     const Registration& rhs) const
{
    int l_port, l_owner;
    int r_port, r_owner;
    return make_tie(lhs, l_port, l_owner) < make_tie(rhs, r_port, r_owner);
}


bool operator==(const Registration& lhs, const Registration& rhs)
{
    int l_port, l_owner;
    int r_port, r_owner;
    return make_tie(lhs, l_port, l_owner) == make_tie(rhs, r_port, r_owner);
}


bool operator!=(const Registration& lhs, const Registration& rhs)
{
    return !(lhs == rhs);
}

} // end namespace proto



namespace registration {

Request::Request(std::string topic,
                 std::string sender,
                 const proto::Registration& data)
  : topic_  {std::move(topic)},
    sender_ {std::move(sender)},
    data_   {data.SerializeAsString()}
{
    // nothing
}


Request::Request(std::string topic, std::string sender, std::string text)
  : topic_  {std::move(topic)},
    sender_ {std::move(sender)},
    data_   {std::move(text)}
{
  // nothing
}


Request::Request(const RequestMsg& msg)
  : topic_  {msg[0].data<const char>(), msg[0].size()},
    sender_ {msg[1].data<const char>(), msg[1].size()},
    data_   {msg[2].data<const char>(), msg[2].size()}
{
  // nothing
}


RequestMsg Request::msg()
{
    return {
        zmq::message_t{topic_.begin(), topic_.end()},
        zmq::message_t{sender_.begin(), sender_.end()},
        zmq::message_t{data_.begin(), data_.end()},
    };
}


proto::Registration Request::data() const
{
    proto::Registration rd;
    rd.ParseFromString(data_);
    return rd;
};



Response::Response(std::string topic, std::string sender)
  : topic_{std::move(topic)},
    sender_{std::move(sender)},
    status_{constants::success}
{
  // nothing
}


Response::Response(std::string topic, std::string sender, RegDataSet data)
  : topic_{std::move(topic)},
    sender_{std::move(sender)},
    status_{constants::success},
    data_{std::move(data)}
{
  // nothing
}


Response::Response(std::string topic, std::string sender, std::string error_msg)
  : topic_{std::move(topic)},
    sender_{std::move(sender)},
    status_{std::move(error_msg)}
{
  // nothing
}


Response::Response(const ResponseMsg& msg)
  : topic_  {msg[0].data<const char>(), msg[0].size()},
    sender_ {msg[1].data<const char>(), msg[1].size()},
    status_ {msg[2].data<const char>(), msg[2].size()}
{
    using ZFrame = const zmq::message_t;
    std::for_each(msg.begin() + n_fields, msg.end(), [=](ZFrame& f) {
        auto reg = proto::Registration{};
        reg.ParseFromString({f.data<const char>(), f.size()});
        data_.insert(reg);
    });
}


ResponseMsg Response::msg()
{
    ResponseMsg msg;
    msg.reserve(n_fields + data_.size());
    msg.emplace_back(topic_.begin(), topic_.end());
    msg.emplace_back(sender_.begin(), sender_.end());
    msg.emplace_back(status_.begin(), status_.end());
    for (auto& reg : data_) {
        auto buf = reg.SerializeAsString();
        msg.emplace_back(buf.begin(), buf.end());
    }
    return msg;
}



bool operator==(const Request& lhs, const Request& rhs)
{
    return std::tie(lhs.topic(), lhs.sender(), lhs.text())
        == std::tie(rhs.topic(), rhs.sender(), rhs.text());
}


bool operator!=(const Request& lhs, const Request& rhs)
{
    return !(lhs == rhs);
}


bool operator==(const Response& lhs, const Response& rhs)
{
    return std::tie(lhs.topic(), lhs.sender(), lhs.status(), lhs.data())
        == std::tie(rhs.topic(), rhs.sender(), rhs.status(), rhs.data());
}


bool operator!=(const Response& lhs, const Response& rhs)
{
    return !(lhs == rhs);
}


proto::Registration create(const std::string& name,
                           const std::string& description,
                           const std::string& host,
                           int port,
                           const Topic& topic,
                           bool is_publisher)
{
    auto data_type = is_publisher
            ? proto::Registration::OwnerType::Registration_OwnerType_PUBLISHER
            : proto::Registration::OwnerType::Registration_OwnerType_SUBSCRIBER;
    auto data = proto::Registration{};
    data.set_name(name);
    data.set_host(host);
    data.set_description(description);
    data.set_port(constants::default_port);
    data.set_domain(topic.domain());
    data.set_subject(topic.subject());
    data.set_type(topic.type());
    data.set_ownertype(data_type);
    return data;
}

} // end namespace registration

} // end namespace xmsg
