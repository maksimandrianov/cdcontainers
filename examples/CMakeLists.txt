cmake_minimum_required(VERSION 2.8)

project(examples)

set(SOURCE
  main.c)

link_directories(${LIBRARY_OUTPUT_PATH})
include_directories(${PROJECT_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME})

