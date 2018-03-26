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
  * @brief The cdc_splay_tree is a struct and functions that provide a splay tree
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_SPLAY_TREE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_SPLAY_TREE_H

#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

/**
 * @brief The cdc_splay_tree_node struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_splay_tree_node {
    struct cdc_splay_tree_node *parent;
    struct cdc_splay_tree_node *left;
    struct cdc_splay_tree_node *right;
    void *key;
    void *value;
};

/**
 * @brief The cdc_splay_tree struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_splay_tree {
    struct cdc_splay_tree_node *root;
    size_t size;
    cdc_binary_pred_fn_t compar;
    struct cdc_data_info *dinfo;
};

/**
 * @brief The cdc_splay_tree_iter struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_splay_tree_iter {
    struct cdc_splay_tree *container;
    struct cdc_splay_tree_node *prev;
    struct cdc_splay_tree_node *current;
};

struct cdc_pair_splay_tree_iter {
    struct cdc_splay_tree_iter first;
    struct cdc_splay_tree_iter second;
};

struct cdc_pair_splay_tree_iter_bool {
    struct cdc_splay_tree_iter first;
    bool second;
};

/**
 * @brief Constructs an empty splay tree
 * @param t - cdc_splay_tree
 * @param info - cdc_data_info
 * @param compar - function that specifies a strict ordering
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_splay_tree_ctor(struct cdc_splay_tree **t, struct cdc_data_info *info,
                                  cdc_binary_pred_fn_t compar);

/**
 * @brief Constructs a splay tree, initialized by an arbitrary number of
 * pointers on cdc_pair's(first - key, and the second - value).  The last item
 * must be NULL.
 * @param t - cdc_splay_tree
 * @param info - cdc_data_info
 * @param compar - function that specifies a strict ordering
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_splay_tree_ctorl(struct cdc_splay_tree **t, struct cdc_data_info *info,
                                   cdc_binary_pred_fn_t compar, ...);

/**
 * @brief Constructs a splay tree, initialized by args. The last item must be NULL.
 * @param t - cdc_splay_tree
 * @param info - cdc_data_info
 * @param compar - function that specifies a strict ordering
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_splay_tree_ctorv(struct cdc_splay_tree **t, struct cdc_data_info *info,
                                   cdc_binary_pred_fn_t compar, va_list args);

/**
 * @brief Destroys the splay tree.
 * @param t - cdc_splay_tree
 */
void cdc_splay_tree_dtor(struct cdc_splay_tree *t);

// Lookup
/**
 * @brief Returns a pinter to the value that is mapped to a key. If the key does
 * not exist, then NULL will return.
 * @param t - cdc_splay_tree
 * @param key - key of the element to find
 * @param value - pinter to the value that is mapped to a key.
 * @return if the key is found - CDC_STATUS_OK, otherwise - CDC_STATUS_NOT_FOUND
 */
enum cdc_stat cdc_splay_tree_get(struct cdc_splay_tree *t, void *key, void **value);

/**
 * @brief Returns the number of elements with key that compares equal to the
 * specified argument key, which is either 1 or 0 since this container does not
 * allow duplicates.
 * @param t - cdc_splay_tree
 * @param key - key value of the elements to count
 * @return number of elements with key key, that is either 1 or 0.
 */
size_t cdc_splay_tree_count(struct cdc_splay_tree *t, void *key);

/**
 * @brief Finds an element with key equivalent to key.
 * @param t - cdc_splay_tree
 * @param key - key value of the element to search for
 * @return iterator to an element with key equivalent to key. If no such element
 * is found, past-the-end iterator is returned.
 */
struct cdc_splay_tree_iter cdc_splay_tree_find(struct cdc_splay_tree *t, void *key);

/**
 * @brief Returns a range containing all elements with key key in the container.
 * The range is defined by two iterators, the first pointing to the first element
 * of the wanted range and the second pointing past the last element of the range.
 * @param t - cdc_splay_tree
 * @param key - key value to compare the elements to
 * @return containing a pair of iterators defining the wanted range. If there are
 * no such elements, past-the-end iterators are returned as both elements of the
 * pair.
 */
