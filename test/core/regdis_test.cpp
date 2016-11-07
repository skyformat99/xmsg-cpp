/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include "helper/registration.h"

#include <gmock/gmock.h>

#include <iostream>

using namespace testing;

using namespace xmsg;
using namespace xmsg::detail;
using namespace xmsg::test;


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
    auto data = RegDataSet{
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



class MockContext : public Context
{
};

class MockDriver : public RegDriver
{
    friend struct DriverTest;

    MockDriver(MockContext& ctx)
      : RegDriver{ctx, {}} {};

    MOCK_METHOD2(request, Response(Request&, int));
};

struct DriverTest : public Test
{
    DriverTest()
      : driver{ctx}
      , subscriber{new_registration("bradbury_sub", "localhost",
                                    "writer:scifi:books", false)}
      , publisher{new_registration("bradbury_pub", "localhost",
                                   "writer:scifi:books", true)}
    {
        ON_CALL(driver, request(_, _))
            .WillByDefault(Return(Response{"", "", ""}));
    }

    void set_response(const Response& response)
    {
        EXPECT_CALL(driver, request(_, _)).WillOnce(Return(response));
    }

    template<typename T>
    void expect_request(const std::string& name,
                        T&& data,
                        const std::string& topic,
                        int timeout)
    {
        auto reg = Request{topic, name, std::forward<T>(data)};
        EXPECT_CALL(driver, request(reg, timeout));
    }

    MockContext ctx;
    MockDriver driver;
    proto::Registration subscriber;
    proto::Registration publisher;
};


TEST_F(DriverTest, SendPublisherRegistration)
{
    expect_request("bradbury_pub",
                   publisher,
                   constants::register_publisher,
                   constants::register_request_timeout);
    driver.add(publisher, true);
}

TEST_F(DriverTest, SendSubscriberRegistration)
{
    expect_request("bradbury_sub",
                   subscriber,
                   constants::register_subscriber,
                   constants::register_request_timeout);
    driver.add(subscriber, false);
}

TEST_F(DriverTest, SendPublisherRemoval)
{
    expect_request("bradbury_pub",
                   publisher,
                   constants::remove_publisher,
                   constants::remove_request_timeout);
    driver.remove(publisher, true);
}

TEST_F(DriverTest, SendSubscriberRemoval)
{
    expect_request("bradbury_sub",
                   subscriber,
                   constants::remove_subscriber,
                   constants::remove_request_timeout);
    driver.remove(subscriber, false);
}

TEST_F(DriverTest, SendHostRemoval)
{
    expect_request("10.2.9.1_node",
                   "10.2.9.1",
                   constants::remove_all_registration,
                   constants::remove_request_timeout);
    driver.remove_all("10.2.9.1_node", "10.2.9.1");
}


TEST_F(DriverTest, SendPublisherFind)
{
    auto data = new_registration("10.2.9.1_node", "10.2.9.1",
                                 "bradbury:scifi:books", true);
    expect_request("10.2.9.1_node",
                   data,
                   constants::find_publisher,
                   constants::find_request_timeout);
    driver.find(data, true);
}


TEST_F(DriverTest, SendSubscriberFind)
{
    auto data = new_registration("10.2.9.1_node", "10.2.9.1",
                                 "bradbury:scifi:books", false);
    expect_request("10.2.9.1_node",
                   data,
                   constants::find_subscriber,
                   constants::find_request_timeout);
    driver.find(data, false);
}


TEST_F(DriverTest, GetRegistration)
{
    auto all_reg = RegDataSet{publisher, subscriber};
    auto data = new_registration("10.2.9.1_node", "10.2.9.1",
                                 "bradbury:scifi:books", false);
    set_response({"", "", all_reg});

    auto res = driver.find(data, false);

    EXPECT_THAT(res, ContainerEq(all_reg));
}



int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
