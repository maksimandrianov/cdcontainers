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
 * @brief The cdc_list is a struct and functions that provide a doubly
 * linked list.
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @defgroup cdc_list
 * @brief The cdc_list is a struct and functions that provide a doubly
 * linked list.
 * @{
 */
/**
 * @brief The cdc_list_node is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_list_node {
  struct cdc_list_node *next;
  struct cdc_list_node *prev;
  void *data;
};

/**
 * @brief The cdc_lisе is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_list {
  struct cdc_list_node *head;
  struct cdc_list_node *tail;
  size_t size;
  struct cdc_data_info *dinfo;
};

/**
 * @brief The cdc_list_iterator is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_list_iter {
  struct cdc_list *container;
  struct cdc_list_node *current;
};

/**
 * @brief The cdc_list_riter is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_list_riter {
  struct cdc_list *container;
  struct cdc_list_node *current;
};

/**
 * @brief For-each macro.
 *
 * Example:
 * @code{.c}
 * struct cdc_list *list = NULL;
 * ...
 * CDC_LIST_FOR_EACH(node, list) {
 *   // node->data
 * }
 * @endcode
 */
#define CDC_LIST_FOR_EACH(item, list) \
  for (cdc_list_node * (item) = (list->head); (item); (item) = (item)->next)

// Base
/**
 * @defgroup cdc_list_base Base
 * @{
 */
/**
 * @brief Constructs an empty list.
 * @param[out] l - cdc_list
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_list_ctor(struct cdc_list **l, struct cdc_data_info *info);

/**
 * @brief Constructs a list, initialized by an variable number of pointers.
 * The last pointer must be CDC_END.
 * @param[out] l - cdc_list
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 *
 * Example:
 * @code{.c}
 * struct cdc_list *list = NULL;
 * if (cdc_list_ctorl(&list, NULL, CDC_FROM_INT(1),
 *                    CDC_FROM_INT(2), CDC_END) != CDC_STATUS_OK) {
 *   // handle error
 * }
 * @endcode
 */
enum cdc_stat cdc_list_ctorl(struct cdc_list **l, struct cdc_data_info *info,
                             ...);

/**
 * @brief Constructs a list, initialized by args. The last pointer must be CDC_END.
 * @param[out] l - cdc_list
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_list_ctorv(struct cdc_list **l, struct cdc_data_info *info,
                             va_list args);

/**
 * @brief Destroys the list.
 * @param[in] l - cdc_list
 */
void cdc_list_dtor(struct cdc_list *l);
/** @} */

// Element access
/**
 * @defgroup cdc_list_element_access Element access
 * @{
 */
/**
 * @brief Returns an element at index position index in the list.
 * @param[in] l - cdc_list
 * @param[in] index - index of an element to return
 * @return element from |index| position.
 */
void *cdc_list_get(struct cdc_list *l, size_t index);

/**
 * @brief Writes to pointer an element from specified position in the list.
 * Bounds checking is performed.
 * @param[in] l - cdc_list
 * @param[in] index - index of an element to write at elem
 * @param[out] elem - pointer where an element will be written
 * @return DC_STATUS_OK in a successful case or CDC_STATUS_OUT_OF_RANGE if the
 * index is incorrect.
 */
enum cdc_stat cdc_list_at(struct cdc_list *l, size_t index, void **elem);

/**
 * @brief Returns a first element in the list.
 * @param[in] l - cdc_list
 * @return first element in the list.
 */
static inline void *cdc_list_front(struct cdc_list *l)
{
  assert(l != NULL);
  assert(l->size > 0);

  return l->head->data;
}

/**
 * @brief Returns a last element in the list.
 * @param[in] l - cdc_list
 * @return last element in the list.
 */
static inline void *cdc_list_back(struct cdc_list *l)
{
  assert(l != NULL);
  assert(l->size > 0);

  return l->tail->data;
}
/** @} */

// Capacity
/**
 * @defgroup cdc_list_capacity Capacity
 * @{
 */
/**
 * @brief Returns the number of elements in the list.
 * @param[in] l - cdc_list
 * @return the number of elements in the list.
 */
static inline size_t cdc_list_size(struct cdc_list *l)
{
  assert(l != NULL);

  return l->size;
}

/**
 * @brief Checks if the list has no elements.
 * @param[in] l - cdc_list
 * @return true if the list is empty, false otherwise.
 */
static inline bool cdc_list_empty(struct cdc_list *l)
{
  assert(l != NULL);

  return l->size == 0;
}
/** @} */

// Modifiers
/**
 * @defgroup cdc_list_modifiers Modifiers
 * @{
 */
/**
 * @brief Sets an element at index position to the value. The function is not
 * called to free memory.
 * @param[in] l - cdc_list
 * @param[in] index - index position where the value will be written
 * @param[in] value - value
 */
void cdc_list_set(struct cdc_list *l, size_t index, void *value);

/**
 * @brief Inserts an element at the end of the list.
 * @param[in] l - cdc_list
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_list_push_back(struct cdc_list *l, void *value);

/**
 * @brief Removes a last element in the list.
 * @param[in] l - cdc_list
 */
void cdc_list_pop_back(struct cdc_list *l);

/**
 * @brief Inserts an element at the beginning of the list.
 * @param[in] l - cdc_list
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_list_push_front(struct cdc_list *l, void *value);

/**
 * @brief Removes a first element in the list.
 * @param[in] l - cdc_list
 */
void cdc_list_pop_front(struct cdc_list *l);

/**
 * @brief Inserts an element at |index| position in the list. If index is 0, the
 * value is prepended to the list. If index is cdc_list_size(), the value is
 * appended to the list.
 * @param[in] l - cdc_list
 * @param[in] index - index position where an element will be inserted
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_list_insert(struct cdc_list *l, size_t index, void *value);

/**
 * @brief Inserts an element in front of the item pointed to by the iterator before.
 * @param[in] before - iterator position before which an element will be inserted
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_list_iinsert(struct cdc_list_iter *before, void *value);

/**
 * @brief Removes an element at index position in the circular arrray.
 * @param[in] l - cdc_list
 * @param[in] index - index position where an element will be removed
 */
void cdc_list_erase(struct cdc_list *l, size_t index);

/**
 * @brief Removes an element associated with the iterator pos from the list.
 * @param[in] pos - iterator
 */
void cdc_list_ierase(struct cdc_list_iter *pos);

/**
 * @brief Removes all the elements from the list.
 * @param[in] l - cdc_list
 */
void cdc_list_clear(struct cdc_list *l);

/**
 * @brief Swaps lists a and b. This operation is very fast and never
 * fails.
 * @param[in, out] a - cdc_list
 * @param[in, out] b - cdc_list
 */
void cdc_list_swap(struct cdc_list *a, struct cdc_list *b);
/** @} */

// Operations
/**
 * @defgroup cdc_list_operations Operations
 * @{
 */
/**
 * @brief Transfers elements from one container, iterators (first, last] to
 * another container at position before iterator position.
 * @param[in] position - iterator before which the content will be inserted
 * @param[in] first, last - range of elements to transfer from other
 */
void cdc_list_splice(struct cdc_list_iter *position,
                     struct cdc_list_iter *first, struct cdc_list_iter *last);

/**
 * @brief Transfers elements from one container, iterators (first, end] to
 * another container at position before iterator position.
 * @param[in] position - iterator before which the content will be inserted
 * @param[in] first - beginning of the range from which elements will be transferred
 */
void cdc_list_ssplice(struct cdc_list_iter *position,
                      struct cdc_list_iter *first);

/**
 * @brief Transfers all elements from container other to another container at
 * position before iterator position.
 * @param[in] position - iterator before which the content will be inserted
 * @param[im] other - cdc_list
 */
void cdc_list_lsplice(struct cdc_list_iter *position, struct cdc_list *other);

/**
 * @brief Merges two sorted lists into one. The lists should be sorted into
 * ascending order.
 * @param[in] l - cdc_list
 * @param[in] other - another cdc_list to merge
 */
void cdc_list_merge(struct cdc_list *l, struct cdc_list *other);

/**
 * @brief Merges two sorted lists into one. The lists should be sorted.
 * @param[in] l - cdc_list
 * @param[in] other - another cdc_list to merge
 * @param[in] compare - comparison function
 */
void cdc_list_cmerge(struct cdc_list *l, struct cdc_list *other,
                     cdc_binary_pred_fn_t compare);
/**
 * @brief Removes from the container all elements for which predicate pred
 * returns true.
 * @param[in] l - cdc_list
 * @param[in] pred - unary predicate which returns ​true if the element should be
 * removed
 */
void cdc_list_erase_if(struct cdc_list *l, cdc_unary_pred_fn_t pred);

/**
 * @brief Reverses the order of elements in the container.
 * @param[in] l - cdc_list
 */
void cdc_list_reverse(struct cdc_list *l);

/**
 * @brief Removes all consecutive duplicate elements from the container. Only
 * the first element in each group of equal elements is left.
 * @param[in] l - cdc_list
 */
void cdc_list_unique(struct cdc_list *l);

/**
 * @brief Removes all consecutive duplicate elements from the container. Only
 * the first element in each group of equal elements is left.
 * @param[in] l - cdc_list
 * @param[in] pred - binary predicate which returns ​true if the elements should
 * be treated as equal.
 */
void cdc_list_punique(struct cdc_list *l, cdc_binary_pred_fn_t pred);

/**
 * @brief Sorts elements in ascending order.
 * @param[in] l - cdc_list
 */
void cdc_list_sort(struct cdc_list *l);

/**
 * @brief Sorts elements in ascending order.
 * @param[in] l - cdc_list
 * @param[in] compare - comparison function object which returns ​true if the
 * first argument is less than (i.e. is ordered before) the second.
 */
void cdc_list_csort(struct cdc_list *l, cdc_binary_pred_fn_t compare);

/**
 * @brief A function |cb| is applied to each item of the list.
 */
void cdc_list_foreach(struct cdc_list *l, void (*cb)(void *));
/** @} */

// Iterators
/**
 * @defgroup cdc_avl_iterators Iterators
 * @{
 */
/**
 * @brief Initializes the iterator to the beginning.
 * @param[in] l - cdc_list
 * @param[out] it - cdc_list_iter
 */
static inline void cdc_list_begin(struct cdc_list *l, struct cdc_list_iter *it)
{
  assert(l != NULL);
  assert(it != NULL);

  it->container = l;
  it->current = l->head;
}

/**
 * @brief Initializes the iterator to the end.
 * @param[in] l - cdc_list
 * @param[out] it - cdc_list_iter
 */
static inline void cdc_list_end(struct cdc_list *l, struct cdc_list_iter *it)
{
  assert(l != NULL);
  assert(it != NULL);

  it->container = l;
  it->current = NULL;
}

/**
 * @brief Initializes the reverse iterator to the beginning.
 * @param[in] l - cdc_list
 * @param[out] it - cdc_list_riter
 */
static inline void cdc_list_rbegin(struct cdc_list *l,
                                   struct cdc_list_riter *it)
{
  assert(l != NULL);
  assert(it != NULL);

  it->container = l;
  it->current = l->tail;
}

/**
 * @brief Initializes the reverse iterator to the end.
 * @param[in] l - cdc_list
 * @param[out] it - cdc_list_riter
 */
static inline void cdc_list_rend(struct cdc_list *l, struct cdc_list_riter *it)
{
  assert(l != NULL);
  assert(it != NULL);

  it->container = l;
  it->current = NULL;
}
/** @} */

// Iterators
/**
 * @defgroup cdc_list_iter
 * @brief The cdc_list_iter is a struct and functions that provide a list iterator.
 * @{
 */
/**
 * @brief Advances the iterator to the next element in the list.
 * @param[in] it - iterator
 */
static inline void cdc_list_iter_next(struct cdc_list_iter *it)
{
  assert(it != NULL);

  it->current = it->current->next;
}

/**
 * @brief Advances the iterator to the previous element in the list.
 * @param[in] it - iterator
 */
static inline void cdc_list_iter_prev(struct cdc_list_iter *it)
{
  assert(it != NULL);

  it->current = it->current ? it->current->prev : it->container->tail;
}

/**
 * @brief Returns true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 * @param[in] it - iterator
 * @return true if there is at least one element ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 */
static inline bool cdc_list_iter_has_next(struct cdc_list_iter *it)
{
  assert(it != NULL);

  return it->current->next != NULL;
}

/**
 * @brief Returns true if there is at least one element behind the iterator, i.e.
 * the iterator is not at the front of the container; otherwise returns false.
 * @param[in] it - iterator
 * @return true if there is at least one element behind the iterator, i.e.
 * the iterator is not at the front of the container; otherwise returns false.
 */
static inline bool cdc_list_iter_has_prev(struct cdc_list_iter *it)
{
  assert(it != NULL);

  return it->current->prev != NULL;
}

/**
 * @brief Returns a current element.
 * @param[in] it - iterator
 */
static inline void *cdc_list_iter_data(struct cdc_list_iter *it)
{
  assert(it != NULL);

  return it->current->data;
}

/**
 * @brief Сasts reverse iterator to iterator.
 * @param[in] rit - reverse iterator
 * @param[out] it - iterator
 */
static inline void cdc_list_iter_from(struct cdc_list_riter *rit,
                                      struct cdc_list_iter *it)
{
  assert(rit != NULL);
  assert(it != NULL);

  it->container = rit->container;
  it->current = rit->current ? rit->current->next : rit->container->head;
}

/**
 * @brief Returns false if the iterator |it1| equal to the iterator |it2|,
 * otherwise returns false.
 * @param[in] it1 - iterator
 * @param[in] it2 - iterator
 * @return false if the iterator |it1| equal to the iterator |it2|,
 * otherwise returns false.
 */
static inline bool cdc_list_iter_is_eq(struct cdc_list_iter *it1,
                                       struct cdc_list_iter *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);

  return it1->container == it2->container && it1->current == it2->current;
}