struct cdc_pair_splay_tree_iter cdc_splay_tree_equal_range(struct cdc_splay_tree *t,
                                                           void *key);

// Capacity
/**
 * @brief Returns the number of items in the splay_tree.
 * @param t - cdc_splay_tree
 * @return size
 */
static inline size_t cdc_splay_tree_size(struct cdc_splay_tree *t)
{
    assert(t != NULL);

    return t->size;
}

/**
 * @brief Returns true if the splay_tree has size 0; otherwise returns false.
 * @param t - cdc_splay_tree
 * @return true if the hash splay_tree has size 0; otherwise returns false
 */
static inline bool cdc_splay_tree_empty(struct cdc_splay_tree *t)
{
    assert(t != NULL);

    return t->size == 0;
}

// Modifiers
/**
 * @brief Removes all the elements from the splay_tree.
 * @param t - cdc_splay_tree
 */
void cdc_splay_tree_clear(struct cdc_splay_tree *t);

/**
 * @brief Inserts element into the container, if the container doesn't already
 * contain an element with an equivalent key.
 * @param t - cdc_splay_tree
 * @param key - key of the element
 * @param value - value of the element
 * @param ret - pair consisting of an iterator to the inserted element (or to the
 * element that prevented the insertion) and a bool denoting whether the insertion
 * took place. The pointer can be equal to NULL
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_splay_tree_insert(struct cdc_splay_tree *t, void *key, void *value,
                                    struct cdc_pair_splay_tree_iter_bool *ret);

/**
 * @brief Inserts an element or assigns to the current element if the key
 * already exists
 * @param t - cdc_splay_tree
 * @param key - key of the element
 * @param value - value of the element
 * @param ret - pair. The bool component is true if the insertion took place and
 * false if the assignment took place. The iterator component is pointing at the
 * element that was inserted or updated
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_splay_tree_insert_or_assign(struct cdc_splay_tree *t,
                                              void *key, void *value,
                                              struct cdc_pair_splay_tree_iter_bool *ret);

/**
 * @brief Removes the element (if one exists) with the key equivalent to key.
 * @param t - cdc_splay_tree
 * @param key - key value of the elements to remove
 * @return number of elements removed
 */
size_t cdc_splay_tree_erase(struct cdc_splay_tree *t, void *key);

/**
 * @brief Swaps splay_trees a and b. This operation is very fast and never fails.
 * @param a - cdc_splay_tree
 * @param b - cdc_splay_tree
 */
void cdc_splay_tree_swap(struct cdc_splay_tree *a, struct cdc_splay_tree *b);

// Iterators
/**
 * @brief Returns an iterator to the beginning
 * @param t - cdc_splay_tree
 * @return iterator to the beginning
 */
struct cdc_splay_tree_iter cdc_splay_tree_begin(struct cdc_splay_tree *t);

/**
 * @brief Returns an iterator to the end
 * @param t - cdc_splay_tree
 * @return iterator to the end
 */
struct cdc_splay_tree_iter cdc_splay_tree_end(struct cdc_splay_tree *t);

// Iterators
/**
 * @brief Advances the iterator to the next item in the splay tree and returns an
 * iterator to the new current item
 */
struct cdc_splay_tree_iter cdc_splay_tree_iter_next(struct cdc_splay_tree_iter it);

/**
 * @brief Makes the preceding item current and returns an iterator to the new
 * current item.
 */
struct cdc_splay_tree_iter cdc_splay_tree_iter_prev(struct cdc_splay_tree_iter it);

/**
 * @brief Returns true if there is at least one item ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 */
static inline bool cdc_splay_tree_iter_has_next(struct cdc_splay_tree_iter it)
{
    return it.current != NULL;
}

/**
 * @brief Returns true if there is at least one item behind the iterator, i.e.
 * the iterator is not at the front of the container; otherwise returns false.
 */
