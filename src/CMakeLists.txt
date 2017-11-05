cmake_minimum_required(VERSION 2.8)

project(cdstructures)

set(SOURCE
        vector.c
        list.c
        status.c
        common.c
        common.h
)


include_directories("${PROJECT_INCLUDE_DIR}")

add_library(${PROJECT_NAME} SHARED ${SOURCE})
