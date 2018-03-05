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
  * linked list
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

/**
 * @brief The cdc_list_node struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_list_node {
        struct cdc_list_node *next;
        struct cdc_list_node *prev;
        void *data;
};

/**
 * @brief The cdc_list struct
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
 * @brief The cdc_list_iterator struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_list_iter
{
        struct cdc_list *container;
        struct cdc_list_node *current;
};

/**
 * @brief The cdc_list_reverse_iterator struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_list_reverse_iter
{
        struct cdc_list *container;
        struct cdc_list_node *current;
};

/**
 * @brief For-each macro
 * @warning This is macro
 */
#define CDC_LIST_FOR_EACH(item, list) \
        for (cdc_list_node *(item) = (list->head); (item); (item) = (item)->next)

/**
 * @brief Constructs an empty list.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_list_ctor(struct cdc_list **l, struct cdc_data_info *info);

/**
 * @brief Constructs a list, initialized by an arbitrary number of pointers.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_list_ctorl(struct cdc_list **l,
                             struct cdc_data_info *info, ...);

/**
 * @brief Constructs a list, initialized by args
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_list_ctorv(struct cdc_list **l,
                             struct cdc_data_info *info, va_list args);

/**
 * @brief Destroys the list.
 */
void cdc_list_dtor(struct cdc_list *l);

// Element access
/**
 * @brief Writes to a value the item at index position in the list.
 * Index must be a valid index position in the list. Returned CDC_STATUS_OK
 * in a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_list_at(struct cdc_list *l, size_t index, void **elem);

void *cdc_list_get(struct cdc_list *l, size_t index);

/**
 * @brief Returns a pointer to the first item in the list.
 * This function assumes that the list isn't empty.
 */
static inline void *cdc_list_front(struct cdc_list *l)
{
        assert(l != NULL);
        assert(l->size > 0);

        return l->head->data;
}

/**
 * @brief Returns a pointer to the last item in the list.
 * This function assumes that the list isn't empty.
 */
static inline  void *cdc_list_back(struct cdc_list *l)
{
        assert(l != NULL);
        assert(l->size > 0);

        return l->tail->data;
}

// Iterators
/**
 * @brief Returns an iterator to the beginning
 */
static inline struct cdc_list_iter cdc_list_begin(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_iter it = {l, l->head};

        return it;
}

/**
 * @brief Returns an iterator to the end
 */
static inline struct cdc_list_iter cdc_list_end(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_iter it = {l, NULL};

        return it;
}

/**
 * @brief Returns an iterator to the beginning
 */
static inline struct cdc_list_reverse_iter cdc_list_rbegin(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_reverse_iter it = {l, l->tail};

        return it;
}

/**
 * @brief Returns an iterator to the end
 */
static inline struct cdc_list_reverse_iter cdc_list_rend(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_reverse_iter it = {l, NULL};

        return it;
}

// Capacity
/**
 * @brief Returns the number of items in the list.
 */
static inline size_t cdc_list_size(struct cdc_list *l)
{
        assert(l != NULL);

        return l->size;
}

/**
 * @brief Returns true if the list has size 0; otherwise returns false.
 */
static inline bool cdc_list_empty(struct cdc_list *l)
{
        assert(l != NULL);

        return l->size == 0;
}

// Modifiers
void cdc_list_set(struct cdc_list *l, size_t index, void *value);

/**
 * @brief Inserts value at the end of the list. Returned CDC_STATUS_OK in a
 * successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_list_push_back(struct cdc_list *l, void *value);

/**
 * @brief Removes the last item in the list. If a function has been installed
 * to delete an item, it will be called for last item. Returned CDC_STATUS_OK
 * in a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_list_pop_back(struct cdc_list *l);

/**
 * @brief Inserts value at the beginning of the list. Returned CDC_STATUS_OK
 * in a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_list_push_front(struct cdc_list *l, void *value);

/**
 * @brief Removes the first item in the list. If a function has been installed
 * to delete an item, it will be called for last item. Returned CDC_STATUS_OK
 * in a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_list_pop_front(struct cdc_list *l);

/**
 * @brief Inserts value at index position in the list. If index is 0, the value
 * is prepended to the list. If index is cdc_list_size(), the value is appended
 * to the list.
 */
enum cdc_stat cdc_list_insert(struct cdc_list *l, size_t index, void *value);

/**
 * @brief Inserts value in front of the item pointed to by the iterator before.
 */
enum cdc_stat cdc_list_iinsert(struct cdc_list_iter before, void *value);

/**
 * @brief Removes the element at index position.
 * The pointer will be written in elem. Index must be a valid index position
 * in the list. The function is not called to free memory.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_list_remove(struct cdc_list *l, size_t index, void **elem);

/**
 * @brief Removes the item associated with the iterator pos from the list.
 * The pointer will be written in elem. The function is not called to free memory.
 * Returned CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_list_iremove(struct cdc_list_iter pos, void **elem);

/**
 * @brief Removes the element at index position.
 * Index must be a valid index position in the list. Returned CDC_STATUS_OK in a
 * successful case or an excellent value indicating an error
 */