/**
 * @brief Advances the reverse iterator to the next element in the list.
 * @param[in] it - reverse iterator
 */
static inline void cdc_list_riter_next(struct cdc_list_riter *it)
{
  assert(it != NULL);

  it->current = it->current->prev;
}

/**
 * @brief Advances the reverse iterator to the previous element in the list.
 * @param[in] it - reverse iterator
 */
static inline void cdc_list_riter_prev(struct cdc_list_riter *it)
{
  assert(it != NULL);

  it->current = it->current ? it->current->next : it->container->head;
}

/**
 * @brief Returns true if there is at least one item ahead of the reverse
 * iterator, i.e. the reverse iterator is not at the back of the container.
 * otherwise returns false.
 * @param[in] it - reverse iterator
 * @return true if there is at least one item ahead of the reverse
 * iterator, i.e. the reverse iterator is not at the back of the container.
 */
static inline bool cdc_list_riter_has_next(struct cdc_list_riter *it)
{
  assert(it != NULL);

  return it->current->prev != NULL;
}

/**
 * @brief Returns true if there is at least one item behind the reverse
 * iterator, i.e. the reverse iterator is not at the front of the container;
 * otherwise returns false.
 * @param[in] it - reverse iterator
 * @return true if there is at least one item behind the reverse
 * iterator, i.e. the reverse iterator is not at the front of the container;
 * otherwise returns false.
 */
