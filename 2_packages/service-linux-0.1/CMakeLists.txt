cmake_minimum_required(VERSION 2.8)
project(service_linux)
find_package (Threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_FILES main.cpp)
add_executable(service_linux ${SOURCE_FILES})
target_link_libraries (service_linux ${CMAKE_THREAD_LIBS_INIT})
