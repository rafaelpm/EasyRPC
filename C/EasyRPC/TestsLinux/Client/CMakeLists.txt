cmake_minimum_required (VERSION 3.8)

add_definitions( -DLINUX )

set(CMAKE_BUILD_TYPE Debug)

include_directories("/home/rafael/Documents/GitHub/EasyRPC/C/EasyRPC/EasyRPC")
include_directories("/home/rafael/Documents/GitHub/EmbeddedCleanArch/libstream/libstream")
include_directories("/home/rafael/Documents/GitHub/EmbeddedCleanArch/libdebug/libdebug")

add_executable (test_client "test_client.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET test_client PROPERTY CXX_STANDARD 20)
endif()