static inline bool cdc_list_riter_has_prev(struct cdc_list_riter *it)
{
  assert(it != NULL);

  return it->current->next != NULL;
}

/**
 * @brief Returns a current element.
 * @param[in] it - reverse iterator
 */
static inline void *cdc_list_riter_data(struct cdc_list_riter *it)
{
  assert(it != NULL);

  return it->current->data;
}

/**
 * @brief Сasts iterator to reverse iterator
 * @param[in] it - iterator
 * @param[out] rit - reverse iterator
 */
static inline void cdc_list_riter_from(struct cdc_list_iter *it,
                                       struct cdc_list_riter *rit)
{
  assert(it != NULL);
  assert(rit != NULL);

  rit->container = it->container;
  rit->current = it->current ? it->current->prev : it->container->tail;
}

/**
 * @brief Returns false if the reverse iterator |rit1| equal to the reverse
 * iterator |rit2|, otherwise returns false.
 * @param[in] rit1 - reverse iterator
 * @param[in] rit2 - reverse iterator
 */
static inline bool cdc_list_riter_is_eq(struct cdc_list_riter *rit1,
                                        struct cdc_list_riter *rit2)
{
  assert(rit1 != NULL);
  assert(rit2 != NULL);

  return rit1->container == rit2->container && rit1->current == rit2->current;
}
/** @} */

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_list list_t;
typedef struct cdc_list_iter list_iter_t;
typedef struct cdc_list_riter list_riter_t;

