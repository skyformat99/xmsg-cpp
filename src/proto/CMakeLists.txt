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

add_library(xmsgproto OBJECT ${PROTO_SOURCES} ${PROTO_HEADERS})
set_property(TARGET xmsgproto PROPERTY COMPILE_FLAGS "-fPIC")

add_library(xmsgproto_headers INTERFACE)
target_link_libraries(xmsgproto_headers INTERFACE ${PROTOBUF_LIBRARIES})
target_include_directories(xmsgproto_headers INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PROTOBUF_INCLUDE_DIRS})

install(FILES ${PROTO_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmsg)
install(FILES ${PROTO_WRAPPERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmsg)
