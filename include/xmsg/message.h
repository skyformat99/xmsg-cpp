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

#ifndef XMSG_CORE_MESSAGE_HPP_
#define XMSG_CORE_MESSAGE_HPP_

#include "topic.h"
#include "meta.pb.h"
#include "data.pb.h"

#include <cstdint>
#include <memory>
#include <vector>


namespace xmsg {

class Message final
{
public:
    template<typename T, typename V>
    Message(T&& topic, std::unique_ptr<proto::MetaData>&& metadata, V&& data)
      : topic_{std::forward<T>(topic)},
        meta_{std::move(metadata)},
        data_{std::forward<V>(data)}
    {
        // nothing
    }

    template<typename T, typename S, typename V>
    Message(T&& topic, S&& mimetype, V&& data)
      : topic_{std::forward<T>(topic)},
        meta_{std::make_unique<proto::MetaData>()},
        data_{std::forward<V>(data)}
    {
        meta_->set_datatype(mimetype);
    }

    const Topic& topic() const { return topic_; }

    proto::MetaData* metadata() { return meta_.get(); }
    const proto::MetaData* metadata() const { return meta_.get(); }

    const std::vector<std::uint8_t>& data() const { return data_; };

private:
    Topic topic_;
    std::unique_ptr<proto::MetaData> meta_;
    std::vector<std::uint8_t> data_;
};

} // end namespace xmsg

#endif // XMSG_CORE_MESSAGE_H_
