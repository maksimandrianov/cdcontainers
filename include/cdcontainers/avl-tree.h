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
 * @brief The cdc_avl_tree is a struct and functions that provide an avl tree.
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_avl_tree_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_avl_tree_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * @defgroup cdc_avl_tree
 * @brief The cdc_avl_tree is a struct and functions that provide an avl tree.
 * @{
 */
/**
 * @brief The cdc_avl_tree_node is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_avl_tree_node {
  struct cdc_avl_tree_node *parent;
  struct cdc_avl_tree_node *left;
  struct cdc_avl_tree_node *right;
  void *key;
  void *value;
  unsigned char height;
};

/**
 * @brief The cdc_avl_tree is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_avl_tree {
  struct cdc_avl_tree_node *root;
  size_t size;
  struct cdc_data_info *dinfo;
};

/**
 * @brief The cdc_avl_tree_iter is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_avl_tree_iter {
  struct cdc_avl_tree *container;
  struct cdc_avl_tree_node *prev;
  struct cdc_avl_tree_node *current;
};

struct cdc_pair_avl_tree_iter {
  struct cdc_avl_tree_iter first;
  struct cdc_avl_tree_iter second;
};

struct cdc_pair_avl_tree_iter_bool {
  struct cdc_avl_tree_iter first;
  bool second;
};

// Base
/**
 * @defgroup cdc_avl_tree_base Base
 * @{
 */
/**
 * @brief Constructs an empty avl tree.
 * @param[out] t - cdc_avl_tree
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_avl_tree_ctor(struct cdc_avl_tree **t, struct cdc_data_info *info);

/**
 * @brief Constructs an avl tree, initialized by an variable number of
 * pointers on cdc_pair's(first - key, and the second - value).  The last item
 * must be CDC_END.
 * @param[out] t - cdc_avl_tree
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 *
 * Example:
 * @code{.c}
 * struct cdc_avl_tree *tree = NULL;
 * cdc_pair value1 = {CDC_FROM_INT(1), CDC_FROM_INT(2)};
 * cdc_pair value2 = {CDC_FROM_INT(3), CDC_FROM_INT(4)};
 * ...
 * if (cdc_avl_tree_ctorl(&tree, info, &value1, &value2, CDC_END) != CDC_STATUS_OK) {
 *   // handle error
 * }
 * @endcode
 */
enum cdc_stat cdc_avl_tree_ctorl(struct cdc_avl_tree **t, struct cdc_data_info *info, ...);

/**
 * @brief Constructs an avl tree, initialized by args. The last item must be
 * CDC_END.
 * @param[out] t - cdc_avl_tree
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_avl_tree_ctorv(struct cdc_avl_tree **t, struct cdc_data_info *info, va_list args);

/**
 * @brief Destroys the avl tree.
 * @param[in] t - cdc_avl_tree
 */
void cdc_avl_tree_dtor(struct cdc_avl_tree *t);
/** @} */

// Lookup
/**
 * @defgroup cdc_avl_tree_lookup Lookup
 * @{
 */
/**
 * @brief Returns a value that is mapped to a key. If the key does
 * not exist, then NULL will return.
 * @param[in] t - cdc_avl_tree
 * @param[in] key - key of the element to find
 * @param[out] value - pinter to the value that is mapped to a key.
 * @return CDC_STATUS_OK if the key is found, CDC_STATUS_NOT_FOUND otherwise.
 */
enum cdc_stat cdc_avl_tree_get(struct cdc_avl_tree *t, void *key, void **value);

/**
 * @brief Returns the number of elements with key that compares equal to the
 * specified argument key, which is either 1 or 0 since this container does not
 * allow duplicates.
 * @param[in] t - cdc_avl_tree
 * @param[in] key - key value of the elements to count
 * @return number of elements with key key, that is either 1 or 0.
 */
size_t cdc_avl_tree_count(struct cdc_avl_tree *t, void *key);

/**
 * @brief Finds an element with key equivalent to key.
 * @param[in] t - cdc_avl_tree
 * @param[in] key - key value of the element to search for
 * @param[out] it - pointer will be recorded iterator to an element with key
 * equivalent to key. If no such element is found, past-the-end iterator is
 * returned.
 */
void cdc_avl_tree_find(struct cdc_avl_tree *t, void *key, struct cdc_avl_tree_iter *it);
/** @} */

// Capacity
/**
 * @defgroup cdc_avl_tree_capacity Capacity
 * @{
 */
/**
 * @brief Returns the number of items in the avl_tree.
 * @param[in] t - cdc_avl_tree
 * @return the number of items in the avl_tree.
 */
static inline size_t cdc_avl_tree_size(struct cdc_avl_tree *t)
{
  assert(t != NULL);

  return t->size;
}

/**
 * @brief Checks if the avl tree has no elements.
 * @param[in] t - cdc_avl_tree
 * @return true if the avl tree is empty, false otherwise.
 */
static inline bool cdc_avl_tree_empty(struct cdc_avl_tree *t)
{
  assert(t != NULL);

  return t->size == 0;
}
/** @} */

// Modifiers
/**
 * @defgroup cdc_avl_tree_modifiers Modifiers
 * @{
 */
/**
 * @brief Removes all the elements from the avl_tree.
 * @param[in] t - cdc_avl_tree
 */
void cdc_avl_tree_clear(struct cdc_avl_tree *t);

/**
 * @brief Inserts an element into the container, if the container doesn't already
 * contain an element with an equivalent key.
 * @param[in] t - cdc_avl_tree
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] ret - pair consisting of an iterator to the inserted element (or to
 * the element that prevented the insertion) and a bool denoting whether the
 * insertion took place. The pointer can be equal to NULL.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_avl_tree_insert(struct cdc_avl_tree *t, void *key, void *value,
                                  struct cdc_pair_avl_tree_iter_bool *ret);

/**
 * @brief Inserts an element into the container, if the container doesn't already
 * contain an element with an equivalent key.
 * @param[in] t - cdc_avl_tree
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] it - iterator to the inserted element (or to the element that
 * prevented the insertion). The pointer can be equal to NULL.
 * @param[out] inserted - bool denoting whether the insertion
 * took place. The pointer can be equal to NULL.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_avl_tree_insert1(struct cdc_avl_tree *t, void *key, void *value,
                                   struct cdc_avl_tree_iter *it, bool *inserted);

/**
 * @brief Inserts an element or assigns to the current element if the key
 * already exists.
 * @param[in] t - cdc_avl_tree
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] ret - pair. The bool component is true if the insertion took place and
 * false if the assignment took place. The iterator component is pointing at the
 * element that was inserted or updated.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_avl_tree_insert_or_assign(struct cdc_avl_tree *t, void *key, void *value,
                                            struct cdc_pair_avl_tree_iter_bool *ret);

/**
 * @brief Inserts an element or assigns to the current element if the key
 * already exists.
 * @param[in] t - cdc_avl_tree
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] it - iterator is pointing at the element that was inserted or updated.
 * The pointer can be equal to NULL
 * @param[out] inserted - bool is true if the insertion took place and false if the
 * assignment took place. The pointer can be equal to NULL
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_avl_tree_insert_or_assign1(struct cdc_avl_tree *t, void *key, void *value,
                                             struct cdc_avl_tree_iter *it, bool *inserted);

/**
 * @brief Removes the element (if one exists) with the key equivalent to key.
 * @param[in] t - cdc_avl_tree
 * @param[in] key - key value of the elements to remove
 * @return number of elements removed.
 */
size_t cdc_avl_tree_erase(struct cdc_avl_tree *t, void *key);

/**
 * @brief Swaps avl_trees a and b. This operation is very fast and never fails.
 * @param[in, out] a - cdc_avl_tree
 * @param[in, out] b - cdc_avl_tree
 */
void cdc_avl_tree_swap(struct cdc_avl_tree *a, struct cdc_avl_tree *b);
/** @} */

// Iterators
/**
 * @defgroup cdc_avl_tree_iterators Iterators
 * @{
 */
/**
 * @brief Initializes the iterator to the beginning.
 * @param[in] t - cdc_avl_tree
 * @param[out] it - cdc_avl_tree_iter
 */
void cdc_avl_tree_begin(struct cdc_avl_tree *t, struct cdc_avl_tree_iter *it);

/**
 * @brief Initializes the iterator to the end.
 * @param[in] t - cdc_avl_tree
 * @param[out] it - cdc_avl_tree_iter
 */
void cdc_avl_tree_end(struct cdc_avl_tree *t, struct cdc_avl_tree_iter *it);
/** @} */

// Iterators
/**
 * @defgroup cdc_avl_tree_iter
 * @brief The cdc_avl_tree_iter is a struct and functions that provide an avl tree iterator.
 * @{
 */
/**
 * @brief Advances the iterator to the next element in the avl tree.
 * @param[in] it - iterator
 */
void cdc_avl_tree_iter_next(struct cdc_avl_tree_iter *it);

/**
 * @brief Advances the iterator to the previous element in the avl tree.
 * @param[in] it - iterator
 */
void cdc_avl_tree_iter_prev(struct cdc_avl_tree_iter *it);

/**
 * @brief Returns true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 * @param[in] it - iterator
 * @return true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 */
static inline bool cdc_avl_tree_iter_has_next(struct cdc_avl_tree_iter *it)
{
  assert(it != NULL);

  return it->current != NULL;
}

/**
 * @brief Returns true if there is at least one element behind the iterator, i.e.
 * the iterator is not at the front of the container; otherwise returns false.
 * @param[in] it - iterator
 * @return true if there is at least one element behind the iterator, i.e.
 * the iterator is not at the front of the container; otherwise returns false.
 */
static inline bool cdc_avl_tree_iter_has_prev(struct cdc_avl_tree_iter *it)
{
  assert(it != NULL);

  return it->prev != NULL;
}

/**
 * @brief Returns an item's key.
 * @param[in] it - iterator
 * @return the item's key.
 */
static inline void *cdc_avl_tree_iter_key(struct cdc_avl_tree_iter *it)
{
  assert(it != NULL);

  return it->current->key;
}

/**
 * @brief Returns an item's value.
 * @param[in] it - iterator
 * @return the item's value.
 */
static inline void *cdc_avl_tree_iter_value(struct cdc_avl_tree_iter *it)
{
  assert(it != NULL);

  return it->current->value;
}

/**
 * @brief Returns a pair, where first - key, second - value.
 * @param[in] it - iterator
 * @return pair, where first - key, second - value.
 */
static inline struct cdc_pair cdc_avl_tree_iter_key_value(struct cdc_avl_tree_iter *it)
{
  assert(it != NULL);

  struct cdc_pair pair = {it->prev->key, it->prev->value};
  return pair;
}

/**
 * @brief Returns false if the iterator |it1| equal to the iterator |it2|,
 * otherwise returns false.
 * @param[in] it1 - iterator
 * @param[in] it2 - iterator
 * @return false if the iterator |it1| equal to the iterator |it2|,
 * otherwise returns false.
 */
static inline bool cdc_avl_tree_iter_is_eq(struct cdc_avl_tree_iter *it1,
                                           struct cdc_avl_tree_iter *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);

  return it1->container == it2->container && it1->prev == it2->prev && it1->current == it2->current;
}
/** @} */

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_avl_tree_node avl_tree_node_t;
typedef struct cdc_avl_tree avl_tree_t;
typedef struct cdc_avl_tree_iter avl_tree_iter_t;
typedef struct cdc_pair_avl_tree_iter pair_avl_tree_iter_t;
typedef struct cdc_pair_avl_tree_iter_bool pair_avl_tree_iter_bool_t;

