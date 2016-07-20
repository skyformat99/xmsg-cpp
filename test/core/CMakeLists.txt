include_directories(${CMAKE_SOURCE_DIR}/src/core)

add_executable(test_context context_test.cpp)
add_test(test_context test_context)
target_link_libraries(test_context xmsg gmock)

add_executable(test_message message_test.cpp)
add_test(test_message test_message)
target_link_libraries(test_message xmsg gmock)

add_executable(test_topic topic_test.cpp)
add_test(test_topic test_topic)
target_link_libraries(test_topic xmsg gmock)

add_executable(test_util util_test.cpp)
add_test(test_util test_util)
target_link_libraries(test_util xmsg gmock)

add_executable(test_helper zhelper_test.cpp)
add_test(test_helper test_helper)
target_link_libraries(test_helper xmsg gmock)

add_executable(test_subscription subscription_test.cpp)
target_link_libraries(test_subscription xmsg gmock)

add_executable(test_regdis regdis_test.cpp)
add_test(test_regdis test_regdis)
target_link_libraries(test_regdis xmsg gmock)

add_executable(test_registration registrar_test.cpp)
target_link_libraries(test_registration xmsg gmock)
