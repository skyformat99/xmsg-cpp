include_directories(SYSTEM ${PROTOBUF_INCLUDE_DIRS})

set(PROTO_FILES
  data.proto
  meta.proto
  registration.proto
)

PROTOBUF_GENERATE_CPP(PROTO_SOURCES PROTO_HEADERS ${PROTO_FILES})

add_library(xmsgproto OBJECT ${PROTO_SOURCES})
target_compile_options(xmsgproto PRIVATE -Wno-extra)
set_property(TARGET xmsgproto PROPERTY COMPILE_FLAGS "-fPIC")

add_library(xmsgproto_headers INTERFACE)
target_link_libraries(xmsgproto_headers INTERFACE ${PROTOBUF_LIBRARIES})
target_include_directories(xmsgproto_headers INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

install(FILES ${PROTO_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmsg/proto)
install(TARGETS xmsgproto_headers EXPORT xMsgTargets)
