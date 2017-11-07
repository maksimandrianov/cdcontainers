#ifndef CDCONTAINERS_TESTS_TESTS_COMMON_H
#define CDCONTAINERS_TESTS_TESTS_COMMON_H

#include <cdcontainers/cdstructures.h>

// Vector tests
void test_vector_ctor();
void test_vector_ctorl();
void test_vector_reserve();
void test_vector_data();
void test_vector_get();
void test_vector_at();
void test_vector_front();
void test_vector_back();
void test_vector_insert();
void test_vector_erase();
void test_vector_clear();
void test_vector_push_back();
void test_vector_pop_back();
void test_vector_swap();

// List tests
void test_list_ctor();
void test_list_ctorl();
void test_list_push_back();
void test_list_push_front();
void test_list_at();
void test_list_front();
void test_list_back();
void test_list_pop_back();
void test_list_pop_front();
void test_list_swap();
void test_list_insert();
void test_list_erase();
void test_list_clear();

// Stackl tests
void test_stackl_ctor();
void test_stackl_ctorl();
void test_stackl_push();
void test_stackl_pop();
void test_stackl_swap();

// Stackv tests
void test_stackv_ctor();
void test_stackv_ctorl();
void test_stackv_push();
void test_stackv_pop();
void test_stackv_swap();

// Queuel tests
void test_queuel_ctor();
void test_queuel_ctorl();
void test_queuel_push();
void test_queuel_pop();
void test_queuel_front();
void test_queuel_back();
void test_queuel_swap();

#endif  // CDSTRUCTURES_TESTS_TESTS_COMMON_H