// Base
#define list_ctor(...) cdc_list_ctor(__VA_ARGS__)
#define list_ctorl(...) cdc_list_ctorl(__VA_ARGS__)
#define list_ctorv(...) cdc_list_ctorv(__VA_ARGS__)
#define list_dtor(...) cdc_list_dtor(__VA_ARGS__)

// Element access
#define list_at(...) cdc_list_at(__VA_ARGS__)
#define list_front(...) cdc_list_front(__VA_ARGS__)
#define list_back(...) cdc_list_back(__VA_ARGS__)

// Capacity
#define list_empty(...) cdc_list_empty(__VA_ARGS__)
#define list_size(...) cdc_list_size(__VA_ARGS__)

// Modifiers
#define list_insert(...) cdc_list_insert(__VA_ARGS__)
#define list_iinsert(...) cdc_list_iinsert(__VA_ARGS__)
#define list_erase(...) cdc_list_erase(__VA_ARGS__)
#define list_ierase(...) cdc_list_ierase(__VA_ARGS__)
#define list_clear(...) cdc_list_clear(__VA_ARGS__)
#define list_push_back(...) cdc_list_push_back(__VA_ARGS__)
#define list_pop_back(...) cdc_list_pop_back(__VA_ARGS__)
#define list_push_front(...) cdc_list_push_front(__VA_ARGS__)
#define list_pop_front(...) cdc_list_pop_back(__VA_ARGS__)
#define list_swap(...) cdc_list_swap(__VA_ARGS__)

