// The MIT License (MIT)
// Copyright (c) 2018 Maksim Andrianov
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
  * @brief The cdc_binomial_heap is a struct and functions that provide a
  * binomial heap
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_BINOMIAL_HEAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_BINOMIAL_HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>
#include <cdcontainers/vector.h>

/**
* @brief The cdc_binomial_heap_node struct
* @warning To avoid problems, do not change the structure fields in the code.
* Use only special functions to access and change structure fields.
*/
struct cdc_binomial_heap_node {
        struct cdc_binomial_heap_node *parent;
        struct cdc_binomial_heap_node *child;
        struct cdc_binomial_heap_node *sibling;
        size_t degree;
        void *key;
};

/**
 * @brief The cdc_binomial_heap struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_binomial_heap {
        struct cdc_binomial_heap_node *root;
        struct cdc_binomial_heap_node *top;
        size_t size;
        cdc_compar_fn_t compar;
        struct cdc_data_info *dinfo;
};

/**
 * @brief The cdc_binomial_heap_iter struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_binomial_heap_iter
{
        struct cdc_binomial_heap *container;
        struct cdc_binomial_heap_node *current;
};

/**
 * @brief Constructs an empty binomial heap.
 * The function compar specifies the ordering of items.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_binomial_heap_ctor(struct cdc_binomial_heap **h,
                                     struct cdc_data_info *info,
                                     cdc_compar_fn_t compar);

/**
 * @brief Constructs a binomial heap, initialized by an arbitrary number of pointers.
 * The function compar specifies the ordering of items.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_binomial_heap_ctorl(struct cdc_binomial_heap **h,
                                      struct cdc_data_info *info,
                                      cdc_compar_fn_t compar, ...);

/**
 * @brief Constructs a binomial heap, initialized by args.
 * The function compar specifies the ordering of items.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_binomial_heap_ctorv(struct cdc_binomial_heap **h,
                                      struct cdc_data_info *info,
                                      cdc_compar_fn_t compar, va_list args);

/**
 * @brief Destroys the binomial heap.
 */
void cdc_binomial_heap_dtor(struct cdc_binomial_heap *h);

// Element access
/**
 * @brief Returns a pointer to the binomial heap's top item. This function
 * assumes that the binomial heap isn't empty.
 */
static inline void *cdc_binomial_heap_top(struct cdc_binomial_heap *h)
{
        assert(h != NULL);

        return h->top->key;
}

// Capacity
/**
 * @brief Returns the number of items in the binomial heap.
 */
static inline size_t cdc_binomial_heap_size(struct cdc_binomial_heap *h)
{
        assert(h != NULL);

        return h->size;
}

/**
 * @brief Returns true if the binomial heap has size 0; otherwise returns false.
 */
static inline bool cdc_binomial_heap_empty(struct cdc_binomial_heap *h)
{
        assert(h != NULL);

        return h->size == 0;
}

// Modifiers
/**
 * @brief Extracts the top item from the binomial heap.
 * This function assumes that the binomial heap isn't empty. Returned
 * CDC_STATUS_OK in a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_binomial_heap_extract_top(struct cdc_binomial_heap *h);

/**
 * @brief Inserts element key to the binomial heap. Returned CDC_STATUS_OK in a
 * successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_binomial_heap_riinsert(struct cdc_binomial_heap *h, void *key,
                                         struct cdc_binomial_heap_iter *ret);

static inline enum cdc_stat cdc_binomial_heap_insert(struct cdc_binomial_heap *h,
                                                     void *key)
{
        assert(h != NULL);

        return cdc_binomial_heap_riinsert(h, key, NULL);
}

/**
 * @brief Increases the item key on the index position in the binomial heap.
 */
void cdc_binomial_heap_change_key(struct cdc_binomial_heap *h,
                                  struct cdc_binomial_heap_iter *pos, void *key);

/**
 * @brief Removes all the elements from the binomial heap. If a function has been
 * installed to delete an item, it will be called for each item.
 */
void cdc_binomial_heap_clear(struct cdc_binomial_heap *h);

/**
 * @brief Swaps binomial heaps a and b. This operation is very fast
 * and never fails.
 */
void cdc_binomial_heap_swap(struct cdc_binomial_heap *a,
                            struct cdc_binomial_heap *b);

// Operations
void cdc_binomial_heap_merge(struct cdc_binomial_heap *h,
                             struct cdc_binomial_heap *other);

bool cdc_binomial_heap_is_heap(struct cdc_binomial_heap *h);

// Iterators
/**
 * @brief Returns a pointer to the current item.
 */
static inline void *cdc_binomial_heap_iter_data(struct cdc_binomial_heap_iter it)
{
        return it.current->key;
}

/**
 * @brief Returns false if the iterator it1 equal to the iterator it2,
 * otherwise returns false
 */
static inline bool cdc_binomial_heap_iter_is_eq(struct cdc_binomial_heap_iter it1,
                                                struct cdc_binomial_heap_iter it2)
{
        return it1.container == it2.container && it1.current == it2.current;
}

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_binomial_heap binomial_heap_t;
typedef struct cdc_binomial_heap_iter binomial_heap_iter;

#define binomial_heap_ctor(...)         cdc_binomial_heap_ctor(__VA_ARGS__)
#define binomial_heap_ctorl(...)        cdc_binomial_heap_ctorl(__VA_ARGS__)
#define binomial_heap_ctorv(...)        cdc_binomial_heap_ctorv(__VA_ARGS__)
#define binomial_heap_dtor(...)         cdc_binomial_heap_dtor(__VA_ARGS__)

// Element access
#define binomial_heap_top(...)          cdc_binomial_heap_top(__VA_ARGS__)

// Capacity
#define binomial_heap_empty(...)        cdc_binomial_heap_empty(__VA_ARGS__)
#define binomial_heap_size(...)         cdc_binomial_heap_size(__VA_ARGS__)

// Modifiers
#define binomial_heap_extract_top(...)  cdc_binomial_heap_extract_top(__VA_ARGS__)
#define binomial_heap_riinsert(...)     cdc_binomial_heap_riinsert(__VA_ARGS__)
#define binomial_heap_insert(...)       cdc_binomial_heap_insert(__VA_ARGS__)
#define binomial_heap_change_key(...)   cdc_binomial_heap_change_key(__VA_ARGS__)
#define binomial_heap_clear(...)        cdc_binomial_heap_clear(__VA_ARGS__)
#define binomial_heap_swap(...)         cdc_binomial_heap_swap(__VA_ARGS__)

// Operations
#define binomial_heap_merge(...)        cdc_binomial_heap_merge(__VA_ARGS__)

#define binomial_heap_is_heap(...)      cdc_binomial_heap_is_heap(__VA_ARGS__)

// Iterators
#define binomial_heap_iter_data(...)    cdc_binomial_heap_iter_data(__VA_ARGS__)
#define binomial_heap_iter_is_eq(...)   cdc_binomial_heap_iter_is_eq(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_BINOMIAL_HEAP_H
