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
#ifndef CDCONTAINERS_TESTS_TESTS_COMMON_H
#define CDCONTAINERS_TESTS_TESTS_COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

// Vector tests
void test_vector_ctor();
void test_vector_ctorl();
void test_vector_reserve();
void test_vector_data();
void test_vector_get();
void test_vector_at();
void test_vector_set();
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
void test_list_remove();
void test_list_iremove();
void test_list_erase();
void test_list_ierase();
void test_list_clear();
void test_list_iterators();
void test_list_reverse_iterators();
void test_list_splice();
void test_list_ssplice();
void test_list_lsplice();
void test_list_merge();
void test_list_remove_if();
void test_list_reverse();
void test_list_unique();
void test_list_sort();

// Stackv tests
void test_stackv_ctor();
void test_stackv_ctorl();
void test_stackv_push();
void test_stackv_pop();
void test_stackv_swap();

// Queued tests
void test_queued_ctor();
void test_queued_ctorl();
void test_queued_push();
void test_queued_pop();
void test_queued_front();
void test_queued_back();
void test_queued_swap();

// Deque tests
void test_deque_ctor();
void test_deque_ctorl();
void test_deque_push_back();
void test_deque_push_front();
void test_deque_get();
void test_deque_at();
void test_deque_set();
void test_deque_front();
void test_deque_back();
void test_deque_pop_back();
void test_deque_pop_front();
void test_deque_swap();
void test_deque_insert();
void test_deque_erase();
void test_deque_clear();
void test_deque_pop_push();

// Heap tests
void test_heap_ctor();
void test_heap_ctorl();
void test_heap_top();
void test_heap_extract_top();
void test_heap_insert();
void test_heap_change_key();
void test_heap_merge();
void test_heap_swap();

// Binomial heap tests
void test_binomial_heap_ctor();
void test_binomial_heap_ctorl();
void test_binomial_heap_dtor();
void test_binomial_heap_top();
void test_binomial_heap_extract_top();
void test_binomial_heap_insert();
void test_binomial_heap_change_key();
void test_binomial_heap_merge();
void test_binomial_heap_swap();

// Pairing heap tests
void test_pairing_heap_ctor();
void test_pairing_heap_ctorl();
void test_pairing_heap_dtor();
void test_pairing_heap_top();
void test_pairing_heap_extract_top();
void test_pairing_heap_insert();
void test_pairing_heap_change_key();
void test_pairing_heap_merge();
void test_pairing_heap_swap();

// Priority queue tests
void test_priority_queue_ctor();
void test_priority_queue_ctorl();
void test_priority_queue_push();
void test_priority_queue_pop();
void test_priority_queue_top();
void test_priority_queue_swap();

// Treap tests
void test_treap_ctor();
void test_treap_ctorl();
void test_treap_insert();
void test_treap_erase();
void test_treap_swap();

#endif  // CDSTRUCTURES_TESTS_TESTS_COMMON_H
