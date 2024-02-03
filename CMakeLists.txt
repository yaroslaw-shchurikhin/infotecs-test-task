cmake_minimum_required(VERSION 3.19)
project(testTask)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(PROGRAM1_TARGET program1)
set(PROGRAM2_TARGET program2)

add_subdirectory(program1)
add_subdirectory(program2)
