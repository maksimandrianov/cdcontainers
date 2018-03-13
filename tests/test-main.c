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
#include <CUnit/Basic.h>
#include "test-common.h"


int main(int argc, char** argv)
{
        CU_pSuite p_suite = NULL;

        if (CUE_SUCCESS != CU_initialize_registry())
                return CU_get_error();

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

        p_suite = CU_add_suite("VECTOR TESTS", NULL, NULL);
        if (p_suite == NULL) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        if (CU_add_test(p_suite, "test_ctor", test_vector_ctor) == NULL ||
            CU_add_test(p_suite, "test_ctorl", test_vector_ctorl) == NULL ||
            CU_add_test(p_suite, "test_reserve", test_vector_reserve) == NULL ||
            CU_add_test(p_suite, "test_data", test_vector_data) == NULL ||
            CU_add_test(p_suite, "test_get", test_vector_get) == NULL ||
            CU_add_test(p_suite, "test_at", test_vector_at) == NULL ||
            CU_add_test(p_suite, "test_set", test_vector_set) == NULL ||
            CU_add_test(p_suite, "test_front", test_vector_front) == NULL ||
            CU_add_test(p_suite, "test_back", test_vector_back) == NULL ||
            CU_add_test(p_suite, "test_insert", test_vector_insert) == NULL ||
            CU_add_test(p_suite, "test_erase", test_vector_erase) == NULL ||
            CU_add_test(p_suite, "test_clear", test_vector_clear) == NULL ||
            CU_add_test(p_suite, "test_push_back", test_vector_push_back) == NULL ||
            CU_add_test(p_suite, "test_pop_back", test_vector_pop_back) == NULL ||
            CU_add_test(p_suite, "test_swap", test_vector_swap) == NULL) {
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
            CU_add_test(p_suite, "test_remove", test_list_remove) == NULL ||
            CU_add_test(p_suite, "test_iremove", test_list_iremove) == NULL ||
            CU_add_test(p_suite, "test_erase", test_list_erase) == NULL ||
            CU_add_test(p_suite, "test_ierase", test_list_ierase) == NULL ||
            CU_add_test(p_suite, "test_clear", test_list_clear) == NULL ||
            CU_add_test(p_suite, "test_iterators", test_list_iterators) == NULL ||
            CU_add_test(p_suite, "test_reverse_iterators", test_list_reverse_iterators) == NULL ||
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

        p_suite = CU_add_suite("DEQUE TESTS", NULL, NULL);
        if (p_suite == NULL) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        if (CU_add_test(p_suite, "test_ctor", test_deque_ctor) == NULL ||
            CU_add_test(p_suite, "test_ctorl", test_deque_ctorl) == NULL ||
            CU_add_test(p_suite, "test_push_back", test_deque_push_back) == NULL ||
            CU_add_test(p_suite, "test_push_front", test_deque_push_front) == NULL ||
            CU_add_test(p_suite, "test_get", test_deque_get) == NULL ||
            CU_add_test(p_suite, "test_at", test_deque_at) == NULL ||
            CU_add_test(p_suite, "test_set", test_deque_set) == NULL ||
            CU_add_test(p_suite, "test_front", test_deque_front) == NULL ||
            CU_add_test(p_suite, "test_back", test_deque_back) == NULL ||
            CU_add_test(p_suite, "test_pop_back", test_deque_pop_back) == NULL ||
            CU_add_test(p_suite, "test_pop_front", test_deque_pop_front) == NULL ||
            CU_add_test(p_suite, "test_swap", test_deque_swap) == NULL ||
            CU_add_test(p_suite, "test_insert", test_deque_insert) == NULL ||
            CU_add_test(p_suite, "test_erase", test_deque_erase) == NULL ||
            CU_add_test(p_suite, "test_clear", test_deque_clear) == NULL ||
            CU_add_test(p_suite, "test_pop_push", test_deque_pop_push) == NULL) {
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
            CU_add_test(p_suite, "test_extract_top", test_binomial_heap_extract_top) == NULL ||
            CU_add_test(p_suite, "test_insert", test_binomial_heap_insert) == NULL ||
            CU_add_test(p_suite, "test_change_key", test_binomial_heap_change_key) == NULL ||
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
            CU_add_test(p_suite, "test_extract_top", test_pairing_heap_extract_top) == NULL ||
            CU_add_test(p_suite, "test_insert", test_pairing_heap_insert) == NULL ||
            CU_add_test(p_suite, "test_change_key", test_pairing_heap_change_key) == NULL ||
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
            CU_add_test(p_suite, "test_swap", test_treap_swap) == NULL) {
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
            CU_add_test(p_suite, "test_equal_range", test_hash_table_equal_range) == NULL ||
            CU_add_test(p_suite, "test_clear", test_hash_table_clear) == NULL ||
            CU_add_test(p_suite, "test_insert", test_hash_table_insert) == NULL ||
            CU_add_test(p_suite, "test_insert_or_assign", test_hash_table_insert_or_assign) == NULL ||
            CU_add_test(p_suite, "test_erase", test_hash_table_erase) == NULL ||
            CU_add_test(p_suite, "test_swap", test_hash_table_swap) == NULL ||
            CU_add_test(p_suite, "test_load_factor", test_hash_table_load_factor) == NULL ||
            CU_add_test(p_suite, "test_max_load_factor", test_hash_table_max_load_factor) == NULL ||
            CU_add_test(p_suite, "test_set_max_load_factor", test_hash_table_set_max_load_factor) == NULL ||
            CU_add_test(p_suite, "test_rehash", test_hash_table_rehash) == NULL ||
            CU_add_test(p_suite, "test_reserve", test_hash_table_reserve) == NULL) {
                CU_cleanup_registry();
                return CU_get_error();
        }

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();

        return CU_get_error();
}
