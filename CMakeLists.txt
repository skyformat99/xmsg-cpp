project(xMsg C CXX)
cmake_minimum_required(VERSION 3.1.3)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to RelWithDebInfo")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()


if(NOT CMAKE_INSTALL_BINDIR)
  set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
endif()

if(NOT CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
endif()

if(NOT CMAKE_INSTALL_SHAREDIR)
  set(CMAKE_INSTALL_SHAREDIR "share" CACHE PATH "other files (share)")
endif()

if(NOT CMAKE_INSTALL_INCLUDEDIR)
  set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "header files (include)")
endif()

if(NOT CMAKE_INSTALL_CONFIGDIR)
  set(CMAKE_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/xmsg"
    CACHE PATH "CMake project files (${CMAKE_INSTALL_LIBDIR}/cmake/xmsg")
endif()

mark_as_advanced(
  CMAKE_INSTALL_BINDIR
  CMAKE_INSTALL_LIBDIR
  CMAKE_INSTALL_SHAREDIR
  CMAKE_INSTALL_INCLUDEDIR
  CMAKE_INSTALL_CONFIGDIR
)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(GCC_MIN_VERSION 4.9.2)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GCC_MIN_VERSION)
    message(FATAL_ERROR "Expected GCC >= ${GCC_MIN_VERSION} but found ${CMAKE_CXX_COMPILER_VERSION}")
  endif()
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-missing-braces")

if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_SKIP_BUILD_RPATH FALSE)
  set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir)
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
  endif()
endif()


find_package(Protobuf 2.6 REQUIRED)
find_package(zmq 4.0 REQUIRED)


option(XMSG_BUILD_TESTS "Build tests" ON)

add_subdirectory("src/core")
add_subdirectory("src/proto")
add_subdirectory("src/sys")
add_subdirectory("doc")

if(XMSG_BUILD_TESTS)
  enable_testing()
  include("cmake/External/GMock.cmake")
  add_subdirectory("test/core")
  add_subdirectory("test/perf")
endif()


# export targets
include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/xmsg-config.cmake.in
  ${PROJECT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/xmsg-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_CONFIGDIR}
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR)

install(EXPORT xMsgTargets
  FILE xmsg-targets.cmake
  NAMESPACE xMsg::
  DESTINATION ${CMAKE_INSTALL_CONFIGDIR})

install(FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/xmsg-config.cmake
  DESTINATION ${CMAKE_INSTALL_CONFIGDIR} COMPONENT Devel)


# uninstall target
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake)
