cmake_minimum_required(VERSION 2.8)

project(cdcontainers)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -g")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -g")

set(LIBRARY_NAME cdcontainers)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(TOP_LEVEL_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_INCLUDE_DIR ${TOP_LEVEL_DIR}/include)


add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)
set_target_properties(tests PROPERTIES EXCLUDE_FROM_ALL TRUE)

