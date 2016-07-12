add_executable(cx_proxy cx_proxy.cpp)
target_link_libraries(cx_proxy xmsg)

install(TARGETS cx_proxy DESTINATION ${CMAKE_INSTALL_BINDIR})
