cmake_minimum_required(VERSION 2.8)

project(${LIBRARY_NAME})

set(SOURCE
        vector.c
        list.c
        stackl.c
        stackv.c
        queuel.c
        status.c
        common.c
)


include_directories("${PROJECT_INCLUDE_DIR}")

add_library(${PROJECT_NAME} SHARED ${SOURCE})
