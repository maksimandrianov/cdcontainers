project(tests)

find_path(CUNIT_INCLUDE_DIR NAMES CUnit/CUnit.h)
mark_as_advanced(CUNIT_INCLUDE_DIR)

find_library(CUNIT_LIBRARY NAMES
  cunit
  libcunit
  cunitlib)
mark_as_advanced(CUNIT_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CUnit DEFAULT_MSG CUNIT_LIBRARY CUNIT_INCLUDE_DIR)

if(CUNIT_FOUND)
  set(CUNIT_LIBRARIES ${CUNIT_LIBRARY})
  set(CUNIT_INCLUDE_DIRS ${CUNIT_INCLUDE_DIR})
endif(CUNIT_FOUND)

include_directories(${PROJECT_INCLUDE_DIR} ${CUNIT_INCLUDE_DIR})

set(SOURCE
  test-array.c
  test-avl-tree.c
  test-binomial-heap.c
  test-common.c
  test-common.h
  test-circular-array.c
  test-deque.c
  test-hash-table.c
  test-heap.c
  test-list.c
  test-main.c
  test-map.c
  test-pairing-heap.c
  test-priority-queueh.c
  test-queue.c
  test-splay-tree.c
  test-stack.c
  test-treap.c
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} ${CUNIT_LIBRARIES} m cdcontainers)

add_custom_target(check ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME} ${PROJECT_NAME})
