/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "zhelper.h"

#include "gmock/gmock.h"

#include <iostream>

using namespace testing;
using namespace xmsg;


TEST(RandomControlId, NineDigitsLong) {
    auto id1 = core::get_random_id();
    auto id2 = core::get_random_id();
    auto id3 = core::get_random_id();

    ASSERT_EQ(id1.size(), 9u);
    ASSERT_EQ(id2.size(), 9u);
    ASSERT_EQ(id3.size(), 9u);
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

    ASSERT_EQ(id1[3], '2');
    ASSERT_EQ(id2[3], '2');
    ASSERT_EQ(id3[3], '2');
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
