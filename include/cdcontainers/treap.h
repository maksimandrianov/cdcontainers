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
 * @brief The cdc_treap is a struct and functions that provide a cartesion tree.
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_TREAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_TREAP_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * @defgroup cdc_treap
 * @brief The cdc_treap is a struct and functions that provide a treap.
 * @{
 */

typedef int (*cdc_priority_fn_t)(void *);

/**
 * @brief The cdc_treap_node is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_treap_node {
  struct cdc_treap_node *parent;
  struct cdc_treap_node *left;
  struct cdc_treap_node *right;
  void *key;
  void *value;
  int priority;
};

/**
 * @brief The cdc_treap is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_treap {
  struct cdc_treap_node *root;
  size_t size;
  cdc_priority_fn_t prior;
  struct cdc_data_info *dinfo;
};

/**
 * @brief The cdc_treap_iter is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_treap_iter {
  struct cdc_treap *container;
  struct cdc_treap_node *prev;
  struct cdc_treap_node *current;
};

struct cdc_pair_treap_iter_bool {
  struct cdc_treap_iter first;
  bool second;
};

// Base
/**
 * @defgroup cdc_treap_base Base
 * @{
 */
/**
 * @brief Constructs an empty treap.
 * @param[out] t - cdc_treap
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_ctor(struct cdc_treap **t, struct cdc_data_info *info);

/**
 * @brief Constructs a treap, initialized by an variable number of
 * pointers on cdc_pair's(first - key, and the second - value).  The last item
 * must be CDC_END.
 * @param[out] t - cdc_treap
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 *
 * Example:
 * @code{.c}
 * struct cdc_treap *tree = NULL;
 * cdc_pair value1 = {CDC_FROM_INT(1), CDC_FROM_INT(2)};
 * cdc_pair value2 = {CDC_FROM_INT(3), CDC_FROM_INT(4)};
 * ...
 * if (cdc_treap_ctorl(&tree, info, &value1, &value2, CDC_END) != CDC_STATUS_OK) {
 *   // handle error
 * }
 * @endcode
 */
enum cdc_stat cdc_treap_ctorl(struct cdc_treap **t, struct cdc_data_info *info, ...);

/**
 * @brief Constructs a treap, initialized by args. The last item must be
 * CDC_END.
 * @param[out] t - cdc_treap
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_ctorv(struct cdc_treap **t, struct cdc_data_info *info, va_list args);

/**
 * @brief Constructs an empty treap.
 * @param[out] t - cdc_treap
 * @param[in] info - cdc_data_info
 * @param[in] prior - function that generates a priority
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_ctor1(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_priority_fn_t prior);

/**
 * @brief Constructs a treap, initialized by an variable number of
 * pointers on cdc_pair's(first - key, and the second - value).  The last item
 * must be CDC_END.
 * @param[out] t - cdc_treap
 * @param[in] info - cdc_data_info
 * @param[in] prior - function that generates a priority
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_ctorl1(struct cdc_treap **t, struct cdc_data_info *info,
                               cdc_priority_fn_t prior, ...);

/**
 * @brief Constructs a treap, initialized by args. The last item must be
 * CDC_END.
 * @param[out] t - cdc_treap
 * @param[in] info - cdc_data_info
 * @param[in] prior - function that generates a priority
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_ctorv1(struct cdc_treap **t, struct cdc_data_info *info,
                               cdc_priority_fn_t prior, va_list args);

/**
 * @brief Destroys the treap.
 * @param[in] t - cdc_treap
 */
void cdc_treap_dtor(struct cdc_treap *t);
/** @} */

// Lookup
/**
 * @defgroup cdc_treap_lookup Lookup
 * @{
 */
/**
 * @brief Returns a value that is mapped to a key. If the key does
 * not exist, then NULL will return.
 * @param[in] t - cdc_treap
 * @param[in] key - key of the element to find
 * @param[out] value - pinter to the value that is mapped to a key.
 * @return CDC_STATUS_OK if the key is found, CDC_STATUS_NOT_FOUND otherwise.
 */
enum cdc_stat cdc_treap_get(struct cdc_treap *t, void *key, void **value);

/**
 * @brief Returns the number of elements with key that compares equal to the
 * specified argument key, which is either 1 or 0 since this container does not
 * allow duplicates.
 * @param[in] t - cdc_treap
 * @param[in] key - key value of the elements to count
 * @return number of elements with key key, that is either 1 or 0.
 */
size_t cdc_treap_count(struct cdc_treap *t, void *key);

/**
 * @brief Finds an element with key equivalent to key.
 * @param[in] t - cdc_treap
 * @param[in] key - key value of the element to search for
 * @param[out] it - pointer will be recorded iterator to an element with key
 * equivalent to key. If no such element is found, past-the-end iterator is
 * returned.
 */
void cdc_treap_find(struct cdc_treap *t, void *key, struct cdc_treap_iter *it);
/** @} */

// Capacity
/**
 * @defgroup cdc_treap_capacity Capacity
 * @{
 */
/**
 * @brief Returns the number of items in the treap.
 * @param[in] t - cdc_treap
 * @return the number of items in the treap.
 */
static inline size_t cdc_treap_size(struct cdc_treap *t)
{
  assert(t != NULL);

  return t->size;
}

/**
 * @brief Checks if the treap has no elements.
 * @param[in] t - cdc_treap
 * @return true if the treap is empty, false otherwise.
 */
static inline bool cdc_treap_empty(struct cdc_treap *t)
{
  assert(t != NULL);

  return t->size == 0;
}
/** @} */

// Modifiers
/**
 * @defgroup cdc_treap_modifiers Modifiers
 * @{
 */
/**
 * @brief Removes all the elements from the treap.
 * @param[in] t - cdc_treap
 */
void cdc_treap_clear(struct cdc_treap *t);

/**
 * @brief Inserts an element into the container, if the container doesn't already
 * contain an element with an equivalent key.
 * @param[in] t - cdc_treap
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] ret - pair consisting of an iterator to the inserted element (or to
 * the element that prevented the insertion) and a bool denoting whether the
 * insertion took place. The pointer can be equal to NULL.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_insert(struct cdc_treap *t, void *key, void *value,
                               struct cdc_pair_treap_iter_bool *ret);

/**
 * @brief Inserts an element into the container, if the container doesn't already
 * contain an element with an equivalent key.
 * @param[in] t - cdc_treap
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] it - iterator to the inserted element (or to the element that
 * prevented the insertion). The pointer can be equal to NULL.
 * @param[out] inserted - bool denoting whether the insertion
 * took place. The pointer can be equal to NULL.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_insert1(struct cdc_treap *t, void *key, void *value,
                                struct cdc_treap_iter *it, bool *inserted);

/**
 * @brief Inserts an element or assigns to the current element if the key
 * already exists.
 * @param[in] t - cdc_treap
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] ret - pair. The bool component is true if the insertion took place and
 * false if the assignment took place. The iterator component is pointing at the
 * element that was inserted or updated.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_insert_or_assign(struct cdc_treap *t, void *key, void *value,
                                         struct cdc_pair_treap_iter_bool *ret);

/**
 * @brief Inserts an element or assigns to the current element if the key
 * already exists.
 * @param[in] t - cdc_treap
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] it - iterator is pointing at the element that was inserted or updated.
 * The pointer can be equal to NULL
 * @param[out] inserted - bool is true if the insertion took place and false if the
 * assignment took place. The pointer can be equal to NULL
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_treap_insert_or_assign1(struct cdc_treap *t, void *key, void *value,
                                          struct cdc_treap_iter *it, bool *inserted);

/**
 * @brief Removes the element (if one exists) with the key equivalent to key.
 * @param[in] t - cdc_treap
 * @param[in] key - key value of the elements to remove
 * @return number of elements removed.
 */
size_t cdc_treap_erase(struct cdc_treap *t, void *key);

/**
 * @brief Swaps treaps a and b. This operation is very fast and never fails.
 * @param[in, out] a - cdc_treap
 * @param[in, out] b - cdc_treap
 */
void cdc_treap_swap(struct cdc_treap *a, struct cdc_treap *b);
/** @} */

// Iterators
/**
 * @defgroup cdc_treap_iterators Iterators
 * @{
 */
/**
 * @brief Initializes the iterator to the beginning.
 * @param t[in] - cdc_treap
 * @param it[out] - cdc_treap_iter
 */
void cdc_treap_begin(struct cdc_treap *t, struct cdc_treap_iter *it);

/**
 * @brief Initializes the iterator to the end.
 * @param[in] t - cdc_treap
 * @param[out] it - cdc_treap_iter
 */
void cdc_treap_end(struct cdc_treap *t, struct cdc_treap_iter *it);
/** @} */

// Iterators
/**
 * @defgroup cdc_treap_iter
 * @brief The cdc_treap_iter is a struct and functions that provide a treap iterator.
 * @{
 */
/**
 * @brief Advances the iterator to the next element in the treap.
 * @param[in] it - iterator
 */
void cdc_treap_iter_next(struct cdc_treap_iter *it);

/**
 * @brief Advances the iterator to the previous element in the treap.
 * @param[in] it - iterator
 */
void cdc_treap_iter_prev(struct cdc_treap_iter *it);

/**
 * @brief Returns true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 * @param[in] it - iterator
 * @return true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 */
static inline bool cdc_treap_iter_has_next(struct cdc_treap_iter *it)
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
static inline bool cdc_treap_iter_has_prev(struct cdc_treap_iter *it)
{
  assert(it != NULL);

  return it->prev != NULL;
}

/**
 * @brief Returns an item's key.
 * @param[in] it - iterator
 * @return the item's key.
 */
static inline void *cdc_treap_iter_key(struct cdc_treap_iter *it)
{
  assert(it != NULL);

  return it->current->key;
}

/**
 * @brief Returns an item's value.
 * @param[in] it - iterator
 * @return the item's value.
 */
static inline void *cdc_treap_iter_value(struct cdc_treap_iter *it)
{
  assert(it != NULL);

  return it->current->value;
}

/**
 * @brief Returns a pair, where first - key, second - value.
 * @param[in] it - iterator
 * @return pair, where first - key, second - value.
 */
static inline struct cdc_pair cdc_treap_iter_key_value(struct cdc_treap_iter *it)
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
static inline bool cdc_treap_iter_is_eq(struct cdc_treap_iter *it1, struct cdc_treap_iter *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);

  return it1->container == it2->container && it1->prev == it2->prev && it1->current == it2->current;
}
/** @} */

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_treap_node treap_node_t;
typedef struct cdc_treap treap_t;
typedef struct cdc_treap_iter treap_iter_t;
typedef struct cdc_pair_treap_iter pair_treap_iter_t;
typedef struct cdc_pair_treap_iter_bool pair_treap_iter_bool_t;

