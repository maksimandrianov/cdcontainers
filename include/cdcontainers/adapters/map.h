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
 * @file map.h
 * @brief The cdc_map is a struct and functions that provide a map.
 *
 * Example usage map:
 * @include map.c
 *
 * @author Maksim Andrianov <maksimandrianov1@yandex.ru>
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_MAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_MAP_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>
#include <cdcontainers/tables/imap.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * @defgroup cdc_map
 * @brief The cdc_map is a struct and functions that provide a map.
 * @{
 */
/**
 * @brief The cdc_map is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_map {
  void *container;
  const struct cdc_map_table *table;
};

/**
 * @brief The cdc_map_iter is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_map_iter {
  void *iter;
  const struct cdc_map_iter_table *table;
};

// Base
/**
 * @defgroup cdc_map_base Base
 * @{
 */
/**
 * @brief Constructs an empty map.
 * @param[in] table - table of a map implementation. It can be cdc_map_avl,
 * cdc_map_splay, cdc_map_map, cdc_map_htable.
 * @param[out] m - cdc_map
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 *
 * Example of map constracting with hash table implementation:
 * @code{.c}
 * struct cdc_map *map = NULL;
 * ...
 * if (cdc_map_ctor(cdc_map_ctor, map, info) != CDC_STATUS_OK) {
 *   // handle error
 * }
 * @endcode
 */
enum cdc_stat cdc_map_ctor(const struct cdc_map_table *table, struct cdc_map **m,
                           struct cdc_data_info *info);

/**
 * @brief Constructs a map, initialized by an variable number of
 * pointers on cdc_pair's(first - key, and the second - value).  The last item
 * must be CDC_END.
 * @param[in] table - table of a map implementation. It can be cdc_map_avl,
 * cdc_map_splay, cdc_map_map, cdc_map_htable.
 * @param[out] m - cdc_map
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 *
 * Example:
 * @code{.c}
 * struct cdc_map *map = NULL;
 * cdc_pair value1 = {CDC_FROM_INT(1), CDC_FROM_INT(2)};
 * cdc_pair value2 = {CDC_FROM_INT(3), CDC_FROM_INT(4)};
 * ...
 * if (cdc_map_ctorl(cdc_map_avl, &tree, info, &value1, &value2, CDC_END) != CDC_STATUS_OK) {
 *   // handle error
 * }
 * @endcode
 */
enum cdc_stat cdc_map_ctorl(const struct cdc_map_table *table, struct cdc_map **m,
                            struct cdc_data_info *info, ...);

/**
 * @brief Constructs a map, initialized by args. The last item must be
 * CDC_END.
 * @param[in] table - table of a map implementation. It can be cdc_map_avl,
 * cdc_map_splay, cdc_map_map, cdc_map_htable.
 * @param[out] m - cdc_map
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_map_ctorv(const struct cdc_map_table *table, struct cdc_map **m,
                            struct cdc_data_info *info, va_list args);
/**
 * @brief Destroys the map.
 * @param[in] t - cdc_map
 */
void cdc_map_dtor(struct cdc_map *m);
/** @} */

// Lookup
/**
 * @defgroup cdc_map_lookup Lookup
 * @{
 */
/**
 * @brief Returns a value that is mapped to a key. If the key does
 * not exist, then NULL will return.
 * @param[in] m - cdc_map
 * @param[in] key - key of the element to find
 * @param[out] value - pinter to the value that is mapped to a key.
 * @return CDC_STATUS_OK if the key is found, CDC_STATUS_NOT_FOUND otherwise.
 */
static inline enum cdc_stat cdc_map_get(struct cdc_map *m, void *key, void **value)
{
  assert(m != NULL);

  return m->table->get(m->container, key, value);
}

/**
 * @brief Returns the number of elements with key that compares equal to the
 * specified argument key, which is either 1 or 0 since this container does not
 * allow duplicates.
 * @param[in] m - cdc_map
 * @param[in] key - key value of the elements to count
 * @return number of elements with key key, that is either 1 or 0.
 */
static inline size_t cdc_map_count(struct cdc_map *m, void *key)
{
  assert(m != NULL);

  return m->table->count(m->container, key);
}

/**
 * @brief Finds an element with key equivalent to key.
 * @param[in] m - cdc_map
 * @param[in] key - key value of the element to search for
 * @param[out] it - pointer will be recorded iterator to an element with key
 * equivalent to key. If no such element is found, past-the-end iterator is
 * returned.
 */
static inline void cdc_map_find(struct cdc_map *m, void *key, struct cdc_map_iter *it)
{
  assert(m != NULL);

  m->table->find(m->container, key, it->iter);
}
/** @} */

// Capacity
/**
 * @defgroup cdc_map_capacity Capacity
 * @{
 */
/**
 * @brief Returns the number of items in the map.
 * @param[in] m - cdc_map
 * @return the number of items in the map.
 */
static inline size_t cdc_map_size(struct cdc_map *m)
{
  assert(m != NULL);

  return m->table->size(m->container);
}

/**
 * @brief Checks if the map has no elements.
 * @param[in] m - cdc_map
 * @return true if the map is empty, false otherwise.
 */
static inline bool cdc_map_empty(struct cdc_map *m)
{
  assert(m != NULL);

  return m->table->empty(m->container);
}
/** @} */

// Modifiers
/**
 * @defgroup cdc_map_modifiers Modifiers
 * @{
 */
/**
 * @brief Removes all the elements from the map.
 * @param[in] m - cdc_map
 */
static inline void cdc_map_clear(struct cdc_map *m)
{
  assert(m != NULL);

  m->table->clear(m->container);
}

/**
 * @brief Inserts an element into the container, if the container doesn't already
 * contain an element with an equivalent key.
 * @param[in] m - cdc_map
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] it - iterator to the inserted element (or to the element that
 * prevented the insertion). The pointer can be equal to NULL.
 * @param[out] inserted - bool denoting whether the insertion
 * took place. The pointer can be equal to NULL.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
static inline enum cdc_stat cdc_map_insert(struct cdc_map *m, void *key, void *value,
                                           struct cdc_map_iter *it, bool *inserted)
{
  assert(m != NULL);

  void *iter = it ? it->iter : NULL;
  return m->table->insert(m->container, key, value, iter, inserted);
}

/**
 * @brief Inserts an element or assigns to the current element if the key
 * already exists.
 * @param[in] m - cdc_map
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] it - iterator is pointing at the element that was inserted or updated.
 * The pointer can be equal to NULL
 * @param[out] inserted - bool is true if the insertion took place and false if the
 * assignment took place. The pointer can be equal to NULL
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
static inline enum cdc_stat cdc_map_insert_or_assign(struct cdc_map *m, void *key, void *value,
                                                     struct cdc_map_iter *it, bool *inserted)
{
  assert(m != NULL);

  void *iter = it ? it->iter : NULL;
  return m->table->insert_or_assign(m->container, key, value, iter, inserted);
}

/**
 * @brief Removes the element (if one exists) with the key equivalent to key.
 * @param[in] m - cdc_map
 * @param[in] key - key value of the elements to remove
 * @return number of elements removed.
 */
static inline size_t cdc_map_erase(struct cdc_map *m, void *key)
{
  assert(m != NULL);

  return m->table->erase(m->container, key);
}

/**
 * @brief Swaps maps a and b. This operation is very fast and never fails.
 * @param[in, out] a - cdc_map
 * @param[in, out] b - cdc_map
 */
static inline void cdc_map_swap(struct cdc_map *a, struct cdc_map *b)
{
  assert(a != NULL);
  assert(b != NULL);
  assert(a->table == b->table);

  CDC_SWAP(void *, a->container, b->container);
}
/** @} */

// Iterators
/**
 * @defgroup cdc_map_iterators Iterators
 * @{
 */
/**
 * @brief Initializes the iterator to the beginning.
 * @param[in] m - cdc_map
 * @param[out] out - cdc_map_iter
 */
static inline void cdc_map_begin(struct cdc_map *m, struct cdc_map_iter *it)
{
  assert(m != NULL);
  assert(it != NULL);

  m->table->begin(m->container, it->iter);
}

/**
 * @brief Initializes the iterator to the end.
 * @param[in] m - cdc_map
 * @param[out] it - iterator
 */
static inline void cdc_map_end(struct cdc_map *m, struct cdc_map_iter *it)
{
  assert(m != NULL);
  assert(it != NULL);

  m->table->end(m->container, it->iter);
}
/** @} */

// Iterators
/**
 * @defgroup cdc_map_iter
 * @brief The cdc_map_iter is a struct and functions that provide a map iterator.
 * @{
 */
/**
 * @brief Constructs a map iterator. Should be called for each new iterator.
 * @param[in] t - cdc_map
 * @param[out] it - iterator
 *
 * Example:
 * @code{.c}
 * struct cdc_map *map = NULL;
 * ...
 * struct cdc_map_iter it = CDC_INIT_STRUCT;
 * if (cdc_map_iter_ctor(map, &it) != CDC_STATUS_OK) {
 *   // handle error
 * }
 *
 * cdc_map_begin(map, &it);
 * while (cdc_map_iter_has_next(&it)) {
 *   void *value = cdc_map_iter_value(&it);
 *   ...
 *   cdc_map_iter_next(&it);
 * }
 *
 * @endcode
 */
enum cdc_stat cdc_map_iter_ctor(struct cdc_map *m, struct cdc_map_iter *it);

/**
 * @brief Destroys the iterator. It should be called after the iterator is no
 * longer needed. Releases resources.
 * @param[in] it - iterator
 */
static inline void cdc_map_iter_dtor(struct cdc_map_iter *it)
{
  assert(it != NULL);

  it->table->dtor(it->iter);
}

/**
 * @brief Returns a type of iterator.
 * @param[in] it - iterator
 * @return a type of iterator.
 */
static inline enum cdc_iterator_type cdc_map_iter_type(struct cdc_map_iter *it)
{
  assert(it != NULL);

  return it->table->type();
}

/**
 * @brief Advances the iterator to the next element in the map.
 * @param[in] it - iterator
 */
static inline void cdc_map_iter_next(struct cdc_map_iter *it)
{
  assert(it != NULL);

  it->table->next(it->iter);
}

/**
 * @brief Advances the iterator to the previous element in the map.
 * @param[in] it - iterator
 */
static inline void cdc_map_iter_prev(struct cdc_map_iter *it)
{
  assert(it != NULL);

  it->table->prev(it->iter);
}

/**
 * @brief Returns true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 * @param[in] it - iterator
 * @return true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 */
static inline bool cdc_map_iter_has_next(struct cdc_map_iter *it)
{
  assert(it != NULL);

  return it->table->has_next(it->iter);
}

/**
 * @brief Returns true if there is at least one element behind the iterator, i.e.
 * the iterator is not at the front of the container; otherwise returns false.
 * @param[in] it - iterator
 * @return true if there is at least one element behind the iterator, i.e.
 * the iterator is not at the front of the container; otherwise returns false.
 */
static inline bool cdc_map_iter_has_prev(struct cdc_map_iter *it)
{
  assert(it != NULL);

  return it->table->has_prev(it->iter);
}

/**
 * @brief Returns an item's key.
 * @param[in] it - iterator
 * @return the item's key.
 */
static inline void *cdc_map_iter_key(struct cdc_map_iter *it)
{
  assert(it != NULL);

  return it->table->key(it->iter);
}

/**
 * @brief Returns an item's value.
 * @param[in] it - iterator
 * @return the item's value.
 */
static inline void *cdc_map_iter_value(struct cdc_map_iter *it)
{
  assert(it != NULL);

  return it->table->value(it->iter);
}

/**
 * @brief Returns a pair, where first - key, second - value.
 * @param[in] it - iterator
 * @return pair, where first - key, second - value.
 */
static inline struct cdc_pair cdc_map_iter_key_value(struct cdc_map_iter *it)
{
  assert(it != NULL);

  return it->table->key_value(it->iter);
}

/**
 * @brief Returns false if the iterator |it1| equal to the iterator |it2|,
 * otherwise returns false.
 * @param[in] it1 - iterator
 * @param[in] it2 - iterator
 * @return false if the iterator |it1| equal to the iterator |it2|,
 * otherwise returns false.
 */
static inline bool cdc_map_iter_is_eq(struct cdc_map_iter *it1, struct cdc_map_iter *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);
  assert(it1->table == it2->table);

  return it1->table->eq(it1->iter, it2->iter);
}
/** @} */

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_map map_t;
typedef struct cdc_map_iter map_iter_t;

