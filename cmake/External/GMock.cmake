INCLUDE(ExternalProject)
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

ExternalProject_Add(
  googlemock
  URL https://github.com/google/googletest/archive/release-1.8.0.zip
  URL_HASH SHA1=667f873ab7a4d246062565fad32fb6d8e203ee73
  TIMEOUT 30
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  LOG_CONFIGURE ON
  LOG_BUILD ON
)

find_package(Threads)

ExternalProject_Get_Property(googlemock SOURCE_DIR)
ExternalProject_Get_Property(googlemock BINARY_DIR)

# avoid error with set_target_properties
file(MAKE_DIRECTORY "${SOURCE_DIR}/googlemock/include")
file(MAKE_DIRECTORY "${SOURCE_DIR}/googletest/include")

add_library(gmock IMPORTED STATIC IMPORTED)
add_library(gtest IMPORTED STATIC IMPORTED)

set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION
    "${BINARY_DIR}/googlemock/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
  INTERFACE_INCLUDE_DIRECTORIES
    "${SOURCE_DIR}/googletest/include"
  IMPORTED_LINK_INTERFACE_LIBRARIES
    "${CMAKE_THREAD_LIBS_INIT}"
)

set_target_properties(gmock PROPERTIES
  IMPORTED_LOCATION
    "${BINARY_DIR}/googlemock/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_STATIC_LIBRARY_SUFFIX}"
  INTERFACE_INCLUDE_DIRECTORIES
    "${SOURCE_DIR}/googlemock/include"
  IMPORTED_LINK_INTERFACE_LIBRARIES
    gtest
)

add_dependencies(gmock googlemock)
