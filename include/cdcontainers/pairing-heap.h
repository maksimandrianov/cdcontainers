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
  * @brief The cdc_pairing_heap is a struct and functions that provide a
  * pairing heap
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_PAIRING_HEAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_PAIRING_HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

/**
* @brief The cdc_pairing_heap_node struct
* @warning To avoid problems, do not change the structure fields in the code.
* Use only special functions to access and change structure fields.
*/
struct cdc_pairing_heap_node {
        struct cdc_pairing_heap_node *parent;
        struct cdc_pairing_heap_node *child;
        struct cdc_pairing_heap_node *sibling;
        void *key;
};

/**
 * @brief The cdc_pairing_heap struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_pairing_heap {
        struct cdc_pairing_heap_node *root;
        size_t size;
        cdc_compar_fn_t compar;
        struct cdc_data_info *dinfo;
};

/**
 * @brief The cdc_pairing_heap_iter struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_pairing_heap_iter
{
        struct cdc_pairing_heap *container;
        struct cdc_pairing_heap_node *current;
};

/**
 * @brief Constructs an empty pairing heap.
 * @param h - cdc_pairing_heap
 * @param info - cdc_data_info
 * @param compar - function that specifies a strict ordering
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_pairing_heap_ctor(struct cdc_pairing_heap **h,
                                    struct cdc_data_info *info,
                                    cdc_compar_fn_t compar);

/**
 * @brief Constructs a pairing heap, initialized by an arbitrary number of
 * pointers. The last item must be NULL.
 * @param h - cdc_pairing_heap
 * @param info - cdc_data_info
 * @param compar - function that specifies a strict ordering
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_pairing_heap_ctorl(struct cdc_pairing_heap **h,
                                     struct cdc_data_info *info,
                                     cdc_compar_fn_t compar, ...);

/**
 * @brief Constructs a pairing heap, initialized by args. The last item must be
 * NULL.
 * @param h - cdc_pairing_heap
 * @param info - cdc_data_info
 * @param compar - function that specifies a strict ordering
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_pairing_heap_ctorv(struct cdc_pairing_heap **h,
                                     struct cdc_data_info *info,
                                     cdc_compar_fn_t compar, va_list args);

/**
 * @brief Destroys the pairing heap.
 * @param h - cdc_pairing_heap
 */
void cdc_pairing_heap_dtor(struct cdc_pairing_heap *h);

// Element access
/**
 * @brief Returns a pointer to the pairing heap's top item. This function
 * assumes that the pairing heap isn't empty.
 * @param h - cdc_pairing_heap
 * @return top item
 */
static inline void *cdc_pairing_heap_top(struct cdc_pairing_heap *h)
{
        assert(h != NULL);

        return h->root->key;
}

// Capacity
/**
 * @brief Returns the number of items in the pairing heap.
 * @param h - cdc_pairing_heap
 * @return size
 */
static inline size_t cdc_pairing_heap_size(struct cdc_pairing_heap *h)
{
        assert(h != NULL);

        return h->size;
}

/**
 * @brief Returns true if the pairing heap has size 0; otherwise returns false.
 * @param h - cdc_pairing_heap
 * @return true if the pairing heap has size 0; otherwise returns false
 */
static inline bool cdc_pairing_heap_empty(struct cdc_pairing_heap *h)
{
        assert(h != NULL);

        return h->size == 0;
}

// Modifiers
/**
 * @brief Extracts the top item from the pairing heap. This function assumes
 * that the pairing heap isn't empty.
 * @param h - cdc_pairing_heap
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_pairing_heap_extract_top(struct cdc_pairing_heap *h);

/**
 * @brief Inserts element key to the pairing heap. Write an iterator pointing
 * to a new element in the ret
 * @param h a cdc_pairing_heap
 * @param key
 * @param ret - pointer to iterator where an iterator will be written indicating
 * the inserted element
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_pairing_heap_riinsert(struct cdc_pairing_heap *h, void *key,
                                        struct cdc_pairing_heap_iter *ret);

/**
 * @brief Inserts element key to the pairing heap.
 * @param h - cdc_pairing_heap
 * @param key
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_pairing_heap_insert(struct cdc_pairing_heap *h,
                                                    void *key)
{
        assert(h != NULL);

        return cdc_pairing_heap_riinsert(h, key, NULL);
}

/**
 * @brief Changes the item key on the pos position in the pairing heap.
 * @param h - cdc_pairing_heap
 * @param pos - iterator that indicates the item with key that you want to change
 * @param key
 */
