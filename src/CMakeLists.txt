cmake_minimum_required(VERSION 2.8)

project(${LIBRARY_NAME})

set(LIB_VERSION_MAJOR 0)
set(LIB_VERSION_MINOR 1)
set(LIB_VERSION_PATCH 0)
set(LIB_FULL_VERSION ${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH})

set(SOURCE
  adapters/deque.c
  adapters/map.c
  adapters/priority-queue.c
  adapters/queue.c
  adapters/stack.c
  array.c
  avl-tree.c
  binomial-heap.c
  circular-array.c
  common.c
  data-info.c
  hash-table.c
  heap.c
  list.c
  pairing-heap.c
  splay-tree.c
  status.c
  tables/map-avl-tree.c
  tables/map-hash-table.c
  tables/map-splay-tree.c
  tables/map-treap.c
  tables/pqueue-binomial-heap.c
  tables/pqueue-heap.c
  tables/pqueue-pairing-heap.c
  tables/seq-circular-array.c
  tables/seq-list.c
  tables/seq-vector.c
  treap.c
)

include_directories("${PROJECT_INCLUDE_DIR}")

add_library(${PROJECT_NAME} SHARED ${SOURCE})

set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${LIB_FULL_VERSION}
  SOVERSION ${LIB_VERSION_MAJOR})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY ${PROJECT_INCLUDE_DIR} DESTINATION .)
