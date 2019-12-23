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
 * @brief The cdc_hash_table is a struct and functions that provide a hash table.
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_HASH_TABLE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_HASH_TABLE_H

#include <cdcontainers/common.h>
#include <cdcontainers/hash.h>
#include <cdcontainers/status.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @defgroup cdc_hash_table
 * @brief The cdc_hash_table is a struct and functions that provide a hash table.
 * @{
 */
/**
 * @brief The cdc_hash_table_entry struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_hash_table_entry {
  struct cdc_hash_table_entry *next;
  void *key;
  void *value;
  size_t hash;
};

/**
 * @brief The cdc_hash_table is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_hash_table {
  struct cdc_hash_table_entry *tail;
  struct cdc_hash_table_entry **buckets;
  size_t bcount;
  float load_factor;
  size_t size;
  struct cdc_data_info *dinfo;
};

/**
 * @brief The cdc_hash_table_iter is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_hash_table_iter {
  struct cdc_hash_table *container;
  struct cdc_hash_table_entry *current;
};
// Base
/**
 * @defgroup cdc_hash_table_base Base
 * @{
 */
/**
 * @brief Constructs an empty hash table.
 * @param[out] t - cdc_hash_table
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_ctor(struct cdc_hash_table **t,
                                  struct cdc_data_info *info);

/**
 * @brief Constructs a hash table, initialized by an variable number of
 * pointers on cdc_pair's(first - key, and the second - value).  The last item
 * must be CDC_END.
 * @param[out] t - cdc_hash_table
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 *
 * Example:
 * @code{.c}
 * struct cdc_hash_table *table = NULL;
 * cdc_pair value1 = {CDC_FROM_INT(1), CDC_FROM_INT(2)};
 * cdc_pair value2 = {CDC_FROM_INT(3), CDC_FROM_INT(4)};
 * ...
 * if (cdc_hash_table_ctorl(&table, info, &value1, &value2, CDC_END) != CDC_STATUS_OK) {
 *   // handle error
 * }
 * @endcode
 */
enum cdc_stat cdc_hash_table_ctorl(struct cdc_hash_table **t,
                                   struct cdc_data_info *info, ...);

/**
 * @brief Constructs a hash table, initialized by args. The last item must be
 * CDC_END.
 * @param[out] t - cdc_hash_table
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_ctorv(struct cdc_hash_table **t,
                                   struct cdc_data_info *info, va_list args);

/**
 * @brief Constructs an empty hash table.
 * @param[out] t - cdc_hash_table
 * @param[in] info - cdc_data_info
 * @param[in] load_factor - maximum load factor
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_ctor1(struct cdc_hash_table **t,
                                   struct cdc_data_info *info,
                                   float load_factor);

/**
 * @brief Constructs a hash table, initialized by an variable number of
 * pointers on cdc_pair's(first - key, and the second - value).  The last item
 * must be CDC_END.
 * @param[out] t - cdc_hash_table
 * @param[in] info - cdc_data_info
 * @param[in] load_factor - maximum load factor
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_ctorl1(struct cdc_hash_table **t,
                                    struct cdc_data_info *info,
                                    float load_factor, ...);

/**
 * @brief Constructs a hash table, initialized by args. The last item must be
 * CDC_END.
 * @param[out] t - cdc_hash_table
 * @param[in] info - cdc_data_info
 * @param[in] load_factor - maximum load factor
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_ctorv1(struct cdc_hash_table **t,
                                    struct cdc_data_info *info,
                                    float load_factor, va_list args);

/**
 * @brief Destroys the hash table.
 * @param[in] t - cdc_hash_table
 */
void cdc_hash_table_dtor(struct cdc_hash_table *t);
/** @} */

// Lookup
/**
 * @defgroup cdc_hash_table_lookup Lookup
 * @{
 */
/**
 * @brief Returns a value that is mapped to a key. If the key does
 * not exist, then NULL will return.
 * @param[in] t - cdc_hash_table
 * @param[in] key - key of the element to find
 * @param[out] value - pinter to the value that is mapped to a key.
 * @return CDC_STATUS_OK if the key is found, CDC_STATUS_NOT_FOUND otherwise.
 */
enum cdc_stat cdc_hash_table_get(struct cdc_hash_table *t, void *key,
                                 void **value);

/**
 * @brief Returns the number of elements with key that compares equal to the
 * specified argument key, which is either 1 or 0 since this container does not
 * allow duplicates.
 * @param[in] t - cdc_hash_table
 * @param[in] key - key value of the elements to count
 * @return number of elements with key key, that is either 1 or 0.
 */
size_t cdc_hash_table_count(struct cdc_hash_table *t, void *key);

/**
 * @brief Finds an element with key equivalent to key.
 * @param[in] t - cdc_hash_table
 * @param[in] key - key value of the element to search for
 * @param[out] it - pointer will be recorded iterator to an element with key
 * equivalent to key. If no such element is found, past-the-end iterator is
 * returned.
 */
void cdc_hash_table_find(struct cdc_hash_table *t, void *key,
                         struct cdc_hash_table_iter *it);
/** @} */

// Capacity
/**
 * @defgroup cdc_hash_table_capacity Capacity
 * @{
 */
/**
 * @brief Returns the number of items in the hash_table.
 * @param[in] t - cdc_hash_table
 * @return the number of items in the hash_table.
 */
static inline size_t cdc_hash_table_size(struct cdc_hash_table *t)
{
  assert(t != NULL);

  return t->size;
}

/**
 * @brief Checks if the hash table has no elements.
 * @param[in] t - cdc_hash_table
 * @return true if the hash table is empty, false otherwise.
 */
static inline bool cdc_hash_table_empty(struct cdc_hash_table *t)
{
  assert(t != NULL);

  return t->size == 0;
}
/** @} */

// Modifiers
/**
 * @defgroup cdc_hash_table_modifiers Modifiers
 * @{
 */
/**
 * @brief Removes all the elements from the hash_table.
 * @param[in] t - cdc_hash_table
 */
void cdc_hash_table_clear(struct cdc_hash_table *t);

/**
 * @brief Inserts an element into the container, if the container doesn't already
 * contain an element with an equivalent key.
 * @param[in] t - cdc_hash_table
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] ret - pair consisting of an iterator to the inserted element (or to
 * the element that prevented the insertion) and a bool denoting whether the
 * insertion took place. The pointer can be equal to NULL.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_insert(struct cdc_hash_table *t, void *key,
                                    void *value, struct cdc_hash_table_iter *it,
                                    bool *inserted);

/**
 * @brief Inserts an element or assigns to the current element if the key
 * already exists.
 * @param[in] t - cdc_hash_table
 * @param[in] key - key of the element
 * @param[in] value - value of the element
 * @param[out] ret - pair. The bool component is true if the insertion took place and
 * false if the assignment took place. The iterator component is pointing at the
 * element that was inserted or updated.
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_insert_or_assign(struct cdc_hash_table *t,
                                              void *key, void *value,
                                              struct cdc_hash_table_iter *it,
                                              bool *inserted);

/**
 * @brief Removes the element (if one exists) with the key equivalent to key.
 * @param[in] t - cdc_hash_table
 * @param[in] key - key value of the elements to remove
 * @return number of elements removed.
 */
size_t cdc_hash_table_erase(struct cdc_hash_table *t, void *key);

/**
 * @brief Swaps hash_tables a and b. This operation is very fast and never fails.
 * @param[in, out] a - cdc_hash_table
 * @param[in, out] b - cdc_hash_table
 */
void cdc_hash_table_swap(struct cdc_hash_table *a, struct cdc_hash_table *b);
/** @} */

// Iterators
/**
 * @defgroup cdc_hash_table_iterators Iterators
 * @{
 */
/**
 * @brief Initializes the iterator to the beginning.
 * @param t[in] - cdc_hash_table
 * @param it[out] - cdc_hash_table_iter
 */
static inline void cdc_hash_table_begin(struct cdc_hash_table *t,
                                        struct cdc_hash_table_iter *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = t->buckets[0]->next;
}

/**
 * @brief Initializes the iterator to the end.
 * @param[in] t - cdc_hash_table
 * @param[out] it - cdc_hash_table_iter
 */
static inline void cdc_hash_table_end(struct cdc_hash_table *t,
                                      struct cdc_hash_table_iter *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = NULL;
}
/** @} */

// Hash policy
/**
 * @defgroup cdc_hash_table_hash_policy Hash policy
 * @{
 */
/**
 * @brief Returns average number of elements per bucket.
 * @param[in] t - cdc_hash_table
 * @return average number of elements per bucket.
 */
static inline float cdc_hash_table_load_factor(struct cdc_hash_table *t)
{
  assert(t != NULL);

  return (float)t->size / (float)t->bcount;
}

/**
 * @brief Returns current maximum load factor.
 * @param[in] t - cdc_hash_table
 * @return current maximum load factor.
 */
static inline float cdc_hash_table_max_load_factor(struct cdc_hash_table *t)
{
  assert(t != NULL);

  return t->load_factor;
}

/**
 * @brief Sets the maximum load factor.
 * @param[in] t - cdc_hash_table
 * @param[in] load_factor - new maximum load factor
 */
static inline void cdc_hash_table_set_max_load_factor(struct cdc_hash_table *t,
                                                      float load_factor)
{
  assert(t != NULL);

  t->load_factor = load_factor;
}

/**
 * @brief Reserves at least the specified number of buckets. This regenerates
 * the hash table.
 * @param[in] t - cdc_hash_table
 * @param[in] count - new number of buckets
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_rehash(struct cdc_hash_table *t, size_t count);

/**
 * @brief Reserves space for at least the specified number of elements. This
 * regenerates the hash table.
 * @param[in] t - cdc_hash_table
 * @param[in] count - new capacity of the container
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_hash_table_reserve(struct cdc_hash_table *t, size_t count);
/** @} */

// Bucket interface
/**
 * @defgroup cdc_hash_table_bucket_interface Bucket interface
 * @{
 */
/**
 * @brief Returns the number of buckets.
 * @param[in] t - cdc_hash_table
 * @return returns the number of buckets.
 */
static inline size_t cdc_hash_table_bucket_count(struct cdc_hash_table *t)
{
  assert(t != NULL);

  return t->bcount;
}
/** @} */

// Iterators
/**
 * @defgroup cdc_hash_table_iter
 * @brief The cdc_hash_table_iter is a struct and functions that provide a hash table iterator.
 * @{
 */
/**
 * @brief Advances the iterator to the next element in the hash table.
 * @param[in] it - iterator
 */
static inline void cdc_hash_table_iter_next(struct cdc_hash_table_iter *it)
{
  assert(it != NULL);

  it->current = it->current->next;
}

/**
 * @brief Returns true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 * @param[in] it - iterator
 * @return true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 */
static inline bool cdc_hash_table_iter_has_next(struct cdc_hash_table_iter *it)
{
  assert(it != NULL);

  return it->current != NULL;
}

/**
 * @brief Returns an item's key.
 * @param[in] it - iterator
 * @return the item's key.
 */
static inline void *cdc_hash_table_iter_key(struct cdc_hash_table_iter *it)
{
  assert(it != NULL);

  return it->current->key;
}

/**
 * @brief Returns an item's value.
 * @param[in] it - iterator
 * @return the item's value.
 */
static inline void *cdc_hash_table_iter_value(struct cdc_hash_table_iter *it)
{
  assert(it != NULL);

  return it->current->value;
}

/**
 * @brief Returns a pair, where first - key, second - value.
 * @param[in] it - iterator
 * @return pair, where first - key, second - value.
 */
static inline struct cdc_pair cdc_hash_table_iter_key_value(
    struct cdc_hash_table_iter *it)
{
  assert(it != NULL);

  struct cdc_pair pair = {it->current->key, it->current->value};
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
static inline bool cdc_hash_table_iter_is_eq(struct cdc_hash_table_iter *it1,
                                             struct cdc_hash_table_iter *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);

  return it1->container == it2->container && it1->current == it2->current;
}
/** @} */

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_hash_table hash_table_t;
typedef struct cdc_hash_table_iter hash_table_iter_t;
typedef struct cdc_pair_hash_table_iter pair_hash_table_iter_t;
typedef struct cdc_pair_hash_table_iter_bool pair_hash_table_iter_bool_t;

// Base
#define hash_table_ctor(...) cdc_hash_table_ctor(__VA_ARGS__)
#define hash_table_ctorl(...) cdc_hash_table_ctorl(__VA_ARGS__)
#define hash_table_ctorv(...) cdc_hash_table_ctorv(__VA_ARGS__)
#define hash_table_ctor1(...) cdc_hash_table_ctor1(__VA_ARGS__)
#define hash_table_ctorl1(...) cdc_hash_table_ctorl1(__VA_ARGS__)
#define hash_table_ctorv1(...) cdc_hash_table_ctorv1(__VA_ARGS__)
#define hash_table_dtor(...) cdc_hash_table_dtor(__VA_ARGS__)

// Lookup
#define hash_table_get(...) cdc_hash_table_get(__VA_ARGS__)
#define hash_table_count(...) cdc_hash_table_count(__VA_ARGS__)
#define hash_table_find(...) cdc_hash_table_find(__VA_ARGS__)

// Capacity
#define hash_table_size(...) cdc_hash_table_size(__VA_ARGS__)
#define hash_table_empty(...) cdc_hash_table_empty(__VA_ARGS__)

// Modifiers
#define hash_table_clear(...) cdc_hash_table_clear(__VA_ARGS__)
#define hash_table_insert(...) cdc_hash_table_insert(__VA_ARGS__)
#define hash_table_insert_or_assign(...) \
  cdc_hash_table_insert_or_assign(__VA_ARGS__)
#define hash_table_erase(...) cdc_hash_table_erase(__VA_ARGS__)
#define hash_table_swap(...) cdc_hash_table_swap(__VA_ARGS__)

// Iterators
#define hash_table_begin(...) cdc_hash_table_begin(__VA_ARGS__)
#define hash_table_end(...) cdc_hash_table_end(__VA_ARGS__)

// Hash policy
#define hash_table_load_factor(...) cdc_hash_table_load_factor(__VA_ARGS__)
#define hash_table_max_load_factor(...) \
  cdc_hash_table_max_load_factor(__VA_ARGS__)
#define hash_table_set_max_load_factor(...) \
  cdc_hash_table_set_max_load_factor(__VA_ARGS__)
#define hash_table_rehash(...) cdc_hash_table_rehash(__VA_ARGS__)
#define hash_table_reserve(...) cdc_hash_table_reserve(__VA_ARGS__)

// Bucket interface
#define hash_table_bucket_count(...) cdc_hash_table_bucket_count(__VA_ARGS__)

// Iterators
#define hash_table_iter_next(...) cdc_hash_table_iter_next(__VA_ARGS__)
#define hash_table_iter_has_next(...) cdc_hash_table_iter_has_next(__VA_ARGS__)
#define hash_table_iter_key(...) cdc_hash_table_iter_key(__VA_ARGS__)
#define hash_table_iter_value(...) cdc_hash_table_iter_value(__VA_ARGS__)
#define hash_table_iter_key_value(...) \
  cdc_hash_table_iter_key_value(__VA_ARGS__)
#define hash_table_iter_is_eq(...) cdc_hash_table_iter_is_eq(__VA_ARGS__)
#endif
/** @} */
#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_HASH_TABLE_H
