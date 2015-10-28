/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "message.h"

#include "helper/utils.h"
#include "gmock/gmock.h"

using namespace testing;

namespace mt = xmsg::mimetype;

namespace {

auto make_test_data()
{
    auto data = xmsg::proto::Data{};
    data.set_flsint32(29);
    data.set_float_(42.f);
    data.set_string("november");
    data.add_doublea(0.);
    data.add_doublea(9.);
    return data;
}

const xmsg::Topic topic = xmsg::Topic::raw("test/topic");

}


TEST(Data, Serialize)
{
    auto data = make_test_data();
    auto buffer = xmsg::proto::serialize_data(data);
    auto result = xmsg::proto::parse_data(buffer);

    EXPECT_THAT(result, Eq(data));
}


TEST(Message, CreateWithBytes)
{
    auto data = std::vector<std::uint8_t>{ 0x0, 0x1, 0x2, 0x3, 0xa, 0xb };
    auto msg = xmsg::Message{topic, "test/binary", data};

    EXPECT_THAT(msg.data(), ContainerEq(data));
    EXPECT_THAT(msg.meta()->datatype(), StrEq("test/binary"));
}


TEST(Message, PassingNullMetadataThrows)
{
    auto data = std::vector<std::uint8_t>{ 0x0, 0x1, 0x2, 0x3, 0xa, 0xb };
    auto meta = std::unique_ptr<xmsg::proto::Meta>{};

    EXPECT_EXCEPTION(xmsg::Message(topic, std::move(meta), data),
                     std::invalid_argument,  "null metadata");
}


TEST(Message, PassingNullMimeTypeThrows)
{
    auto data = std::vector<std::uint8_t>{ 0x0, 0x1, 0x2, 0x3, 0xa, 0xb };
    EXPECT_EXCEPTION(xmsg::Message(topic, nullptr, data),
                     std::invalid_argument,  "null mime-type");
}


TEST(Message, EqualMessages)
{
    auto data = std::vector<std::uint8_t>{ 0x0, 0x1, 0x2, 0x3, 0xa, 0xb };

    auto msg1 = xmsg::Message{topic, "test/binary", data};
    auto msg2 = xmsg::Message{topic, "test/binary", data};

    ASSERT_TRUE(msg1 == msg2);
}


TEST(Message, CreateCopy)
{
    auto data = std::vector<std::uint8_t>{ 0x0, 0x1, 0x2, 0x3, 0xa, 0xb };

    auto msg1 = xmsg::Message{topic, "test/binary", data};
    auto msg2 = xmsg::Message{msg1};

    ASSERT_TRUE(msg1 == msg2);
}


TEST(Message, SwapMessages)
{
    auto topic1 = xmsg::Topic::raw("topic1");
    auto topic2 = xmsg::Topic::raw("topic2");
    auto data1 = std::vector<std::uint8_t>{ 0x0, 0x1, 0x2, 0x3, 0xa, 0xb };
    auto data2 = std::vector<std::uint8_t>{ 0x0, 0x4, 0x5, 0x6, 0xc, 0xd };

    auto msg1 = xmsg::Message{topic1, "test/binary1", data1};
    auto msg2 = xmsg::Message{topic2, "test/binary2", data2};

    auto copy1 = msg1;
    auto copy2 = msg2;

    swap(copy1, copy2);

    ASSERT_THAT(copy1, Eq(msg2));
    ASSERT_THAT(copy2, Eq(msg1));
}


TEST(Message, CreateWithProtoData)
{
    auto data = make_test_data();
    auto msg = xmsg::make_message(topic, data);
    auto result = xmsg::parse_message(msg);

    EXPECT_THAT(result, Eq(data));
    EXPECT_THAT(msg.meta()->datatype(), StrEq(mt::xmsg_data));
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
