// The MIT License (MIT)
// Copyright (c) 2019 Maksim Andrianov
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

/*! @mainpage cdcontainers - data containers and data structures for C
 *
 * Library of data containers and data structures for C programming language.
 * The cdcontainers interface is similar to C ++ STL. The library contains the
 * following data containers:
 *   - cdc_array - dynamic array. See array.h.
 *   - cdc_list - doubly linked list. See list.h.
 *   - cdc_circular_array - circular array. See circular_array.h.
 *   - cdc_heap - binary heap. See heap.h.
 *   - cdc_binomial_heap - binomial heap. See binomial-heap.h.
 *   - cdc_pairing_heap - pairing heap. See pairing-heap.h.
 *   - cdc_hash_table - hash table with collisions resolved by chaining. See
 * hash-table.h.
 *   - cdc_avl_tree - avl tree. See avl-tree.h.
 *   - cdc_splay_tree - splay tree. See splay-tree.h.
 *   - cdc_treep - сartesian tree. See treap.h.
 *
 * and following adapters:
 *   - cdc_stack (Can work with: cdc_array, cdc_list, cdc_circular_array). See
 * stack.h.
 *   - cdc_queue (Can work with: cdc_array, cdc_list, cdc_circular_array). See
 * queue.h.
 *   - cdc_proirity_queue (Can work with: cdc_heap, cdc_binomial_heap,
 * cdc_pairing_heap). See priority-queue.h.
 *   - cdc_map (Can work with: cdc_avl_tree, cdc_splay_tree, cdc_treep). See
 * map.h.
 *
 *  Example usage array:
 *  @include array.c
 *
 */

#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_CDC_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_CDC_H

#include <cdcontainers/adapters/map.h>
#include <cdcontainers/adapters/priority-queue.h>
#include <cdcontainers/adapters/queue.h>
#include <cdcontainers/adapters/stack.h>
#include <cdcontainers/array.h>
#include <cdcontainers/avl-tree.h>
#include <cdcontainers/binomial-heap.h>
#include <cdcontainers/casts.h>
#include <cdcontainers/circular-array.h>
#include <cdcontainers/common.h>
#include <cdcontainers/global.h>
#include <cdcontainers/hash-table.h>
#include <cdcontainers/hash.h>
#include <cdcontainers/heap.h>
#include <cdcontainers/list.h>
#include <cdcontainers/pairing-heap.h>
#include <cdcontainers/splay-tree.h>
#include <cdcontainers/status.h>
#include <cdcontainers/treap.h>

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_CDC_H
