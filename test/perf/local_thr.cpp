#include <xmsg/util.h>
#include <xmsg/xmsg.h>

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>


std::string bind_to;
long message_size;
long message_count;

long elapsed_time;

std::mutex mtx;
std::condition_variable cond;


class LocalCallback
{
public:
    void operator()(xmsg::Message& msg);

private:
    using Time = std::chrono::high_resolution_clock;
    using point = decltype(Time::now());
    using us = std::chrono::microseconds;

    void start_clock();
    long stop_clock();

    point watch;
    int nr = 0;
};


int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "usage: local_thr <bind-to> <message-size> <message-count>" << std::endl;
        return EXIT_FAILURE;
    }

    bind_to = xmsg::util::to_host_addr(argv[1]);
    message_size = std::stoi(argv[2]);
    message_count = std::stol(argv[3]);

    try {
        auto subscriber = xmsg::xMsg("thr_subscriber");
        auto connection = subscriber.connect(bind_to);
        auto topic = xmsg::Topic::raw("thr_topic");
        auto cb = LocalCallback{};

        auto sub = subscriber.subscribe(topic, std::move(connection), cb);
        std::cout << "Waiting for messages..." << std::endl;

        {
            auto lock = std::unique_lock<std::mutex>{mtx};
            cond.wait(lock);
        }

        if (elapsed_time == 0) {
            elapsed_time = 1;
        }

        long throughput = (long) (message_count / (double) elapsed_time * 1000000L);
        double megabits = (double) (throughput * message_size * 8) / 1000000;
        double latency = (double) elapsed_time / (message_count);

        printf("message elapsed: %.3f [s]\n", (double) elapsed_time / 1000000L);
        printf("message size: %ld [B]\n", message_size);
        printf("message count: %ld\n", message_count);
        printf("mean transfer time: %.3f [us]\n", latency);
        printf("mean transfer rate: %d [msg/s]\n", (int) throughput);
        printf("mean throughput: %.3f [Mb/s]\n", megabits);

        subscriber.unsubscribe(std::move(sub));

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


void LocalCallback::operator()(xmsg::Message& msg)
{
    int size = msg.data().size();
    if (size != message_size) {
        printf("message of incorrect size received %d\n", size);
        exit(EXIT_FAILURE);
    }
    ++nr;
    if (nr == 1) {
        start_clock();
    } else if (nr == message_count) {
        elapsed_time = stop_clock();
        auto lock = std::unique_lock<std::mutex>{mtx};
        cond.notify_one();
    }
}


void LocalCallback::start_clock()
{
    watch = Time::now();
}


long LocalCallback::stop_clock()
{
    return std::chrono::duration_cast<us>(Time::now() - watch).count();
}
