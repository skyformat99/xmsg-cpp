include_directories(${CMAKE_SOURCE_DIR}/include/xmsg)

set(XMSG_FILES
  util.cpp
)

add_library(xmsg SHARED ${XMSG_FILES})
target_link_libraries(xmsg xmsgproto)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/xmsg/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmsg)
install(TARGETS xmsg DESTINATION ${CMAKE_INSTALL_LIBDIR})
