/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include <xmsg/topic.h>

#include <gmock/gmock.h>

using namespace testing;
using namespace xmsg;

using TopicQuery = std::string(Topic::*)(void) const;


template<typename S>
inline void assert_topic(const Topic& t, const S& s)
{
    EXPECT_THAT(t.str(), StrEq(s));
}


template<typename T, typename S>
inline void assert_call(const T& topic, TopicQuery fun, const S& part)
{
    EXPECT_THAT((Topic::raw(topic).*fun)(), StrEq(part));
}


template<typename T, typename S>
inline void assert_parent(const T& t1, const S& t2)
{
    EXPECT_TRUE(Topic::raw(t1).is_parent(Topic::raw(t2)));
}

template<typename T, typename S>
inline void assert_not_parent(const T& t1, const S& t2)
{
    EXPECT_FALSE(Topic::raw(t1).is_parent(Topic::raw(t2)));
}


TEST(Topic, BuildWithValidValues) {
    assert_topic(Topic::build("rock"), "rock");
    assert_topic(Topic::build("rock", "metal"), "rock:metal");
    assert_topic(Topic::build("rock", "metal", "metallica"), "rock:metal:metallica");
}


TEST(Topic, BuildWithExtendedTopic) {
    assert_topic(Topic::build("rock", "metal", "metallica:lars:*"),
                 "rock:metal:metallica:lars");
    assert_topic(Topic::build("rock", "metal", "metallica:lars:james"),
                 "rock:metal:metallica:lars:james");
}


TEST(Topic, BuildWithValidUndefinedValues) {
    assert_topic(Topic::build("rock", "*"), "rock");
    assert_topic(Topic::build("rock", "*", "*"), "rock");
    assert_topic(Topic::build("rock", "metal", "*"), "rock:metal");
}


TEST(Topic, BuildWithInvalidUndefinedValues) {
    auto check_exception = [] (auto... params) {
        ASSERT_THROW(Topic::build(params...), std::invalid_argument);
    };
    check_exception("*");
    check_exception("*", "master");
    check_exception("rock", "*", "metallica");
}


TEST(Topic, WrapRawTopic) {
    auto assert_raw = [] (auto t) {
        assert_topic(Topic::raw(t), t);
    };
    assert_raw("rock");
    assert_raw("rock:metal");
    assert_raw("rock:metal:metallica");
    assert_raw("rock:metal:metallica:lars:james");
}


TEST(Topic, GetDomain) {
    auto assert_domain = [] (auto t) {
        assert_call(t, &Topic::domain, "rock");
    };
    assert_domain("rock");
    assert_domain("rock:metal");
    assert_domain("rock:metal:metallica");
    assert_domain("rock:metal:metallica:lars");
}


TEST(Topic, GetSubject) {
    auto assert_subject = [] (auto t, auto s) {
        assert_call(t, &Topic::subject, s);
    };
    assert_subject("rock", Topic::ANY);
    assert_subject("rock:metal", "metal");
    assert_subject("rock:metal:metallica", "metal");
    assert_subject("rock:metal:metallica:lars", "metal");
}


TEST(Topic, GetType) {
    auto assert_type = [] (auto t, auto s) {
        assert_call(t, &Topic::type, s);
    };
    assert_type("rock", Topic::ANY);
    assert_type("rock:metal", Topic::ANY);
    assert_type("rock:metal:metallica", "metallica");
    assert_type("rock:metal:metallica:lars", "metallica:lars");
}


TEST(Topic, IsParentCompareDomain) {
    assert_parent("rock", "rock");
    assert_parent("rock", "rock:metal");
    assert_parent("rock", "rock:metal:slayer");

    assert_not_parent("rock", "movies");
    assert_not_parent("rock", "movies:classic");
    assert_not_parent("rock", "movies:classic:casablanca");
}


TEST(Topic, IsParentCompareSubject) {
    assert_parent("rock:metal", "rock:metal");
    assert_parent("rock:metal", "rock:metal:metallica");

    assert_not_parent("rock:metal", "rock");
    assert_not_parent("rock:metal", "rock:alternative");
    assert_not_parent("rock:metal", "rock:alternative:nirvana");

    assert_not_parent("rock:metal", "movies");
    assert_not_parent("rock:metal", "movies:thriller");
    assert_not_parent("rock:metal", "movies:thriller:shark");
}


TEST(Topic, IsParentCompareType) {
    assert_parent("rock:metal:metallica", "rock:metal:metallica");

    assert_not_parent("rock:metal:metallica", "rock");

    assert_not_parent("rock:metal:metallica", "rock:metal");
    assert_not_parent("rock:metal:metallica", "rock:metal:slayer");

    assert_not_parent("rock:metal:metallica", "rock:alternative");
    assert_not_parent("rock:metal:metallica", "rock:alternative:nirvana");

    assert_not_parent("rock:metal:metallica", "movies");
    assert_not_parent("rock:metal:metallica", "movies:thriller");
    assert_not_parent("rock:metal:metallica", "movies:thriller:shark");
}



int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
