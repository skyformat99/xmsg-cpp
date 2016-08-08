/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim: set ts=8 sts=4 et sw=4 tw=80: */

#include <xmsg/topic.h>

#include "helper/registration.h"

#include <cstdio>
#include <cstdlib>
#include <string>

using namespace xmsg;
using namespace xmsg::detail;
using namespace xmsg::proto;

void test_registration_database(void);
void add_random(int);
void remove_random(int);
void remove_random_host(void);
void remove_host(const std::string&);
void remove_all(void);
void check(void);
void check(bool);
Registration discovery_request(const std::string&, bool);
RegDataSet find(const std::string&, bool);
bool check_publisher(const Registration&);

zmq::context_t ctx;
RegDriver driver{ctx, {}};

RegDataSet reg_data;
std::string name = "registrat_test";

int main()
{
    using Time = std::chrono::high_resolution_clock;
    using s = std::chrono::duration<float>;

    auto timer = [t = Time::now()]() {
        return std::chrono::duration_cast<s>(Time::now() - t).count();
    };

    test_registration_database();

    printf("Total time: %.2f [s]\n", timer());
    return 0;
}


void test_registration_database()
{
    add_random(10000);
    check();

    remove_random(2500);
    check();

    add_random(1000);
    check();

    remove_random_host();
    check();

    add_random(1000);
    check();

    remove_all();
    check();
}


void add_random(int size)
{
    printf("INFO: Registering %d random actors...\n", size);
    for (int i = 0; i < size; i++) {
        auto reg = test::random_registration();
        driver.add(reg, check_publisher(reg));
        reg_data.insert(reg);
    }
}


void remove_random(int size)
{
    printf("INFO: Removing %d random actors...\n", size);

    std::uniform_int_distribution<int> gen(0, reg_data.size() - size);
    auto first = gen(test::rng);
    auto end = first + size;
    auto i = 0;
    for (auto reg_it = reg_data.begin(); reg_it != reg_data.end(); ) {
        if (i == end) {
            break;
        }
        if (i >= first) {
            driver.remove(*reg_it, check_publisher(*reg_it));
            reg_it = reg_data.erase(reg_it);
        } else {
            ++reg_it;
        }
        ++i;
    }
}


void remove_random_host()
{
    remove_host(test::random(test::hosts));
}


void remove_host(const std::string& host)
{
    printf("INFO: Removing host %s\n", host.data());
    for (auto it = reg_data.begin(); it != reg_data.end(); ) {
        if (it->host() == host) {
            it = reg_data.erase(it);
        } else {
            ++it;
        }
    }
    driver.remove_all("test", host);
}


void remove_all()
{
    printf("INFO: Removing all actors\n");
    for (auto& host : test::hosts) {
        driver.remove_all("test", host);
    }
    reg_data.clear();
}


void check(void)
{
    check(true);
    check(false);
}


void check(bool is_publisher)
{
    for (auto& topic : test::topics) {
        auto data = discovery_request(topic, is_publisher);

        auto result = driver.find(data, is_publisher);
        auto expected = find(topic, is_publisher);

        if (result == expected) {
            auto owner = is_publisher ? "publishers" : "subscribers";
            printf("Found %3zd %s for %s\n", result.size(), owner, topic.data());
        } else {
            printf("Topic: %s\n", topic.data());
            printf("Result: %zd\n", result.size());
            printf("Expected: %zd\n", expected.size());
            printf("Sets doesn't match!!!\n");
            exit(EXIT_FAILURE);
        }
    }
}


Registration discovery_request(const std::string& topic, bool is_publisher)
{
    return test::new_registration(name, "localhost", topic, is_publisher);
}


RegDataSet find(const std::string& topic, bool is_publisher)
{
    RegDataSet data;
    Topic search_topic = Topic::raw(topic);
    for (auto& reg : reg_data) {
        if (is_publisher != check_publisher(reg)) {
            continue;
        }
        auto reg_topic = Topic::build(reg.domain(), reg.subject(), reg.type());
        if (search_topic.is_parent(reg_topic)) {
            data.insert(reg);
        }
    }
    return data;
}


bool check_publisher(const Registration& reg)
{
    return reg.ownertype() == Registration::PUBLISHER;
}
