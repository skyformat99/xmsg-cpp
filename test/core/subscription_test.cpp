/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "xmsg.h"
#include "util.h"

#include "helper/proxy_wrapper.h"

#include "gmock/gmock.h"
#include "zmq.hpp"

#include <atomic>

using namespace testing;


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

    xmsg::test::ProxyThread proxy_thread{};

    auto sub_thread = std::thread{[&](){
        try {
            auto actor = xmsg::xMsg{"test_subscriber"};
            auto connection = actor.connect();
            xmsg::util::sleep(100);

            auto topic = xmsg::Topic::raw("test_topic");
            auto cb = [&] (xmsg::Message& msg) {
                auto i = xmsg::parse_message<int>(msg);
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
                auto msg = xmsg::make_message(topic, i);
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


TEST(Subscription, syncPublishReceivesAllResponses)
{
    struct Check {
        std::atomic_int counter{0};
        std::atomic_long sum{0};

        const int N = 100;
        const long SUM_N = 4950L;
    } check;

    xmsg::test::ProxyThread proxy_thread{};

    auto syncpub_thread = std::thread{[&](){
        try {
            auto sub_actor = xmsg::xMsg{"test_subscriber"};
            auto sub_con = sub_actor.connect();
            auto rep_con = sub_actor.connect();
            xmsg::util::sleep(100);

            auto sub_topic = xmsg::Topic::raw("test_topic");
            auto sub_cb = [&] (xmsg::Message& m) {
                auto r_topic = m.meta()->replyto();
                auto r_data = xmsg::parse_message<int>(m);
                auto r_msg = xmsg::make_message(xmsg::Topic::raw(r_topic), r_data);
                sub_actor.publish(rep_con, r_msg);
            };

            auto sub = sub_actor.subscribe(sub_topic, std::move(sub_con), sub_cb);
            xmsg::util::sleep(100);

            auto pub_actor = xmsg::xMsg{"test_publisher"};
            auto pub_con = pub_actor.connect();
            xmsg::util::sleep(100);

            auto pub_topic = xmsg::Topic::raw("test_topic");
            for (int i = 0; i < check.N; i++) {
                auto msg = xmsg::make_message(pub_topic, i);
                auto res_msg = pub_actor.sync_publish(pub_con, msg, 1000);
                auto data = xmsg::parse_message<int>(res_msg);
                check.counter.fetch_add(1);
                check.sum.fetch_add(data);
            }

            sub_actor.unsubscribe(std::move(sub));
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }};

    syncpub_thread.join();
    proxy_thread.stop();

    ASSERT_THAT(check.counter.load(), Eq(check.N));
    ASSERT_THAT(check.sum.load(), Eq(check.SUM_N));
}



int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
