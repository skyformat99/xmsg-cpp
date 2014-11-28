include_directories(${PROTOBUF_INCLUDE_DIRS})

set(PROTO_FILES
  data.proto
  meta.proto
  registration.proto
)

set(PROTO_WRAPPERS
  data.h
  meta.h
  registration.h
)

PROTOBUF_GENERATE_CPP(PROTO_SOURCES PROTO_HEADERS ${PROTO_FILES})
add_library(xmsgproto STATIC ${PROTO_SOURCES} ${PROTO_HEADERS})
target_link_libraries(xmsgproto ${PROTOBUF_LIBRARIES})
target_include_directories(xmsgproto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

install(FILES ${PROTO_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmsg)
install(FILES ${PROTO_WRAPPERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmsg)
