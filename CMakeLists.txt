cmake_minimum_required(VERSION 3.25)
project(graph)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
option(ENABLE_FULL_ERRORS OFF)
if(ENABLE_FULL_ERRORS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

add_executable(graph main.cpp 
  ui/Console.cpp
  controller/CommandController.cpp 
  service/GraphService.cpp
  errors/InvalidInputError.cpp
)
