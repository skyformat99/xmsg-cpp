/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "xmsg.h"
#include "util.h"

#include "helper/proxy_wrapper.h"

#include "gmock/gmock.h"
#include "zmq.hpp"

#include <atomic>

using namespace testing;


xmsg::Message create_message(const xmsg::Topic& topic, int data)
{
    auto xmeta = std::make_unique<xmsg::proto::MetaData>();
    auto xdata = xmsg::proto::Data{};
    xmeta->set_datatype("native");
    xdata.set_flsint32(data);
    auto vdata = std::vector<std::uint8_t>(xdata.ByteSize());
    xdata.SerializeToArray(vdata.data(), vdata.size());
    return {topic, std::move(xmeta), std::move(vdata)};
}


int parseData(const xmsg::Message& msg)
{
    auto& v = msg.data();
    xmsg::proto::Data data;
    data.ParseFromArray(v.data(), v.size());
    return data.flsint32();
}



TEST(Subscription, UnsubscribeStopsThread)
{
    auto actor = xmsg::xMsg{"test"};
    auto con = actor.connect();

    auto cb = [] (xmsg::Message& msg) { return std::move(msg); };
    auto sub = actor.subscribe(xmsg::Topic::build("test"), std::move(con), cb);
    xmsg::util::sleep(1000);

    actor.unsubscribe(std::move(sub));
}


TEST(Subscription, SuscribeReceivesAllMessages)
{
    struct Check {
        std::atomic_int counter{0};
        std::atomic_long sum{0};

        const int N = 10000;
        const long SUM_N = 49995000L;
    } check;

    auto proxy_thread = xmsg::test::ProxyThread{};

    auto sub_thread = std::thread{[&](){
        try {
            auto actor = xmsg::xMsg{"test_subscriber"};
            auto connection = actor.connect();
            xmsg::util::sleep(100);

            auto topic = xmsg::Topic::raw("test_topic");
            auto cb = [&] (xmsg::Message& msg) {
                int i = parseData(msg);
                check.counter.fetch_add(1);
                check.sum.fetch_add(i);
                return std::move(msg);
            };

            auto sub = actor.subscribe(topic, std::move(connection), cb);
            while (check.counter.load() < check.N) {
                xmsg::util::sleep(100);
            }
            actor.unsubscribe(std::move(sub));
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }};

    xmsg::util::sleep(100);

    auto pub_thread = std::thread{[&](){
        try {
            auto actor = xmsg::xMsg{"test_publisher"};
            auto connection = actor.connect();
            xmsg::util::sleep(100);

            auto topic = xmsg::Topic::raw("test_topic");
            for (int i = 0; i < check.N; i++) {
                auto msg = create_message(topic, i);
                actor.publish(connection, msg);
            }
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }};

    sub_thread.join();
    pub_thread.join();

    proxy_thread.stop();

    ASSERT_THAT(check.counter.load(), Eq(check.N));
    ASSERT_THAT(check.sum.load(), Eq(check.SUM_N));
}



int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}