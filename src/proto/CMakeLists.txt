set(PROTO_FILES
  data.proto
  meta.proto
  registration.proto
)

PROTOBUF_GENERATE_CPP(PROTO_SOURCES PROTO_HEADERS ${PROTO_FILES})

add_library(xmsgproto OBJECT ${PROTO_SOURCES})
set_target_properties(xmsgproto PROPERTIES
  CXX_CLANG_TIDY ""
  POSITION_INDEPENDENT_CODE ON)
target_include_directories(xmsgproto SYSTEM PRIVATE
  ${PROTOBUF_INCLUDE_DIRS}
)

add_library(xmsgproto_headers INTERFACE)
target_link_libraries(xmsgproto_headers INTERFACE ${PROTOBUF_LIBRARIES})
target_include_directories(xmsgproto_headers SYSTEM INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  ${PROTOBUF_INCLUDE_DIRS}
)

install(FILES ${PROTO_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmsg/proto)
install(TARGETS xmsgproto_headers EXPORT xMsgTargets)
