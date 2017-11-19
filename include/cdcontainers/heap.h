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
  * @brief The cdc_heap is a struct and functions that provide a heap
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>
#include <cdcontainers/vector.h>

/**
 * @brief The cdc_heap struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_heap {
        struct cdc_vector *vector;
        cdc_compar_func_t compar;
        struct cdc_data_info *dinfo;
};

/**
 * @brief Constructs an empty heap.
 * The function compar specifies the ordering of items.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_heap_ctor(struct cdc_heap **h, struct cdc_data_info *info,
                            cdc_compar_func_t compar);

/**
 * @brief Constructs a heap, initialized by an arbitrary number of pointers.
 * The function compar specifies the ordering of items.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_heap_ctorl(struct cdc_heap **h, struct cdc_data_info *info,
                             cdc_compar_func_t compar, ...);

/**
 * @brief Constructs a heap, initialized by args.
 * The function compar specifies the ordering of items.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_heap_ctorv(struct cdc_heap **h, struct cdc_data_info *info,
                             cdc_compar_func_t compar, va_list args);

/**
 * @brief Destroys the heap.
 */
void cdc_heap_dtor(struct cdc_heap *h);

// Element access
/**
 * @brief Returns a pointer to the heap's top item. This function assumes
 * that the heap isn't empty.
 */
static inline void *cdc_heap_top(struct cdc_heap *h)
{
        assert(h != NULL);

        return cdc_vector_front(h->vector);
}

// Capacity
/**
 * @brief Returns the number of items in the heap.
 */
static inline size_t cdc_heap_size(struct cdc_heap *h)
{
        assert(h != NULL);

        return cdc_vector_size(h->vector);
}

/**
 * @brief Returns true if the heap has size 0; otherwise returns false.
 */
static inline bool cdc_heap_empty(struct cdc_heap *h)
{
        assert(h != NULL);

        return cdc_vector_size(h->vector) == 0;
}


// Modifiers
/**
 * @brief Extracts the top item from the heap.
 * This function assumes that the heap isn't empty. Returned CDC_STATUS_OK in
 * a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_heap_extract_top(struct cdc_heap *h);

/**
 * @brief Inserts element key to the heap. Returned CDC_STATUS_OK in a
 * successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_heap_insert(struct cdc_heap *h, void *key);

/**
 * @brief Increases the item key on the index position in the heap.
 */
void cdc_heap_increase_key(struct cdc_heap *h, size_t i, void *key);

/**
 * @brief Swaps heaps a and b. This operation is very fast and never fails.
 */
void cdc_heap_swap(struct cdc_heap *a, struct cdc_heap *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_heap heap_t;

#define heap_ctor(...)         cdc_heap_ctor(__VA_ARGS__)
#define heap_ctorl(...)        cdc_heap_ctorl(__VA_ARGS__)
#define heap_ctorv(...)        cdc_heap_ctorv(__VA_ARGS__)
#define heap_dtor(...)         cdc_heap_dtor(__VA_ARGS__)

// Element access
#define heap_top(...)          cdc_heap_top(__VA_ARGS__)

// Capacity
#define heap_empty(...)        cdc_heap_empty(__VA_ARGS__)
#define heap_size(...)         cdc_heap_size(__VA_ARGS__)

// Modifiers
#define heap_extract_top(...)  cdc_heap_extract_top(__VA_ARGS__)
#define heap_insert(...)       cdc_heap_insert(__VA_ARGS__)
#define heap_increase_key(...) cdc_heap_increase_key(__VA_ARGS__)
#define heap_swap(...)         cdc_heap_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H
