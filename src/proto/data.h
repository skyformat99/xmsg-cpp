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

#ifndef XMSG_PROTO_DATA_H_
#define XMSG_PROTO_DATA_H_

#include "data.pb.h"
#include "constants.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace xmsg {
namespace proto {

namespace internal {

template <typename T>
inline void set_value(Data& data, const T& value)
        { static_assert(sizeof(T) == 0, "Unsupported data type"); }

inline void set_value(Data& data, std::int32_t value)
        { data.set_flsint32(value); }
inline void set_value(Data& data, std::int64_t &value)
        { data.set_flsint64(value); }
inline void set_value(Data& data, float value)
        { data.set_float_(value); }
inline void set_value(Data& data, double value)
        { data.set_double_(value); }
inline void set_value(Data& data, const std::string& value)
        { data.set_string(value); }
inline void set_value(Data& data, const char* value)
        { data.set_string(value); }

template <typename R, typename C>
inline void set_repeated(R* repeated, const C& values)
        { R data{values.cbegin(), values.cend()}; repeated->Swap(&data); }

inline void set_value(Data& data, const std::vector<std::int32_t>& value)
        { set_repeated(data.mutable_flsint32a(), value); }
inline void set_value(Data& data, const std::vector<std::int64_t>& value)
        { set_repeated(data.mutable_flsint64a(), value); }
inline void set_value(Data& data, const std::vector<float>& value)
        { set_repeated(data.mutable_floata(), value); }
inline void set_value(Data& data, const std::vector<double>& value)
        { set_repeated(data.mutable_doublea(), value); }
inline void set_value(Data& data, const std::vector<std::string>& value)
        { set_repeated(data.mutable_stringa(), value); }

template <typename T>
inline T get_value(Data& data)
        { static_assert(sizeof(T) == 0, "Unsupported data type"); return T{}; }

template<> inline std::int32_t get_value(Data& data)
        { return data.flsint32(); }
template<> inline std::int64_t get_value(Data& data)
        { return data.flsint64(); }
template<> inline float get_value(Data& data)
        { return data.float_(); }
template<> inline double get_value(Data& data)
        { return data.double_(); }
template<> inline std::string get_value(Data& data)
        { return data.string(); }

template<> inline std::vector<std::int32_t> get_value(Data& data)
        { auto& a = data.flsint32a(); return {a.begin(), a.end()}; }
template<> inline std::vector<std::int64_t> get_value(Data& data)
        { auto& a = data.flsint64a(); return {a.begin(), a.end()}; }
template<> inline std::vector<float> get_value(Data& data)
        { auto& a = data.floata(); return {a.begin(), a.end()}; }
template<> inline std::vector<double> get_value(Data& data)
        { auto& a = data.doublea(); return {a.begin(), a.end()}; }
template<> inline std::vector<std::string> get_value(Data& data)
        { auto& a = data.stringa(); return {a.begin(), a.end()}; }

template <typename T>
inline std::string get_mimetype()
        { static_assert(sizeof(T) == 0, "Unsupported data type"); return ""; }

template<> inline std::string get_mimetype<std::int32_t>()
        { return mimetype::single_sint32; }
template<> inline std::string get_mimetype<std::int64_t>()
        { return mimetype::single_sint64; }
template<> inline std::string get_mimetype<float>()
        { return mimetype::single_float; }
template<> inline std::string get_mimetype<double>()
        { return mimetype::single_double; }
template<> inline std::string get_mimetype<std::string>()
        { return mimetype::single_string; }
template<> inline std::string get_mimetype<const char*>()
        { return mimetype::single_string; }

template<> inline std::string get_mimetype<std::vector<std::int32_t>>()
        { return mimetype::array_sint32; }
template<> inline std::string get_mimetype<std::vector<std::int64_t>>()
        { return mimetype::array_sint64; }
template<> inline std::string get_mimetype<std::vector<float>>()
        { return mimetype::array_float; }
template<> inline std::string get_mimetype<std::vector<double>>()
        { return mimetype::array_double; }
template<> inline std::string get_mimetype<std::vector<std::string>>()
        { return mimetype::array_string; }

} // end namespace internal


template<typename T>
inline Data make_data(T&& data)
{
    auto xdata = Data{};
    internal::set_value(xdata, std::forward<T>(data));
    return xdata;
}

template<typename T>
inline T parse_data(Data& data)
{
    return internal::get_value<T>(data);
}

inline std::vector<std::uint8_t> serialize_data(const Data& data)
{
    auto buffer = std::vector<std::uint8_t>(data.ByteSize());
    data.SerializeToArray(buffer.data(), buffer.size());
    return buffer;
}

inline Data parse_data(const std::vector<std::uint8_t>& buffer)
{
    auto xdata = Data{};
    xdata.ParseFromArray(buffer.data(), buffer.size());
    return xdata;
}


inline bool operator==(const Data& lhs, const Data& rhs)
{
    return lhs.SerializeAsString() == rhs.SerializeAsString();
}

inline bool operator!=(const Data& lhs, const Data& rhs)
{
    return !(lhs == rhs);
}

} // end namespace proto
} // end namespace xmsg

#endif // XMSG_PROTO_DATA_H_
