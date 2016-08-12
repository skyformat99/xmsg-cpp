# - try to find evio library
#
# Non-cache variables you might use in your CMakeLists.txt:
#  ZMQ_FOUND
#  ZMQ_INCLUDE_DIRS
#  ZMQ_LIBRARIES

set(ZMQ
  "${ZMQ}"
  CACHE
  PATH
  "Directory to search for zmq")

find_package(Threads REQUIRED)

if(CMAKE_SIZEOF_VOID_P MATCHES "8")
  set(_LIBSUFFIXES lib64 lib)
else()
  set(_LIBSUFFIXES lib)
endif()

find_library(ZMQ_LIBRARY
  NAMES
  zmq
  PATHS
  "${ZMQ}"
  PATH_SUFFIXES
  ${_LIBSUFFIXES})

get_filename_component(_libdir "${ZMQ_LIBRARY}" PATH)

find_path(ZMQ_INCLUDE_DIR
  NAMES
  zmq.h
  HINTS
  "${_libdir}/.."
  PATHS
  "${ZMQ}"
  PATH_SUFFIXES
  include include/zmq)

if(DEFINED ZMQ_INCLUDE_DIR)
  file(READ "${ZMQ_INCLUDE_DIR}/zmq.h" _ZMQ_VERSION_H_CONTENTS)
  string(REGEX MATCH "#define ZMQ_VERSION_MAJOR ([0-9])" _MATCH "${_ZMQ_VERSION_H_CONTENTS}")
  set(ZMQ_VERSION_MAJOR ${CMAKE_MATCH_1})
  string(REGEX MATCH "#define ZMQ_VERSION_MINOR ([0-9])" _MATCH "${_ZMQ_VERSION_H_CONTENTS}")
  set(ZMQ_VERSION_MINOR ${CMAKE_MATCH_1})
  string(REGEX MATCH "#define ZMQ_VERSION_PATCH ([0-9])" _MATCH "${_ZMQ_VERSION_H_CONTENTS}")
  set(ZMQ_VERSION_PATCH ${CMAKE_MATCH_1})
  set (ZMQ_VERSION "${ZMQ_VERSION_MAJOR}.${ZMQ_VERSION_MINOR}.${ZMQ_VERSION_PATCH}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(zmq
  REQUIRED_VARS ZMQ_LIBRARY ZMQ_INCLUDE_DIR
  VERSION_VAR ZMQ_VERSION
  FAIL_MESSAGE DEFAULT_MSG
)

if(ZMQ_FOUND)
  set(ZMQ_LIBRARIES ${ZMQ_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
  set(ZMQ_INCLUDE_DIRS ${ZMQ_INCLUDE_DIR})
  mark_as_advanced(ZMQ)
endif()

mark_as_advanced(
  ZMQ_LIBRARY
  ZMQ_INCLUDE_DIR)