// Base
#define avl_tree_ctor(...) cdc_avl_tree_ctor(__VA_ARGS__)
#define avl_tree_ctorv(...) cdc_avl_tree_ctorv(__VA_ARGS__)
#define avl_tree_ctorl(...) cdc_avl_tree_ctorl(__VA_ARGS__)
#define avl_tree_dtor(...) cdc_avl_tree_dtor(__VA_ARGS__)

// Lookup
#define avl_tree_get(...) cdc_avl_tree_get(__VA_ARGS__)
#define avl_tree_count(...) cdc_avl_tree_count(__VA_ARGS__)
#define avl_tree_find(...) cdc_avl_tree_find(__VA_ARGS__)

// Capacity
#define avl_tree_size(...) cdc_avl_tree_size(__VA_ARGS__)
#define avl_tree_empty(...) cdc_avl_tree_empty(__VA_ARGS__)

// Modifiers
#define avl_tree_clear(...) cdc_avl_tree_clear(__VA_ARGS__)
#define avl_tree_insert(...) cdc_avl_tree_insert(__VA_ARGS__)
#define avl_tree_insert1(...) cdc_avl_tree_insert1(__VA_ARGS__)
#define avl_tree_insert_or_assign(...) cdc_avl_tree_insert_or_assign(__VA_ARGS__)
#define avl_tree_insert_or_assign1(...) cdc_avl_tree_insert_or_assign1(__VA_ARGS__)
#define avl_tree_erase(...) cdc_avl_tree_erase(__VA_ARGS__)
#define avl_tree_swap(...) cdc_avl_tree_swap(__VA_ARGS__)

// Iterators
#define avl_tree_begin(...) cdc_avl_tree_begin(__VA_ARGS__)
#define avl_tree_end(...) cdc_avl_tree_end(__VA_ARGS__)

// Iterators
#define avl_tree_iter_next(...) cdc_avl_tree_iter_next(__VA_ARGS__)
#define avl_tree_iter_has_next(...) cdc_avl_tree_iter_has_next(__VA_ARGS__)
#define avl_tree_iter_has_prev(...) cdc_avl_tree_iter_has_prev(__VA_ARGS__)
#define avl_tree_iter_prev(...) cdc_avl_tree_iter_prev(__VA_ARGS__)
#define avl_tree_iter_key(...) cdc_avl_tree_iter_key(__VA_ARGS__)
#define avl_tree_iter_value(...) cdc_avl_tree_iter_value(__VA_ARGS__)
#define avl_tree_iter_key_value(...) cdc_avl_tree_iter_key_value(__VA_ARGS__)
#define avl_tree_iter_is_eq(...) cdc_avl_tree_iter_is_eq(__VA_ARGS__)
#endif
/** @} */
#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_avl_tree_H