static inline bool cdc_splay_tree_iter_has_prev(struct cdc_splay_tree_iter it)
{
    return it.prev != NULL;
}

/**
 * @brief Returns a pointer to the item's key.
 */
static inline void *cdc_splay_tree_iter_key(struct cdc_splay_tree_iter it)
{
    return it.current->key;
}

/**
 * @brief Returns a pointer to the item's value.
 */
static inline void *cdc_splay_tree_iter_value(struct cdc_splay_tree_iter it)
{
    return it.current->value;
}

/**
 * @brief Returns a pair, where first - key, second - value.
 */
static inline struct cdc_pair cdc_splay_tree_iter_key_value(
        struct cdc_splay_tree_iter it)
{
    struct cdc_pair pair = {it.prev->key, it.prev->value};

    return pair;
}

/**
 * @brief Returns false if the iterator it1 equal to the iterator it2,
 * otherwise returns false.
 */
static inline bool cdc_splay_tree_iter_is_eq(struct cdc_splay_tree_iter it1,
                                             struct cdc_splay_tree_iter it2)
{
    return it1.container == it2.container &&
            it1.prev == it2.prev &&
            it1.current == it2.current;
}

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_splay_tree splay_tree_t;
typedef struct cdc_splay_tree_iter splay_tree_iter_t;
typedef struct cdc_pair_splay_tree_iter pair_splay_tree_iter_t;
typedef struct cdc_pair_splay_tree_iter_bool pair_splay_tree_iter_bool_t;

#define splay_tree_ctor(...)                cdc_splay_tree_ctor(__VA_ARGS__)
#define splay_tree_ctorv(...)               cdc_splay_tree_ctorv(__VA_ARGS__)
#define splay_tree_ctorl(...)               cdc_splay_tree_ctorl(__VA_ARGS__)
#define splay_tree_dtor(...)                cdc_splay_tree_dtor(__VA_ARGS__)

// Lookup
#define splay_tree_get(...)                 cdc_splay_tree_get(__VA_ARGS__)
#define splay_tree_count(...)               cdc_splay_tree_count(__VA_ARGS__)
#define splay_tree_find(...)                cdc_splay_tree_find(__VA_ARGS__)
#define splay_tree_equal_range(...)         cdc_splay_tree_equal_range(__VA_ARGS__)

// Capacity
#define splay_tree_size(...)                cdc_splay_tree_size(__VA_ARGS__)
#define splay_tree_empty(...)               cdc_splay_tree_empty(__VA_ARGS__)

// Modifiers
#define splay_tree_clear(...)               cdc_splay_tree_clear(__VA_ARGS__)
#define splay_tree_insert(...)              cdc_splay_tree_insert(__VA_ARGS__)
#define splay_tree_insert_or_assign(...)    cdc_splay_tree_insert_or_assign(__VA_ARGS__)
#define splay_tree_erase(...)               cdc_splay_tree_erase(__VA_ARGS__)
#define splay_tree_swap(...)                cdc_splay_tree_swap(__VA_ARGS__)

// Iterators
#define splay_tree_begin(...)               cdc_splay_tree_begin(__VA_ARGS__)
#define splay_tree_end(...)                 cdc_splay_tree_end(__VA_ARGS__)

// Iterators
#define splay_tree_iter_next(...)           cdc_splay_tree_iter_next(__VA_ARGS__)
#define splay_tree_iter_has_next(...)       cdc_splay_tree_iter_has_next(__VA_ARGS__)
#define splay_tree_iter_key(...)            cdc_splay_tree_iter_key(__VA_ARGS__)
#define splay_tree_iter_value(...)          cdc_splay_tree_iter_value(__VA_ARGS__)
#define splay_tree_iter_key_value(...)      cdc_splay_tree_iter_key_value(__VA_ARGS__)
#define splay_tree_iter_is_eq(...)          cdc_splay_tree_iter_is_eq(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_SPLAY_TREE_H
