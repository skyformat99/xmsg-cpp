# xMsg

xMsg is a lightweight, yet full featured publish/subscribe messaging system,
presenting asynchronous publish/subscribe inter-process communication
protocol: an API layer in Java, Python and C++.

xMsg provides in memory registration database that is used to register xMsg
actors (i.e. publishers and subscribers). Hence, xMsg API includes methods for
registering and discovering publishers and subscribers. This makes xMsg a
suitable framework to build symmetric SOA based applications. For example a
services that has a message to publishing can check to see if there are enough
subscribers of this particular message type.

To solve dynamic discovery problem in pub/sub environment the need of a proxy
server is unavoidable. xMsg is using 0MQ socket libraries and borrows 0MQ
proxy, which is a simple stateless message switch to address mentioned dynamic
discovery problem.

xMsg stores proxy connection objects internally in a connection pool for
efficiency reasons. To avoid proxy connection concurrency, thus achieving
maximum performance, connection objects are not shared between threads. Each
xMsg actor tread will reuse an available connection object, or create a new
proxy connection if it is not available in the pool.

xMsg publisher can send a message of any topic. xMsg subscribers subscribe
to abstract topics and provide callbacks to handle messages as they arrive,
in a so called subscribe-and-forget mode. Neither publisher nor subscriber
knows of each others existence. Thus publishers and subscribers are completely
independent of each others. Yet, for a proper communication they need to
establish some kind of relationship or binding, and that binding is the
communication or message topic. Note that multiple xMsg actors can
communicate without interfering with each other via simple topic
naming conventions. xMsg topic convention defines three parts: _domain_,
_subject_, and _type_, presented by the Topic class.

xMsg subscriber callbacks will run in a separate thread. For that reason xMsg
provides a thread pool, simplifying the job of a user. Note that user provided
callback routines must be thread safe and/or thread enabled.

In a conclusion we present the xMsg entire API

    connect
    release
    publish
    sync_publish
    subscribe
    unsubscribe
    register_as_publisher
    register_as_subscriber
    deregister_as_publisher
    deregister_as_subscriber
    find_publishers
    find_subscribers

For more details and API method signatures check the Doxygen documentation.


## Build notes

xMsg requires a C++14 compiler (GCC 4.9+) and CMake 3.1+

#### Ubuntu 14.04

Support PPAs:

    sudo apt-get install software-properties-common

Add a PPA for GCC (do not use GCC 5.x due to ABI changes):

    sudo add-apt-repository ppa:ubuntu-toolchain-r/test
    sudo apt-get update
    sudo apt-get install build-essential gcc-4.9 g++-4.9

Set GCC 4.9 as default:

    sudo update-alternatives \
            --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 60 \
            --slave /usr/bin/g++ g++ /usr/bin/g++-4.9

Add a PPA for CMake:

    sudo add-apt-repository ppa:george-edison55/cmake-3.x
    sudo apt-get update
    sudo apt-get install cmake

#### Ubuntu 16.04

Install GCC and CMake from the repositories:

    sudo apt-get install build-essential cmake

#### OS X El Capitan

Install Xcode command line tools:

    xcode-select --install

Install CMake using [Homebrew](http://brew.sh/):

    brew install cmake

### Dependencies

xMsg uses [Protocol Buffers](https://developers.google.com/protocol-buffers/docs/downloads)
and [ZeroMQ](http://zeromq.org/area:download).

#### Ubuntu 14.04 and 16.04

Install from the repositories:

    sudo apt-get install libzmq3-dev libprotobuf-dev protobuf-compiler

#### OS X

Use Homebrew:

    brew tap homebrew/versions
    brew install zmq protobuf260

### Installation

To build with CMake a configure wrapper script is provided:

    ./configure --prefix=<INSTALL_DIR>
    make
    make install


## Authors

For assistance contact authors:

* Vardan Gyurjyan    (<gurjyan@jlab.org>)
* Sebastián Mancilla (<smancill@jlab.org>)
* Ricardo Oyarzún    (<oyarzun@jlab.org>)

Enjoy...