// Base
#define treap_ctor(...) cdc_treap_ctor(__VA_ARGS__)
#define treap_ctorv(...) cdc_treap_ctorv(__VA_ARGS__)
#define treap_ctorl(...) cdc_treap_ctorl(__VA_ARGS__)
#define treap_ctor1(...) cdc_treap_ctor1(__VA_ARGS__)
#define treap_ctorv1(...) cdc_treap_ctorv1(__VA_ARGS__)
#define treap_ctorl1(...) cdc_treap_ctorl1(__VA_ARGS__)
#define treap_dtor(...) cdc_treap_dtor(__VA_ARGS__)

// Lookup
#define treap_get(...) cdc_treap_get(__VA_ARGS__)
#define treap_count(...) cdc_treap_count(__VA_ARGS__)
#define treap_find(...) cdc_treap_find(__VA_ARGS__)

// Capacity
#define treap_size(...) cdc_treap_size(__VA_ARGS__)
#define treap_empty(...) cdc_treap_empty(__VA_ARGS__)

// Modifiers
#define treap_clear(...) cdc_treap_clear(__VA_ARGS__)
#define treap_insert(...) cdc_treap_insert(__VA_ARGS__)
#define treap_insert1(...) cdc_treap_insert1(__VA_ARGS__)
#define treap_insert_or_assign(...) cdc_treap_insert_or_assign(__VA_ARGS__)
#define treap_insert_or_assign1(...) cdc_treap_insert_or_assign1(__VA_ARGS__)
#define treap_erase(...) cdc_treap_erase(__VA_ARGS__)
#define treap_swap(...) cdc_treap_swap(__VA_ARGS__)

// Iterators
#define treap_begin(...) cdc_treap_begin(__VA_ARGS__)
#define treap_end(...) cdc_treap_end(__VA_ARGS__)

// Iterators
#define treap_iter_next(...) cdc_treap_iter_next(__VA_ARGS__)
#define treap_iter_prev(...) cdc_treap_iter_prev(__VA_ARGS__)
#define treap_iter_has_next(...) cdc_treap_iter_has_next(__VA_ARGS__)
#define treap_iter_has_prev(...) cdc_treap_iter_has_prev(__VA_ARGS__)
#define treap_iter_key(...) cdc_treap_iter_key(__VA_ARGS__)
#define treap_iter_value(...) cdc_treap_iter_value(__VA_ARGS__)
#define treap_iter_key_value(...) cdc_treap_iter_key_value(__VA_ARGS__)
#define treap_iter_is_eq(...) cdc_treap_iter_is_eq(__VA_ARGS__)
#endif
/** @} */
#endif  // CDSTRUCTURES_INCLUDE_CDCONTAINERS_VECTOR_H
