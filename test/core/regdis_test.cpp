/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "regdis.h"
#include "helper/registration.h"

#include "gmock/gmock.h"

#include <iostream>

using namespace testing;

using namespace xmsg;
using namespace xmsg::registration;
using namespace xmsg::registration::test;


TEST(Request, CreateDataRequest)
{
    auto data = new_registration("asimov", "10.2.9.1", "writer:scifi", true);

    auto send_req = Request{"foo:bar", "foo_service", data};
    auto recv_req = Request{send_req.msg()};

    EXPECT_THAT(recv_req.topic(), StrEq("foo:bar"));
    EXPECT_THAT(recv_req.sender(), StrEq("foo_service"));
    EXPECT_THAT(recv_req.data(), Eq(data));
}

TEST(Request, CreateTextRequest)
{
    auto send_req = Request{"foo:bar", "foo_service", "10.2.9.2"};
    auto recv_req = Request{send_req.msg()};

    EXPECT_THAT(recv_req.topic(), StrEq("foo:bar"));
    EXPECT_THAT(recv_req.sender(), StrEq("foo_service"));
    EXPECT_THAT(recv_req.text(), StrEq("10.2.9.2"));
}



TEST(Response, CreateSuccessResponse)
{
    auto send_res = Response{"foo:bar", "registration_fe"};
    auto recv_res = Response{send_res.msg()};

    EXPECT_THAT(recv_res.topic(), StrEq("foo:bar"));
    EXPECT_THAT(recv_res.sender(), StrEq("registration_fe"));
    EXPECT_THAT(recv_res.status(), StrEq(constants::success));
    EXPECT_THAT(recv_res.data(), SizeIs(0));
}

TEST(Response, CreateErrorResponse)
{
    auto error = "could not handle request";
    auto send_res = Response{"foo:bar", "registration_fe", error};

    EXPECT_THAT(send_res.topic(), StrEq("foo:bar"));
    EXPECT_THAT(send_res.sender(), StrEq("registration_fe"));
    EXPECT_THAT(send_res.status(), StrEq(error));
    EXPECT_THAT(send_res.data(), SizeIs(0));
}

TEST(Response, CreateDataResponse)
{
    auto data = RegDataSet {
        new_registration("asimov", "10.2.9.1", "writer.scifi:books", true),
        new_registration("bradbury", "10.2.9.1", "writer.scifi:books", true),
    };

    auto send_res = Response{"foo:bar", "registration_fe", data};
    auto recv_res = Response{send_res.msg()};

    EXPECT_THAT(recv_res.topic(), StrEq("foo:bar"));
    EXPECT_THAT(recv_res.sender(), StrEq("registration_fe"));
    EXPECT_THAT(recv_res.status(), StrEq(constants::success));
    EXPECT_THAT(recv_res.data(), ContainerEq(data));
}



int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
