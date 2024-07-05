cmake_minimum_required(VERSION 3.10)
project(gopp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Directory: ${CMAKE_SOURCE_DIR}/src")
message(STATUS "Include Directory: ${CMAKE_SOURCE_DIR}/include")
message(STATUS "Tests Directory: ${CMAKE_SOURCE_DIR}/tests")

include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
add_library(context STATIC src/Context.cpp)
add_library(task STATIC src/Task.cpp)
add_library(scheduler STATIC src/Scheduler.cpp)
add_library(coroutine STATIC src/Coroutine.cpp)

# Link task with context
target_link_libraries(task context)
target_link_libraries(scheduler task)
target_link_libraries(coroutine context)

# Add test executables
add_executable(test_context tests/test_context.cpp)
add_executable(test_task tests/test_task.cpp)
add_executable(test_scheduler tests/test_scheduler.cpp)
add_executable(test_coroutine tests/test_coroutine.cpp)

# Link the context library to the test executables
target_link_libraries(test_context context)
target_link_libraries(test_task context task)
target_link_libraries(test_scheduler scheduler coroutine context task)
target_link_libraries(test_coroutine coroutine context)
