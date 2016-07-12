/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include <xmsg/constants.h>
#include <xmsg/topic.h>

#include "regdis.h"

#include <random>
#include <string>

namespace xmsg {
namespace registration {
namespace test {

using DataArray = std::vector<std::string>;

const DataArray topics = {
    "writer",
    "writer:adventures",
    "writer:adventures:books",
    "writer:adventures:tales",
    "writer:scifi:books",
    "writer:scifi:tales",
    "actor",
    "actor:action",
    "actor:drama",
    "actor:comedy",
    "actor:action:movies",
    "actor:action:series",
    "actor:comedy:movies",
    "actor:comedy:series",
    "actor:drama:movies",
    "actor:drama:series",
};

const DataArray names = {
    "A", "B", "C", "D", "E",
    "F", "G", "H", "I", "J",
    "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T",
    "U", "V", "W", "X", "Y",
    "Z" };

const DataArray hosts = {
    "10.2.9.50",  "10.2.9.60",
    "10.2.9.51",  "10.2.9.61",
    "10.2.9.52",  "10.2.9.62",
    "10.2.9.53",  "10.2.9.63",
    "10.2.9.54",  "10.2.9.64",
    "10.2.9.55",  "10.2.9.65",
    "10.2.9.56",  "10.2.9.66",
    "10.2.9.57",  "10.2.9.67",
    "10.2.9.58",  "10.2.9.68",
    "10.2.9.59",  "10.2.9.69",
};

std::mt19937 rng{std::random_device{}()};
std::bernoulli_distribution bdist{};
std::uniform_int_distribution<int> udist{0, 16 * 26 * 10}; // size of data


proto::Registration new_registration(const std::string& name,
                                     const std::string& host,
                                     const std::string& topic,
                                     bool is_publisher)
{
    return create(name, name + "test data",
                  host, constants::default_port,
                  Topic::raw(topic), is_publisher);
}


const std::string& random(const DataArray& data)
{
    int idx = udist(rng) % data.size();
    return data[idx];
}


proto::Registration random_registration()
{
    auto name = random(names);
    auto host = random(hosts);
    auto topic = random(topics);
    auto is_publisher = bdist(rng);
    return new_registration(name, host, topic, is_publisher);
}

} // end namespace test
} // end namespace registration
} // end namespace xmsg