void cdc_pairing_heap_change_key(struct cdc_pairing_heap *h,
                                 struct cdc_pairing_heap_iter *pos, void *key);

/**
 * @brief Removes all the elements from the pairing heap.
 * @param h - cdc_pairing_heap
 */
void cdc_pairing_heap_clear(struct cdc_pairing_heap *h);

/**
 * @brief Swaps pairing heaps a and b. This operation is very fast and never
 * fails.
 * @param a - cdc_pairing_heap
 * @param b - cdc_pairing_heap
 */
void cdc_pairing_heap_swap(struct cdc_pairing_heap *a,
                           struct cdc_pairing_heap *b);

// Operations
/**
 * @brief Merges two heaps. In the heap h will be the result of the merger,
 * and the heap other will remain empty.
 * @param h - cdc_pairing_heap
 * @param other - other cdc_pairing_heap
 */
void cdc_pairing_heap_merge(struct cdc_pairing_heap *h,
                            struct cdc_pairing_heap *other);

/**
 * @brief Checks the heap property.
 * @param h - cdc_pairing_heap
 * @return result of the check
 */
bool cdc_pairing_heap_is_heap(struct cdc_pairing_heap *h);

// Iterators
/**
 * @brief Returns a pointer to the key of current item.
 */
static inline void *cdc_pairing_heap_iter_data(struct cdc_pairing_heap_iter it)
{
        return it.current->key;
}

/**
 * @brief Returns false if the iterator it1 equal to the iterator it2,
 * otherwise returns false
 */
static inline bool cdc_pairing_heap_iter_is_eq(struct cdc_pairing_heap_iter it1,
                                               struct cdc_pairing_heap_iter it2)
{
        return it1.container == it2.container && it1.current == it2.current;
}

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_pairing_heap pairing_heap_t;
typedef struct cdc_pairing_heap_iter pairing_heap_iter;

#define pairing_heap_ctor(...)         cdc_pairing_heap_ctor(__VA_ARGS__)
#define pairing_heap_ctorl(...)        cdc_pairing_heap_ctorl(__VA_ARGS__)
#define pairing_heap_ctorv(...)        cdc_pairing_heap_ctorv(__VA_ARGS__)
#define pairing_heap_dtor(...)         cdc_pairing_heap_dtor(__VA_ARGS__)

// Element access
#define pairing_heap_top(...)          cdc_pairing_heap_top(__VA_ARGS__)

// Capacity
#define pairing_heap_empty(...)        cdc_pairing_heap_empty(__VA_ARGS__)
#define pairing_heap_size(...)         cdc_pairing_heap_size(__VA_ARGS__)

// Modifiers
#define pairing_heap_extract_top(...)  cdc_pairing_heap_extract_top(__VA_ARGS__)
#define pairing_heap_riinsert(...)     cdc_pairing_heap_riinsert(__VA_ARGS__)
#define pairing_heap_insert(...)       cdc_pairing_heap_insert(__VA_ARGS__)
#define pairing_heap_change_key(...)   cdc_pairing_heap_change_key(__VA_ARGS__)
#define pairing_heap_clear(...)        cdc_pairing_heap_clear(__VA_ARGS__)
#define pairing_heap_swap(...)         cdc_pairing_heap_swap(__VA_ARGS__)

// Operations
#define pairing_heap_merge(...)        cdc_pairing_heap_merge(__VA_ARGS__)

#define pairing_heap_is_heap(...)      cdc_pairing_heap_is_heap(__VA_ARGS__)

// Iterators
#define pairing_heap_iter_data(...)    cdc_pairing_heap_iter_data(__VA_ARGS__)
#define pairing_heap_iter_is_eq(...)   cdc_pairing_heap_iter_is_eq(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_PAIRING_HEAP_H
