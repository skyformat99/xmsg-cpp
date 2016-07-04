/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "zhelper.h"

#include "gmock/gmock.h"

#include <iostream>
#include <limits>
#include <thread>

using namespace testing;
using namespace xmsg;

TEST(UniqueReplyTo, GenerateUniqueReplyTo)
{
    core::set_unique_replyto(-1);

    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000000"));
    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000001"));
    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000002"));

    core::set_unique_replyto(900000);

    auto t1 = std::thread{[](){
            for (int i = 1; i < 50000; i++) {
                core::get_unique_replyto("id");
            }
    }};
    auto t2 = std::thread{[](){
            for (int i = 0; i < 40000; i++) {
                core::get_unique_replyto("id");
            }
    }};

    t1.join();
    t2.join();

    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2990000"));

    for (int i = 1; i < 10000; i++) {
        core::get_unique_replyto("id");
    }

    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000000"));
    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000001"));
    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000002"));
}


TEST(UniqueReplyTo, OverflowUniqueReplyTo)
{
    core::set_unique_replyto(std::numeric_limits<std::uint_fast32_t>::max());

    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000000"));
    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000001"));
    ASSERT_THAT(core::get_unique_replyto("id"), StrEq("ret:id:2000002"));
}


TEST(UniqueReplyTo, EncodeIdentity) {
    auto encode = core::encode_identity("10.2.9.1", "test_actor");

    ASSERT_EQ(8u, encode.size());
}


TEST(RandomControlId, NineDigitsLong) {
    auto id1 = core::get_random_id();
    auto id2 = core::get_random_id();
    auto id3 = core::get_random_id();

    ASSERT_EQ(9u, id1.size());
    ASSERT_EQ(9u, id2.size());
    ASSERT_EQ(9u, id3.size());
}


TEST(RandomControlId, SameThreeDigitsPrefix) {
    auto pre1 = core::get_random_id().substr(1, 4);
    auto pre2 = core::get_random_id().substr(1, 4);
    auto pre3 = core::get_random_id().substr(1, 4);

    ASSERT_EQ(pre1, pre2);
    ASSERT_EQ(pre2, pre3);
}


TEST(RandomControlId, FourthDigitIsCppIdentifier) {
    auto id1 = core::get_random_id();
    auto id2 = core::get_random_id();
    auto id3 = core::get_random_id();

    ASSERT_EQ('2', id1[0]);
    ASSERT_EQ('2', id2[0]);
    ASSERT_EQ('2', id3[0]);
}


TEST(RandomControlId, RamdomLastFiveDigits) {
    auto suff1 = core::get_random_id().substr(4);
    auto suff2 = core::get_random_id().substr(4);
    auto suff3 = core::get_random_id().substr(4);

    ASSERT_NE(suff1, suff2);
    ASSERT_NE(suff1, suff3);
    ASSERT_NE(suff2, suff3);
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
