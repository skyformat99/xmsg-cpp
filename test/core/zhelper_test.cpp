/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "zhelper.h"

#include "gmock/gmock.h"

#include <iostream>

using namespace testing;
using namespace xmsg;

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
    auto pre1 = core::get_random_id().substr(0, 3);
    auto pre2 = core::get_random_id().substr(0, 3);
    auto pre3 = core::get_random_id().substr(0, 3);

    ASSERT_EQ(pre1, pre2);
    ASSERT_EQ(pre2, pre3);
}


TEST(RandomControlId, FourthDigitIsCppIdentifier) {
    auto id1 = core::get_random_id();
    auto id2 = core::get_random_id();
    auto id3 = core::get_random_id();

    ASSERT_EQ('2', id1[3]);
    ASSERT_EQ('2', id2[3]);
    ASSERT_EQ('2', id3[3]);
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
