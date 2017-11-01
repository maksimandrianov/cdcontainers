cmake_minimum_required(VERSION 2.8)

project(cdstructuresl)

include_directories("${TOP_DIR}/include/")

set(SOURCE_LIB
        "vector.c"
        "status.c"
)

add_library(cdstructuresl SHARED ${SOURCE_LIB})