// Base
#define map_ctor(...) cdc_map_ctor(__VA_ARGS__)
#define map_ctorv(...) cdc_map_ctorv(__VA_ARGS__)
#define map_ctorl(...) cdc_map_ctorl(__VA_ARGS__)
#define map_dtor(...) cdc_map_dtor(__VA_ARGS__)

// Lookup
#define map_get(...) cdc_map_get(__VA_ARGS__)
#define map_count(...) cdc_map_count(__VA_ARGS__)
#define map_find(...) cdc_map_find(__VA_ARGS__)

// Capacity
#define map_size(...) cdc_map_size(__VA_ARGS__)
#define map_empty(...) cdc_map_empty(__VA_ARGS__)

// Modifiers
#define map_clear(...) cdc_map_clear(__VA_ARGS__)
#define map_insert(...) cdc_map_insert(__VA_ARGS__)
#define map_insert_or_assign(...) cdc_map_insert_or_assign(__VA_ARGS__)
#define map_erase(...) cdc_map_erase(__VA_ARGS__)
#define map_swap(...) cdc_map_swap(__VA_ARGS__)

// Iterators
#define map_begin(...) cdc_map_begin(__VA_ARGS__)
#define map_end(...) cdc_map_end(__VA_ARGS__)

// Iterators
#define map_iter_ctor(...) cdc_map_iter_ctor(__VA_ARGS__)
#define map_iter_dtor(...) cdc_map_iter_dtor(__VA_ARGS__)
#define map_iter_next(...) cdc_map_iter_next(__VA_ARGS__)
#define map_iter_prev(...) cdc_map_iter_prev(__VA_ARGS__)
#define map_iter_has_next(...) cdc_map_iter_has_next(__VA_ARGS__)
#define map_iter_has_prev(...) cdc_map_iter_has_prev(__VA_ARGS__)
#define map_iter_key(...) cdc_map_iter_key(__VA_ARGS__)
#define map_iter_value(...) cdc_map_iter_value(__VA_ARGS__)
#define map_iter_key_value(...) cdc_map_iter_key_value(__VA_ARGS__)
#define map_iter_is_eq(...) cdc_map_iter_is_eq(__VA_ARGS__)
#define map_iter_type(...) cdc_map_iter_type(__VA_ARGS__)
#endif
/** @} */
#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_MAP_H
