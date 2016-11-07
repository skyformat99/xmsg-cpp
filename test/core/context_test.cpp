/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include <xmsg/context.h>

#include <gmock/gmock.h>

#include <future>

using namespace testing;


TEST(Context, SingletonSingleThreadInstances)
{
    auto ctx1 = xmsg::Context::instance();
    auto ctx2 = xmsg::Context::instance();

    ASSERT_THAT(ctx1, Eq(ctx2));
}


TEST(Context, SingletonMultiThreadInstances)
{
    auto l = []() { return xmsg::Context::instance(); };

    auto f1 = std::async(std::launch::async, l);
    auto f2 = std::async(std::launch::async, l);

    auto ctx1 = f1.get();
    auto ctx2 = f2.get();

    ASSERT_THAT(ctx1, Eq(ctx2));
}


TEST(Context, CreateNewContexts)
{
    auto ctx1 = xmsg::Context::create();
    auto ctx2 = xmsg::Context::create();

    ASSERT_THAT(ctx1.get(), Ne(ctx2.get()));
}


TEST(Context, SetIOThreads)
{
    auto ctx = xmsg::Context::create();

    ctx->set_io_threads(4);

    ASSERT_THAT(ctx->io_threads(), Eq(4));
}


TEST(Context, SetMaxSockets)
{
    auto ctx = xmsg::Context::create();

    ctx->set_max_sockets(2048);

    ASSERT_THAT(ctx->max_sockets(), Eq(2048));
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
