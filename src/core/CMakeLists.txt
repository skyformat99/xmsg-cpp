set(XMSG_FILES
  xmsg.cpp
  address.cpp
  context.cpp
  connection_driver.cpp
  connection_pool.cpp
  connection_setup.cpp
  proxy.cpp
  registration_driver.cpp
  topic.cpp
  subscription.cpp
  util.cpp
  zhelper.cpp
)

add_library(xmsg SHARED ${XMSG_FILES} $<TARGET_OBJECTS:xmsgproto>)
target_link_libraries(xmsg xmsgproto_headers ${ZMQ_LIBRARIES})
target_include_directories(xmsg PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${ZMQ_INCLUDE_DIRS}
  )

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/xmsg/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmsg)
install(TARGETS xmsg DESTINATION ${CMAKE_INSTALL_LIBDIR})
