project(examples)

link_directories(${LIBRARY_OUTPUT_PATH})
include_directories(${PROJECT_INCLUDE_DIR})

set(LIBRARY_NAME cdcontainers)

add_executable(array array.c)
target_link_libraries(array ${LIBRARY_NAME})

add_executable(map map.c)
target_link_libraries(map ${LIBRARY_NAME})
