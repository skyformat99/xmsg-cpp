INCLUDE(ExternalProject)
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

ExternalProject_Add(
  googlemock
  URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
  URL_HASH SHA1=f9d9dd882a25f4069ed9ee48e70aff1b53e3c5a5
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
file(MAKE_DIRECTORY "${SOURCE_DIR}/include")
file(MAKE_DIRECTORY "${SOURCE_DIR}/gtest/include")

add_library(gmock IMPORTED STATIC IMPORTED)
add_library(gtest IMPORTED STATIC IMPORTED)

set_target_properties(gtest PROPERTIES
  IMPORTED_LOCATION
    "${BINARY_DIR}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
  INTERFACE_INCLUDE_DIRECTORIES
    "${SOURCE_DIR}/gtest/include"
  IMPORTED_LINK_INTERFACE_LIBRARIES
    "${CMAKE_THREAD_LIBS_INIT}"
)

set_target_properties(gmock PROPERTIES
  IMPORTED_LOCATION
    "${BINARY_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock${CMAKE_STATIC_LIBRARY_SUFFIX}"
  INTERFACE_INCLUDE_DIRECTORIES
    "${SOURCE_DIR}/include"
  IMPORTED_LINK_INTERFACE_LIBRARIES
    gtest
)

add_dependencies(gmock googlemock)
