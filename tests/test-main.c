// The MIT License (MIT)
// Copyright (c) 2017 Maksim Andrianov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
#include "cdcontainers/global.h"
#include "test-common.h"

#include <CUnit/Basic.h>

int main(int argc, char** argv)
{
  CDC_UNUSED(argc);
  CDC_UNUSED(argv);

  CU_pSuite p_suite = NULL;

  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }

  p_suite = CU_add_suite("COMMON TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "ptr_float_cast", test_ptr_float_cast) == NULL ||
      CU_add_test(p_suite, "ptr_double_cast", test_ptr_double_cast) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("ARRAY TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_array_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_array_ctorl) == NULL ||
      CU_add_test(p_suite, "test_reserve", test_array_reserve) == NULL ||
      CU_add_test(p_suite, "test_data", test_array_data) == NULL ||
      CU_add_test(p_suite, "test_get", test_array_get) == NULL ||
      CU_add_test(p_suite, "test_at", test_array_at) == NULL ||
      CU_add_test(p_suite, "test_set", test_array_set) == NULL ||
      CU_add_test(p_suite, "test_front", test_array_front) == NULL ||
      CU_add_test(p_suite, "test_back", test_array_back) == NULL ||
      CU_add_test(p_suite, "test_insert", test_array_insert) == NULL ||
      CU_add_test(p_suite, "test_erase", test_array_erase) == NULL ||
      CU_add_test(p_suite, "test_clear", test_array_clear) == NULL ||
      CU_add_test(p_suite, "test_push_back", test_array_push_back) == NULL ||
      CU_add_test(p_suite, "test_pop_back", test_array_pop_back) == NULL ||
      CU_add_test(p_suite, "test_swap", test_array_swap) == NULL ||
      CU_add_test(p_suite, "test_shrink_to_fit", test_array_shrink_to_fit) ==
          NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("LIST TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_list_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_list_ctorl) == NULL ||
      CU_add_test(p_suite, "test_push_back", test_list_push_back) == NULL ||
      CU_add_test(p_suite, "test_push_front", test_list_push_front) == NULL ||
      CU_add_test(p_suite, "test_at", test_list_at) == NULL ||
      CU_add_test(p_suite, "test_front", test_list_front) == NULL ||
      CU_add_test(p_suite, "test_back", test_list_back) == NULL ||
      CU_add_test(p_suite, "test_pop_back", test_list_pop_back) == NULL ||
      CU_add_test(p_suite, "test_pop_front", test_list_pop_front) == NULL ||
      CU_add_test(p_suite, "test_swap", test_list_swap) == NULL ||
      CU_add_test(p_suite, "test_insert", test_list_insert) == NULL ||
      CU_add_test(p_suite, "test_erase", test_list_erase) == NULL ||
      CU_add_test(p_suite, "test_ierase", test_list_ierase) == NULL ||
      CU_add_test(p_suite, "test_clear", test_list_clear) == NULL ||
      CU_add_test(p_suite, "test_iterators", test_list_iterators) == NULL ||
      CU_add_test(p_suite, "test_reverse_iterators",
                  test_list_reverse_iterators) == NULL ||
      CU_add_test(p_suite, "test_splice", test_list_splice) == NULL ||
      CU_add_test(p_suite, "test_ssplice", test_list_ssplice) == NULL ||
      CU_add_test(p_suite, "test_lsplice", test_list_lsplice) == NULL ||
      CU_add_test(p_suite, "test_merge", test_list_merge) == NULL ||
      CU_add_test(p_suite, "test_erase_if", test_list_erase_if) == NULL ||
      CU_add_test(p_suite, "test_reverse", test_list_reverse) == NULL ||
      CU_add_test(p_suite, "test_unique", test_list_unique) == NULL ||
      CU_add_test(p_suite, "test_sort", test_list_sort) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("STACKV TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_stackv_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_stackv_ctorl) == NULL ||
      CU_add_test(p_suite, "test_push", test_stackv_push) == NULL ||
      CU_add_test(p_suite, "test_pop", test_stackv_pop) == NULL ||
      CU_add_test(p_suite, "test_swap", test_stackv_swap) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("QUEUED TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_queued_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_queued_ctorl) == NULL ||
      CU_add_test(p_suite, "test_push", test_queued_push) == NULL ||
      CU_add_test(p_suite, "test_pop", test_queued_pop) == NULL ||
      CU_add_test(p_suite, "test_front", test_queued_front) == NULL ||
      CU_add_test(p_suite, "test_back", test_queued_back) == NULL ||
      CU_add_test(p_suite, "test_swap", test_queued_swap) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("CIRCULAR ARRAY TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_circular_array_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_circular_array_ctorl) == NULL ||
      CU_add_test(p_suite, "test_push_back", test_circular_array_push_back) ==
          NULL ||
      CU_add_test(p_suite, "test_push_front", test_circular_array_push_front) ==
          NULL ||
      CU_add_test(p_suite, "test_insert", test_circular_array_insert) == NULL ||
      CU_add_test(p_suite, "test_get", test_circular_array_get) == NULL ||
      CU_add_test(p_suite, "test_at", test_circular_array_at) == NULL ||
      CU_add_test(p_suite, "test_set", test_circular_array_set) == NULL ||
      CU_add_test(p_suite, "test_front", test_circular_array_front) == NULL ||
      CU_add_test(p_suite, "test_back", test_circular_array_back) == NULL ||
      CU_add_test(p_suite, "test_pop_back", test_circular_array_pop_back) ==
          NULL ||
      CU_add_test(p_suite, "test_pop_front", test_circular_array_pop_front) ==
          NULL ||
      CU_add_test(p_suite, "test_swap", test_circular_array_swap) == NULL ||
      CU_add_test(p_suite, "test_erase", test_circular_array_erase) == NULL ||
      CU_add_test(p_suite, "test_clear", test_circular_array_clear) == NULL ||
      CU_add_test(p_suite, "test_pop_push", test_circular_array_pop_push) ==
          NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("HEAP TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_heap_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_heap_ctorl) == NULL ||
      CU_add_test(p_suite, "test_top", test_heap_top) == NULL ||
      CU_add_test(p_suite, "test_extract_top", test_heap_extract_top) == NULL ||
      CU_add_test(p_suite, "test_insert", test_heap_insert) == NULL ||
      CU_add_test(p_suite, "test_change_key", test_heap_change_key) == NULL ||
      CU_add_test(p_suite, "test_merge", test_heap_merge) == NULL ||
      CU_add_test(p_suite, "test_swap", test_heap_swap) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("BINOMIAL HEAP TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_binomial_heap_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_binomial_heap_ctorl) == NULL ||
      CU_add_test(p_suite, "test_dtor", test_binomial_heap_dtor) == NULL ||
      CU_add_test(p_suite, "test_top", test_binomial_heap_top) == NULL ||
      CU_add_test(p_suite, "test_extract_top",
                  test_binomial_heap_extract_top) == NULL ||
      CU_add_test(p_suite, "test_insert", test_binomial_heap_insert) == NULL ||
      CU_add_test(p_suite, "test_change_key", test_binomial_heap_change_key) ==
          NULL ||
      CU_add_test(p_suite, "test_merge", test_binomial_heap_merge) == NULL ||
      CU_add_test(p_suite, "test_swap", test_binomial_heap_swap) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("PAIRING HEAP TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_pairing_heap_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_pairing_heap_ctorl) == NULL ||
      CU_add_test(p_suite, "test_dtor", test_pairing_heap_dtor) == NULL ||
      CU_add_test(p_suite, "test_top", test_pairing_heap_top) == NULL ||
      CU_add_test(p_suite, "test_extract_top", test_pairing_heap_extract_top) ==
          NULL ||
      CU_add_test(p_suite, "test_insert", test_pairing_heap_insert) == NULL ||
      CU_add_test(p_suite, "test_change_key", test_pairing_heap_change_key) ==
          NULL ||
      CU_add_test(p_suite, "test_merge", test_pairing_heap_merge) == NULL ||
      CU_add_test(p_suite, "test_swap", test_pairing_heap_swap) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("PRIORITY QUEUE TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_priority_queue_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_priority_queue_ctorl) == NULL ||
      CU_add_test(p_suite, "test_push", test_priority_queue_push) == NULL ||
      CU_add_test(p_suite, "test_pop", test_priority_queue_pop) == NULL ||
      CU_add_test(p_suite, "test_top", test_priority_queue_top) == NULL ||
      CU_add_test(p_suite, "test_swap", test_priority_queue_swap) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("TREAP TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_treap_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_treap_ctorl) == NULL ||
      CU_add_test(p_suite, "test_insert", test_treap_insert) == NULL ||
      CU_add_test(p_suite, "test_swap", test_treap_swap) == NULL ||
      CU_add_test(p_suite, "test_get", test_treap_get) == NULL ||
      CU_add_test(p_suite, "test_count", test_treap_count) == NULL ||
      CU_add_test(p_suite, "test_find", test_treap_find) == NULL ||
      CU_add_test(p_suite, "test_equal_range", test_treap_equal_range) ==
          NULL ||
      CU_add_test(p_suite, "test_clear", test_treap_clear) == NULL ||
      CU_add_test(p_suite, "test_insert_or_assign",
                  test_treap_insert_or_assign) == NULL ||
      CU_add_test(p_suite, "test_erase", test_treap_erase) == NULL ||
      CU_add_test(p_suite, "test_iterators", test_treap_iterators) == NULL ||
      CU_add_test(p_suite, "test_height", test_treap_height) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("HASH TABLE TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_hash_table_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_hash_table_ctorl) == NULL ||
      CU_add_test(p_suite, "test_get", test_hash_table_get) == NULL ||
      CU_add_test(p_suite, "test_count", test_hash_table_count) == NULL ||
      CU_add_test(p_suite, "test_find", test_hash_table_find) == NULL ||
      CU_add_test(p_suite, "test_equal_range", test_hash_table_equal_range) ==
          NULL ||
      CU_add_test(p_suite, "test_clear", test_hash_table_clear) == NULL ||
      CU_add_test(p_suite, "test_insert", test_hash_table_insert) == NULL ||
      CU_add_test(p_suite, "test_insert_or_assign",
                  test_hash_table_insert_or_assign) == NULL ||
      CU_add_test(p_suite, "test_erase", test_hash_table_erase) == NULL ||
      CU_add_test(p_suite, "test_swap", test_hash_table_swap) == NULL ||
      CU_add_test(p_suite, "test_rehash", test_hash_table_rehash) == NULL ||
      CU_add_test(p_suite, "test_reserve", test_hash_table_reserve) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("SPLAY TREE TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_splay_tree_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_splay_tree_ctorl) == NULL ||
      CU_add_test(p_suite, "test_insert", test_splay_tree_insert) == NULL ||
      CU_add_test(p_suite, "test_swap", test_splay_tree_swap) == NULL ||
      CU_add_test(p_suite, "test_get", test_splay_tree_get) == NULL ||
      CU_add_test(p_suite, "test_count", test_splay_tree_count) == NULL ||
      CU_add_test(p_suite, "test_find", test_splay_tree_find) == NULL ||
      CU_add_test(p_suite, "test_equal_range", test_splay_tree_equal_range) ==
          NULL ||
      CU_add_test(p_suite, "test_clear", test_splay_tree_clear) == NULL ||
      CU_add_test(p_suite, "test_insert_or_assign",
                  test_splay_tree_insert_or_assign) == NULL ||
      CU_add_test(p_suite, "test_erase", test_splay_tree_erase) == NULL ||
      CU_add_test(p_suite, "test_iterators", test_splay_tree_iterators) ==
          NULL ||
      CU_add_test(p_suite, "test_height", test_splay_tree_height) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("AVL TREE TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_avl_tree_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_avl_tree_ctorl) == NULL ||
      CU_add_test(p_suite, "test_insert", test_avl_tree_insert) == NULL ||
      CU_add_test(p_suite, "test_swap", test_avl_tree_swap) == NULL ||
      CU_add_test(p_suite, "test_get", test_avl_tree_get) == NULL ||
      CU_add_test(p_suite, "test_count", test_avl_tree_count) == NULL ||
      CU_add_test(p_suite, "test_find", test_avl_tree_find) == NULL ||
      CU_add_test(p_suite, "test_clear", test_avl_tree_clear) == NULL ||
      CU_add_test(p_suite, "test_insert_or_assign",
                  test_avl_tree_insert_or_assign) == NULL ||
      CU_add_test(p_suite, "test_erase", test_avl_tree_erase) == NULL ||
      CU_add_test(p_suite, "test_iterators", test_avl_tree_iterators) == NULL ||
      CU_add_test(p_suite, "test_height", test_avl_tree_height) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  p_suite = CU_add_suite("MAP TESTS", NULL, NULL);
  if (p_suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (CU_add_test(p_suite, "test_ctor", test_map_ctor) == NULL ||
      CU_add_test(p_suite, "test_ctorl", test_map_ctorl) == NULL ||
      CU_add_test(p_suite, "test_insert", test_map_insert) == NULL ||
      CU_add_test(p_suite, "test_swap", test_map_swap) == NULL ||
      CU_add_test(p_suite, "test_get", test_map_get) == NULL ||
      CU_add_test(p_suite, "test_count", test_map_count) == NULL ||
      CU_add_test(p_suite, "test_find", test_map_find) == NULL ||
      CU_add_test(p_suite, "test_clear", test_map_clear) == NULL ||
      CU_add_test(p_suite, "test_insert_or_assign",
                  test_map_insert_or_assign) == NULL ||
      CU_add_test(p_suite, "test_erase", test_map_erase) == NULL ||
      CU_add_test(p_suite, "test_iterators", test_map_iterators) == NULL ||
      CU_add_test(p_suite, "test_iter_type", test_map_iter_type) == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();

  return CU_get_error();
}
