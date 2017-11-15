cmake_minimum_required(VERSION 2.8)

project(tests)

find_path(CUNIT_INCLUDE_DIR NAMES CUnit/CUnit.h)
mark_as_advanced(CUNIT_INCLUDE_DIR)

find_library(CUNIT_LIBRARY NAMES
        cunit
        libcunit
        cunitlib
)
mark_as_advanced(CUNIT_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CUnit DEFAULT_MSG CUNIT_LIBRARY CUNIT_INCLUDE_DIR)

if(CUNIT_FOUND)
        set(CUNIT_LIBRARIES ${CUNIT_LIBRARY})
        set(CUNIT_INCLUDE_DIRS ${CUNIT_INCLUDE_DIR})
endif(CUNIT_FOUND)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

set(SOURCE
      test-common.h
      test-vector.c
      test-list.c
      test-stackl.c
      test-stackv.c
      test-queuel.c
      test-queued.c
      test-deque.c
      test-main.c
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} cunit ${LIBRARY_NAME})

add_custom_target(check ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME} ${PROJECT_NAME})
