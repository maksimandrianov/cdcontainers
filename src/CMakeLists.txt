cmake_minimum_required(VERSION 2.8)

project(${LIBRARY_NAME})

set(LIB_VERSION_MAJOR 0)
set(LIB_VERSION_MINOR 1)
set(LIB_VERSION_PATCH 0)
set(LIB_FULL_VERSION ${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${LIB_VERSION_PATCH})

set(SOURCE
        vector.c
        list.c
        deque.c
        heap.c
        stack.c
        queue.c
        priority-queue.c
        map.c
        treap.c
        binomial-heap.c
        pairing-heap.c
        hash-table.c
        splay-tree.c
        avl-tree.c
        status.c
        common.c
        data-info.c
        tables/pqueue-heap.c
        tables/pqueue-binomial-heap.c
        tables/pqueue-pairing-heap.c
        tables/seq-vector.c
        tables/seq-list.c
        tables/seq-deque.c
        tables/map-avl-tree.c
        tables/map-splay-tree.c
        tables/map-treap.c
)

include_directories("${PROJECT_INCLUDE_DIR}")
file(GLOB_RECURSE HEADERS "${PROJECT_INCLUDE_DIR}/*.h")

add_library(${PROJECT_NAME} SHARED ${SOURCE})

set_target_properties(${LIBRARY_NAME} PROPERTIES
        VERSION ${LIB_FULL_VERSION}
        SOVERSION ${LIB_VERSION_MAJOR}
)


install (TARGETS ${PROJECT_NAME} DESTINATION lib)
install (FILES ${HEADERS} DESTINATION include/${LIBRARY_NAME})
