include_directories(${CMAKE_SOURCE_DIR}/include/xmsg)
include_directories(${ZMQ_INCLUDE_DIRS})

add_executable(cx_proxy xmsg_proxy.cpp)
target_link_libraries(cx_proxy xmsg)

install(TARGETS cx_proxy DESTINATION ${CMAKE_INSTALL_BINDIR})