// Operations
#define list_splice(...) cdc_list_splice(__VA_ARGS__)
#define list_ssplice(...) cdc_list_ssplice(__VA_ARGS__)
#define list_lsplice(...) cdc_list_lsplice(__VA_ARGS__)
#define list_merge(...) cdc_list_merge(__VA_ARGS__)
#define list_cmerge(...) cdc_list_cmerge(__VA_ARGS__)
#define list_erase_if(...) cdc_list_erase_if(__VA_ARGS__)
#define list_reverse(...) cdc_list_reverse(__VA_ARGS__)
#define list_unique(...) cdc_list_unique(__VA_ARGS__)
#define list_punique(...) cdc_list_punique(__VA_ARGS__)
#define list_sort(...) cdc_list_sort(__VA_ARGS__)
#define list_csort(...) cdc_list_csort(__VA_ARGS__)

#define list_foreach(...) cdc_list_foreach(__VA_ARGS__)

// Iterators
#define list_begin(...) cdc_list_begin(__VA_ARGS__)
#define list_end(...) cdc_list_end(__VA_ARGS__)
#define list_rbegin(...) cdc_list_rbegin(__VA_ARGS__)
#define list_rend(...) cdc_list_rend(__VA_ARGS__)

// Iterators
#define list_iter_next(...) cdc_list_iter_next(__VA_ARGS__)
#define list_iter_prev(...) cdc_list_iter_prev(__VA_ARGS__)
#define list_iter_has_next(...) cdc_list_iter_has_next(__VA_ARGS__)
#define list_iter_has_prev(...) cdc_list_iter_has_prev(__VA_ARGS__)
#define list_iter_data(...) cdc_list_iter_data(__VA_ARGS__)
#define list_riter_from(...) cdc_list_riter_from(__VA_ARGS__)
#define list_iter_is_eq(...) cdc_list_iter_is_eq(__VA_ARGS__)

#define list_riter_next(...) cdc_list_riter_next(__VA_ARGS__)
#define list_riter_prev(...) cdc_list_riter_prev(__VA_ARGS__)
#define list_riter_has_next(...) cdc_list_riter_has_next(__VA_ARGS__)
#define list_riter_has_prev(...) cdc_list_riter_has_prev(__VA_ARGS__)
#define list_riter_data(...) cdc_list_riter_data(__VA_ARGS__)
#define list_iter_from(...) cdc_list_iter_from(__VA_ARGS__)
#define list_riter_is_eq(...) cdc_list_riter_is_eq(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H
