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
/**
 * @file
 * @author Maksim Andrianov <maksimandrianov1@yandex.ru>
 * @brief The cdc_heap is a struct and functions that provide a binary heap
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>
#include <cdcontainers/vector.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief The cdc_heap struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_heap {
  struct cdc_vector *vector;
};

/**
 * @brief The cdc_heap_iter struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_heap_iter {
  struct cdc_vector *container;
  size_t current;
};

/**
 * @brief Constructs an empty heap.
 * @param h - cdc_heap
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_heap_ctor(struct cdc_heap **h, struct cdc_data_info *info);

/**
 * @brief Constructs a heap, initialized by an arbitrary number of pointers. The
 * last item must be NULL.
 * @param h - cdc_heap
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_heap_ctorl(struct cdc_heap **h, struct cdc_data_info *info,
                             ...);

/**
 * @brief Constructs a heap, initialized by args. The last item must be NULL.
 * @param h - cdc_heap
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_heap_ctorv(struct cdc_heap **h, struct cdc_data_info *info,
                             va_list args);

/**
 * @brief Destroys the heap.
 * @param h - cdc_heap
 */
void cdc_heap_dtor(struct cdc_heap *h);

// Element access
/**
 * @brief Returns a pointer to the heap's top item. This function assumes that
 * the heap isn't empty.
 * @param h - cdc_heap
 * @return top item
 */
static inline void *cdc_heap_top(struct cdc_heap *h)
{
  assert(h != NULL);

  return cdc_vector_front(h->vector);
}

// Capacity
/**
 * @brief Returns the number of items in the heap.
 * @param h - cdc_heap
 * @return size
 */
static inline size_t cdc_heap_size(struct cdc_heap *h)
{
  assert(h != NULL);

  return cdc_vector_size(h->vector);
}

/**
 * @brief Returns true if the heap has size 0; otherwise returns false.
 * @param h - cdc_heap
 * @return true if the heap has size 0; otherwise returns false
 */
static inline bool cdc_heap_empty(struct cdc_heap *h)
{
  assert(h != NULL);

  return cdc_vector_empty(h->vector);
}

// Modifiers
/**
 * @brief Extracts the top item from the heap. This function assumes that the
 * heap isn't empty.
 * @param h - cdc_heap
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_heap_extract_top(struct cdc_heap *h);

/**
 * @brief Inserts element key to the heap. Write an iterator pointing to a new
 * element in the ret
 * @param h - cdc_heap
 * @param key
 * @param ret - pointer to iterator where an iterator will be written indicating
 * the inserted element
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_heap_riinsert(struct cdc_heap *h, void *key,
                                struct cdc_heap_iter *ret);

/**
 * @brief Inserts element key to the heap.
 * @param h - cdc_heap
 * @param key
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_heap_insert(struct cdc_heap *h, void *key)
{
  assert(h != NULL);

  return cdc_heap_riinsert(h, key, NULL);
}

/**
 * @brief Changes the item key on the pos position in the heap.
 * @param h - cdc_heap
 * @param pos - iterator that indicates the item with key that you want to
 * change
 * @param key
 */
void cdc_heap_change_key(struct cdc_heap *h, struct cdc_heap_iter *pos,
                         void *key);

/**
 * @brief Removes all the elements from the heap.
 * @param h - cdc_heap
 */
void cdc_heap_clear(struct cdc_heap *h);

/**
 * @brief Swaps heaps a and b. This operation is very fast and never fails.
 * @param a - cdc_heap
 * @param b - cdc_heap
 */
void cdc_heap_swap(struct cdc_heap *a, struct cdc_heap *b);

// Operations
/**
 * @brief Merges two heaps. In the heap h will be the result of the merger,
 * and the heap other will remain empty.
 * @param h - cdc_heap
 * @param other - other cdc_heap
 */
enum cdc_stat cdc_heap_merge(struct cdc_heap *h, struct cdc_heap *other);

/**
 * @brief Checks the heap property.
 * @param h - cdc_heap
 * @return result of the check
 */
bool cdc_heap_is_heap(struct cdc_heap *h);

// Iterators
/**
 * @brief Returns a pointer to the key of current item.
 */
static inline void *cdc_heap_iter_data(struct cdc_heap_iter *it)
{
  assert(it != NULL);

  return cdc_vector_get(it->container, it->current);
}

/**
 * @brief Returns false if the iterator it1 equal to the iterator it2,
 * otherwise returns false
 */
static inline bool cdc_heap_iter_is_eq(struct cdc_heap_iter *it1,
                                       struct cdc_heap_iter *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);

  return it1->container == it2->container && it1->current == it2->current;
}

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_heap heap_t;
typedef struct cdc_heap_iter heap_iter_t;

#define heap_ctor(...) cdc_heap_ctor(__VA_ARGS__)
#define heap_ctorl(...) cdc_heap_ctorl(__VA_ARGS__)
#define heap_ctorv(...) cdc_heap_ctorv(__VA_ARGS__)
#define heap_dtor(...) cdc_heap_dtor(__VA_ARGS__)

// Element access
#define heap_top(...) cdc_heap_top(__VA_ARGS__)

// Capacity
#define heap_empty(...) cdc_heap_empty(__VA_ARGS__)
#define heap_size(...) cdc_heap_size(__VA_ARGS__)

// Modifiers
#define heap_extract_top(...) cdc_heap_extract_top(__VA_ARGS__)
#define heap_riinsert(...) cdc_heap_riinsert(__VA_ARGS__)
#define heap_insert(...) cdc_heap_insert(__VA_ARGS__)
#define heap_change_key(...) cdc_heap_change_key(__VA_ARGS__)
#define heap_clear(...) cdc_heap_clear(__VA_ARGS__)
#define heap_swap(...) cdc_heap_swap(__VA_ARGS__)

// Operations
#define heap_merge(...) cdc_heap_merge(__VA_ARGS__)

#define heap_is_heap(...) cdc_heap_is_heap(__VA_ARGS__)

// Iterators
#define heap_iter_data(...) cdc_heap_iter_data(__VA_ARGS__)
#define heap_iter_is_eq(...) cdc_heap_iter_is_eq(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H
