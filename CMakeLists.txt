project(xMsg C CXX)
cmake_minimum_required(VERSION 3.0.0)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()


if (NOT CMAKE_INSTALL_BINDIR)
  set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
endif()

if (NOT CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
endif()

if (NOT CMAKE_INSTALL_SHAREDIR)
  set(CMAKE_INSTALL_SHAREDIR "share" CACHE PATH "other files (share)")
endif()

if(NOT CMAKE_INSTALL_INCLUDEDIR)
  set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C header files (include)")
endif()

mark_as_advanced(CMAKE_INSTALL_BINDIR)
mark_as_advanced(CMAKE_INSTALL_LIBDIR)
mark_as_advanced(CMAKE_INSTALL_SHAREDIR)
mark_as_advanced(CMAKE_INSTALL_INCLUDEDIR)


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-missing-braces -std=c++14")

find_package(Protobuf REQUIRED)
find_package(zmq REQUIRED)


option(XMSG_BUILD_TESTS "Build tests" ON)


add_subdirectory("src/core")
add_subdirectory("src/proto")
add_subdirectory("src/sys")

if (XMSG_BUILD_TESTS)
  enable_testing()
  include("cmake/External/GMock.cmake")
  add_subdirectory("test/core")
  add_subdirectory("test/perf")
endif (XMSG_BUILD_TESTS)


# uninstall target
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