static inline enum cdc_stat cdc_list_erase(struct cdc_list *l, size_t index)
{
        return cdc_list_remove(l, index, NULL);
}

/**
 * @brief Removes the item associated with the iterator pos from the list.
 * Returned CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_list_ierase(struct cdc_list_iter pos)
{
        return cdc_list_iremove(pos, NULL);
}

/**
 * @brief Removes all the elements from the list. If a function has been
 * installed to delete an item, it will be called for each item.
 */
void cdc_list_clear(struct cdc_list *l);

/**
 * @brief Swaps lists a and b. This operation is very fast and never fails.
 */
void cdc_list_swap(struct cdc_list *a, struct cdc_list *b);

// Operations
void cdc_list_splice(struct cdc_list_iter position, struct cdc_list_iter first,
                     struct cdc_list_iter last);

void cdc_list_ssplice(struct cdc_list_iter position, struct cdc_list_iter first);

void cdc_list_lsplice(struct cdc_list_iter position, struct cdc_list *other);

void cdc_list_merge(struct cdc_list *l, struct cdc_list *other);

void cdc_list_cmerge(struct cdc_list *l, struct cdc_list *other,
                     cdc_compar_fn_t compare);

void cdc_list_erase_if(struct cdc_list *l, cdc_unary_pred_fn_t pred);

void cdc_list_reverse(struct cdc_list *l);

void cdc_list_unique(struct cdc_list *l);

void cdc_list_punique(struct cdc_list *l, cdc_binary_pred_fn_t pred);

void cdc_list_sort(struct cdc_list *l);

void cdc_list_csort(struct cdc_list *l, cdc_compar_fn_t compare);

/**
 * @brief A function cb is applied to each item of the list.
 */
void cdc_list_foreach(struct cdc_list *l, void (*cb)(void *));

// Iterators
/**
 * @brief Advances the iterator to the next item in the list and returns an
 * iterator to the new current item
 */
static inline struct cdc_list_iter cdc_list_iter_next(struct cdc_list_iter it)
{
        it.current = it.current->next;
        return it;
}

/**
 * @brief Makes the preceding item current and returns an iterator to the new
 * current item.
 */
static inline struct cdc_list_iter cdc_list_iter_prev(struct cdc_list_iter it)
{
        it.current = it.current ? it.current->prev : it.container->tail;
        return it;
}

/**
 * @brief Returns true if there is at least one item ahead of the iterator, i.e.
 * the iterator is not at the back of the container; otherwise returns false.
 */
static inline bool cdc_list_iter_has_next(struct cdc_list_iter it)
{
        return it.current->next != NULL;
}

/**
 * @brief Returns true if there is at least one item behind the iterator, i.e.
 * the iterator is not at the front of the container; otherwise returns false.
 */
static inline bool cdc_list_iter_has_prev(struct cdc_list_iter it)
{
        return it.current->prev != NULL;
}

/**
 * @brief Returns a pointer to the current item.
 */
static inline void *cdc_list_iter_data(struct cdc_list_iter it)
{
        return it.current->data;
}

/**
 * @brief Сast list reverse iterator to iterator
 */
static inline struct cdc_list_iter cdc_list_iter_from(
                struct cdc_list_reverse_iter rit)
{
        struct cdc_list_iter it = {
                rit.container, (rit.current ? rit.current->next
                                            : rit.container->head)
        };

        return it;
}

/**
 * @brief Returns false if the iterator it1 equal to the iterator it2,
 * otherwise returns false
 */
static inline bool cdc_list_iter_is_eq(struct cdc_list_iter it1,
                                       struct cdc_list_iter it2)
{
        return it1.container == it2.container && it1.current == it2.current;
}

/**
 * @brief Advances the reverse iterator to the next item in the list and
 * returns an iterator to the new current item
 */
static inline struct cdc_list_reverse_iter cdc_list_riter_next(
                struct cdc_list_reverse_iter it)
{
        it.current = it.current->prev;
        return it;
}

/**
 * @brief Makes the preceding item current and returns an iterator to the new
 * current item.
 */
static inline struct cdc_list_reverse_iter cdc_list_riter_prev(
                struct cdc_list_reverse_iter it)
{
        it.current = it.current ? it.current->next : it.container->head;
        return it;
}

/**
 * @brief Returns true if there is at least one item ahead of the reverse
 * iterator, i.e. the reverse iterator is not at the back of the container;
 * otherwise returns false.
 */
static inline bool cdc_list_riter_has_next(struct cdc_list_reverse_iter it)
{
        return it.current->prev != NULL;
}

/**
 * @brief Returns true if there is at least one item behind the reverse iterator,
 * i.e. the reverse iterator is not at the front of the container; otherwise
 * returns false.
 */
static inline bool cdc_list_riter_has_prev(struct cdc_list_reverse_iter it)
{
        return it.current->next != NULL;
}

/**
 * @brief Returns a pointer to the current item.
 */
static inline void *cdc_list_riter_data(struct cdc_list_reverse_iter it)
{
        return it.current->data;
}

