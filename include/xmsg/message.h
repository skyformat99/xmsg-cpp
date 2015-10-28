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

#ifndef XMSG_CORE_MESSAGE_HPP_
#define XMSG_CORE_MESSAGE_HPP_

#include "topic.h"
#include "meta.h"
#include "data.h"

#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>


namespace xmsg {

class Message final
{
public:
    template<typename T, typename V>
    Message(T&& topic, std::unique_ptr<proto::Meta>&& metadata, V&& data)
      : topic_{std::forward<T>(topic)},
        meta_{metadata ? std::move(metadata)
                       : throw std::invalid_argument{"null metadata"}},
        data_{std::forward<V>(data)}
    {
        // nothing
    }

    template<typename T, typename S, typename V>
    Message(T&& topic, S&& mimetype, V&& data)
      : topic_{std::forward<T>(topic)},
        meta_{proto::make_meta()},
        data_{std::forward<V>(data)}
    {
        proto::set_datatype(*meta_, mimetype);
    }

    Message(const Message& other)
      : topic_{other.topic_},
        meta_{proto::copy_meta(*other.meta_)},
        data_{other.data_}
    { }

    Message& operator=(const Message& other)
    {
        topic_ = other.topic_;
        meta_ = proto::copy_meta(*other.meta_);
        data_ = other.data_;
        return *this;
    }

    Message(Message&&) = default;
    Message& operator=(Message&&) = default;

    ~Message() = default;

public:
    friend void swap(Message& lhs, Message& rhs)
    {
        using std::swap;
        swap(lhs.topic_, rhs.topic_);
        swap(lhs.meta_, rhs.meta_);
        swap(lhs.data_, rhs.data_);
    }

public:
    const Topic& topic() const { return topic_; }

    proto::Meta* meta() { return meta_.get(); }
    const proto::Meta* meta() const { return meta_.get(); }

    const std::vector<std::uint8_t>& data() const { return data_; }

public:
    const std::string& datatype() const { return meta_->datatype(); }

    bool has_replyto() const { return meta_->has_replyto(); }
    Topic replyto() const { return Topic::raw(meta_->replyto()); }

private:
    Topic topic_;
    std::unique_ptr<proto::Meta> meta_;
    std::vector<std::uint8_t> data_;
};


template<typename T>
inline Message make_message(T&& topic, const proto::Data& data)
{
    auto buffer = proto::serialize_data(data);
    return {std::forward<T>(topic), mimetype::xmsg_data, std::move(buffer)};
}


inline proto::Data parse_message(const Message& msg)
{
    return proto::parse_data(msg.data());
}


inline bool operator==(const Message& lhs, const Message& rhs)
{
    return std::tie(lhs.topic(), *lhs.meta(), lhs.data())
        == std::tie(rhs.topic(), *rhs.meta(), rhs.data());
}

inline bool operator!=(const Message& lhs, const Message& rhs)
{
    return !(lhs == rhs);
}


} // end namespace xmsg

#endif // XMSG_CORE_MESSAGE_H_
