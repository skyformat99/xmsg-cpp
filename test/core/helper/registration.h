/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "constants.h"
#include "regdis.h"
#include "topic.h"

#include <string>

namespace xmsg {
namespace registration {
namespace test {

proto::Registration new_registration(const std::string& name,
                                     const std::string& host,
                                     const std::string& topic,
                                     bool is_publisher)
{
    return create(name, name + "test data",
                  host, constants::default_port,
                  Topic::raw(topic), is_publisher);
}

} // end namespace test
} // end namespace registration
} // end namespace xmsg