/**
 * @brief Сast list iterator to reverse iterator
 */
static inline struct cdc_list_reverse_iter cdc_list_riter_from(
                struct cdc_list_iter it)
{
        struct cdc_list_reverse_iter rit = {
                it.container, (it.current ? it.current->prev
                                          : it.container->tail)
        };

        return rit;
}

/**
 * @brief Returns false if the reverse iterator rit1 equal to the reverse
 * iterator rit2, otherwise returns false
 */
static inline bool cdc_list_riter_is_eq(struct cdc_list_reverse_iter rit1,
                                        struct cdc_list_reverse_iter rit2)
{
        return rit1.container == rit2.container && rit1.current == rit2.current;
}

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_list list_t;
typedef struct cdc_list_iter list_iter_t;
typedef struct cdc_list_reverse_iter list_reverse_iter_t;

#define list_ctor(...)           cdc_list_ctor(__VA_ARGS__)
#define list_ctorl(...)          cdc_list_ctorl(__VA_ARGS__)
#define list_ctorv(...)          cdc_list_ctorv(__VA_ARGS__)
#define list_dtor(...)           cdc_list_dtor(__VA_ARGS__)

// Element access
#define list_at(...)             cdc_list_at(__VA_ARGS__)
#define list_front(...)          cdc_list_front(__VA_ARGS__)
#define list_back(...)           cdc_list_back(__VA_ARGS__)

// Iterators
#define list_begin(...)          cdc_list_begin(__VA_ARGS__)
#define list_end(...)            cdc_list_end(__VA_ARGS__)
#define list_rbegin(...)         cdc_list_rbegin(__VA_ARGS__)
#define list_rend(...)           cdc_list_rend(__VA_ARGS__)

// Capacity
#define list_empty(...)          cdc_list_empty(__VA_ARGS__)
#define list_size(...)           cdc_list_size(__VA_ARGS__)

// Modifiers
#define list_insert(...)         cdc_list_insert(__VA_ARGS__)
#define list_iinsert(...)        cdc_list_iinsert(__VA_ARGS__)
#define list_erase(...)          cdc_list_erase(__VA_ARGS__)
#define list_ierase(...)         cdc_list_ierase(__VA_ARGS__)
#define list_remove(...)         cdc_list_remove(__VA_ARGS__)
#define list_iremove(...)        cdc_list_iremove(__VA_ARGS__)
#define list_clear(...)          cdc_list_clear(__VA_ARGS__)
#define list_push_back(...)      cdc_list_push_back(__VA_ARGS__)
#define list_pop_back(...)       cdc_list_pop_back(__VA_ARGS__)
#define list_push_front(...)     cdc_list_push_front(__VA_ARGS__)
#define list_pop_front(...)      cdc_list_pop_back(__VA_ARGS__)
#define list_swap(...)           cdc_list_swap(__VA_ARGS__)

// Operations
#define list_splice(...)         cdc_list_splice(__VA_ARGS__)
#define list_ssplice(...)        cdc_list_ssplice(__VA_ARGS__)
#define list_lsplice(...)        cdc_list_lsplice(__VA_ARGS__)
#define list_merge(...)          cdc_list_merge(__VA_ARGS__)
#define list_cmerge(...)         cdc_list_cmerge(__VA_ARGS__)
#define list_erase_if(...)       cdc_list_erase_if(__VA_ARGS__)
#define list_reverse(...)        cdc_list_reverse(__VA_ARGS__)
#define list_unique(...)         cdc_list_unique(__VA_ARGS__)
#define list_punique(...)        cdc_list_punique(__VA_ARGS__)
#define list_sort(...)           cdc_list_sort(__VA_ARGS__)
#define list_csort(...)          cdc_list_csort(__VA_ARGS__)

#define list_foreach(...)        cdc_list_foreach(__VA_ARGS__)

// Iterators
#define list_iter_next(...)      cdc_list_iter_next(__VA_ARGS__)
#define list_iter_prev(...)      cdc_list_iter_prev(__VA_ARGS__)
#define list_iter_has_next(...)  cdc_list_iter_has_next(__VA_ARGS__)
#define list_iter_has_prev(...)  cdc_list_iter_has_prev(__VA_ARGS__)
#define list_iter_data(...)      cdc_list_iter_data(__VA_ARGS__)
#define list_riter_from(...)     cdc_list_riter_from(__VA_ARGS__)
#define list_iter_is_eq(...)     cdc_list_iter_is_eq(__VA_ARGS__)

#define list_riter_next(...)     cdc_list_riter_next(__VA_ARGS__)
#define list_riter_prev(...)     cdc_list_riter_prev(__VA_ARGS__)
#define list_riter_has_next(...) cdc_list_riter_has_next(__VA_ARGS__)
#define list_riter_has_prev(...) cdc_list_riter_has_prev(__VA_ARGS__)
#define list_riter_data(...)     cdc_list_riter_data(__VA_ARGS__)
#define list_iter_from(...)      cdc_list_iter_from(__VA_ARGS__)
#define list_riter_is_eq(...)    cdc_list_riter_is_eq(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H
