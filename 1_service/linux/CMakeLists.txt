cmake_minimum_required(VERSION 3.2)
project(service_linux)
find_package (Threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(service_linux ${SOURCE_FILES})
target_link_libraries (service_linux ${CMAKE_THREAD_LIBS_INIT})