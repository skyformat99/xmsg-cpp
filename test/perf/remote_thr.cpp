#include "xmsg.h"
#include "util.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    if (argc != 4) {
        std::cerr << "usage: remote_thr <bind-to> <message-size> <message-count>" << std::endl;
        return EXIT_FAILURE;
    }

    const auto bind_to = xmsg::util::to_host_addr(argv[1]);
    const auto message_size = std::stoi(argv[2]);
    const auto message_count = std::stol(argv[3]);

    try {
        auto publisher = xmsg::xMsg("thr_publisher");
        auto connection = publisher.connect(bind_to);

        xmsg::util::sleep(100);

        auto topic = xmsg::Topic::raw("thr_topic");
        auto meta = std::make_unique<xmsg::proto::MetaData>();
        auto data = std::vector<std::uint8_t>(message_size);
        meta->set_datatype("data/binary");

        auto msg = xmsg::Message{topic, std::move(meta), data};

        for (int i = 0; i < message_count; ++i) {
            publisher.publish(connection, msg);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
