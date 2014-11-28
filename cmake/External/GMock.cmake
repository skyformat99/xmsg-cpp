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
SET(GMOCK_INCLUDE_DIRS "${SOURCE_DIR}/include" "${SOURCE_DIR}/gtest/include")
SET(GMOCK_LIBRARIES
  "${BINARY_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}gmock.a"
  "${BINARY_DIR}/gtest/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.a"
  ${CMAKE_THREAD_LIBS_INIT}
)
