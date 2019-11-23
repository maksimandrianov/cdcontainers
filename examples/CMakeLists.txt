cmake_minimum_required(VERSION 2.8)

project(examples)

link_directories(${LIBRARY_OUTPUT_PATH})
include_directories(${PROJECT_INCLUDE_DIR})

add_executable(array array.c)
target_link_libraries(array ${LIBRARY_NAME})
