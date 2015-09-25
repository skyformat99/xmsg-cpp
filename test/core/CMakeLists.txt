include_directories(${CMAKE_SOURCE_DIR}/include/xmsg)
include_directories(${GMOCK_INCLUDE_DIRS})

add_executable(test_topic topic_test.cpp)
add_dependencies(test_topic googlemock)
add_test(test_topic test_topic)
target_link_libraries(test_topic xmsg ${GMOCK_LIBRARIES})

add_executable(test_util util_test.cpp)
add_dependencies(test_util googlemock)
add_test(test_util test_util)
target_link_libraries(test_util xmsg ${GMOCK_LIBRARIES})
