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
        meta_{std::move(metadata)},
        data_{std::forward<V>(data)}
    {
        // nothing
    }

    template<typename T, typename S, typename V>
    Message(T&& topic, S&& mimetype, V&& data)
      : topic_{std::forward<T>(topic)},
        meta_{std::make_unique<proto::Meta>()},
        data_{std::forward<V>(data)}
    {
        meta_->set_datatype(mimetype);
    }

    const Topic& topic() const { return topic_; }

    proto::Meta* meta() { return meta_.get(); }
    const proto::Meta* meta() const { return meta_.get(); }

    const std::vector<std::uint8_t>& data() const { return data_; };

private:
    Topic topic_;
    std::unique_ptr<proto::Meta> meta_;
    std::vector<std::uint8_t> data_;
};


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
