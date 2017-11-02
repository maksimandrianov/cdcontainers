cmake_minimum_required(VERSION 2.8)

project(cdstructures)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(TOP_LEVEL_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${TOP_LEVEL_DIR}/include)

add_subdirectory(cdstructures)
add_subdirectory(examples)
#enable_testing()
#add_subdirectory(tests)


