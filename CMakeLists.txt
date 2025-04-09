cmake_minimum_required(VERSION 3.25)
project(graph)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

add_executable(graph main.cpp)
