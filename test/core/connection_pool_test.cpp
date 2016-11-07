/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include <xmsg/connection_pool.h>

#include "connection_driver.h"
#include "registration_driver.h"

#include <gmock/gmock.h>


using namespace testing;
using namespace xmsg;

auto ctx = std::make_shared<zmq::context_t>();


class ConnectionPoolMock : public ConnectionPool {
public:
    ConnectionPoolMock()
        : ConnectionPool(ctx)
    { }

public:
    int new_connections = 0;

private:
    detail::ProxyDriverPtr create_connection(const ProxyAddress& addr) override
    {
        new_connections++;
        return detail::ProxyDriverPtr{new detail::ProxyDriver(*ctx, addr, {})};
    }

    detail::RegDriverPtr create_connection(const RegAddress& addr) override
    {
        new_connections++;
        return detail::RegDriverPtr{new detail::RegDriver(*ctx, addr)};
    }
};


template<typename A, typename U>
class ConnectionPoolTest : public Test
{
public:
    void create_connection()
    {
        auto addr1 = A{"10.2.9.1"};
        auto addr2 = A{"10.2.9.2"};

        auto c1 = pool->get_connection(addr1);
        auto c2 = pool->get_connection(addr2);
        auto c3 = pool->get_connection(addr2);

        EXPECT_THAT(pool->new_connections, Eq(3));

        EXPECT_THAT(c1.address(), Eq(addr1));
        EXPECT_THAT(c2.address(), Eq(addr2));
        EXPECT_THAT(c3.address(), Eq(addr2));

        EXPECT_THAT(c1.get(), Ne(c2.get()));
        EXPECT_THAT(c1.get(), Ne(c3.get()));
        EXPECT_THAT(c2.get(), Ne(c3.get()));
    }


    void reuse_connection()
    {
        auto addr1 = A{"10.2.9.1"};
        auto addr2 = A{"10.2.9.2"};

        typename U::pointer pc1;
        typename U::pointer pc2;
        typename U::pointer pc3;
        {
            U cc1 = pool->get_connection(addr1);
            U cc2 = pool->get_connection(addr2);
            U cc3 = pool->get_connection(addr2);

            pc1 = cc1.get();
            pc2 = cc2.get();
            pc3 = cc3.get();
        }

        auto c3 = pool->get_connection(addr2);
        auto c2 = pool->get_connection(addr2);
        auto c1 = pool->get_connection(addr1);

        EXPECT_THAT(pool->new_connections, Eq(3));

        EXPECT_THAT(c1.get(), Eq(pc1));
        EXPECT_THAT(c2.get(), Eq(pc2));
        EXPECT_THAT(c3.get(), Eq(pc3));
    }

private:
    std::unique_ptr<ConnectionPoolMock> pool = std::make_unique<ConnectionPoolMock>();
};


using ProxyConnectionTest = ConnectionPoolTest<ProxyAddress, ProxyConnection>;
using RegConnectionTest = ConnectionPoolTest<RegAddress, RegConnection>;


TEST_F(ProxyConnectionTest, GetConnection)
{
    create_connection();
}

TEST_F(ProxyConnectionTest, ReleaseConnection)
{
    reuse_connection();
}



TEST_F(RegConnectionTest, GetConnection)
{
    create_connection();
}

TEST_F(RegConnectionTest, ReleaseConnection)
{
    reuse_connection();
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
