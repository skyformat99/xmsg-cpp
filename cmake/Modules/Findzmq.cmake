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

find_path(ZMQ_CPP_INCLUDE_DIR
  NAMES
  zmq.hpp
  HINTS
  "${_libdir}/.."
  PATHS
  "${ZMQ}"
  PATH_SUFFIXES
  include include/zmq)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(zmq
  DEFAULT_MSG
  ZMQ_LIBRARY
  ZMQ_INCLUDE_DIR
  ZMQ_CPP_INCLUDE_DIR)

if(ZMQ_FOUND)
  set(ZMQ_LIBRARIES ${ZMQ_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
  set(ZMQ_INCLUDE_DIRS ${ZMQ_INCLUDE_DIR} ${ZMQ_CPP_INCLUDE_DIR})
  mark_as_advanced(ZMQ)
endif()

mark_as_advanced(
  ZMQ_LIBRARY
  ZMQ_INCLUDE_DIR)
