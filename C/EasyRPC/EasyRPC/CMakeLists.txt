cmake_minimum_required(VERSION 3.7)

project( easy_rpc VERSION 1.0)

add_executable( easy_rpc tests.cpp )

include_directories(${CMAKE_SOURCE_DIR} /home/rafael/Documents/GitHub/EmbeddedCleanArch/libstream/libstream)


#target_include_directories( easy_rpc PUBLIC "${KERNEL_HEADERS}" "${OTHER_KERNEL1}")

add_definitions(-std=c++0x -L/usr/lib -lpthread)

