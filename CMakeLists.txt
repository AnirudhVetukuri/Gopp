cmake_minimum_required(VERSION 3.10)
project(gopp)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Directory: ${CMAKE_SOURCE_DIR}/src")
message(STATUS "Include Directory: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "Tests Directory: ${CMAKE_SOURCE_DIR}/tests")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(context STATIC src/Context.cpp)

add_executable(test_context tests/test_context.cpp)

target_link_libraries(test_context context)
