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
struct cdc_list_reverse_iterator
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
static inline struct cdc_list_reverse_iterator cdc_list_rbegin(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_reverse_iterator it = {l, l->tail};

        return it;
}

/**
 * @brief Returns an iterator to the end
 */
static inline struct cdc_list_reverse_iterator cdc_list_ernd(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_reverse_iterator it = {l, NULL};

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
 * @brief Removes the element at index position.
 * The pointer will be written in elem. Index must be a valid index position
 * in the list. The function is not called to free memory.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_list_erase(struct cdc_list *l, size_t index, void **elem);

/**
 * @brief Removes all the elements from the list. If a function has been
 * installed to delete an item, it will be called for each item. Index must be
 * a valid index position in the list
 */
void cdc_list_clear(struct cdc_list *l);

/**
 * @brief Swaps lists a and b. This operation is very fast and never fails.
 */
void cdc_list_swap(struct cdc_list *a, struct cdc_list *b);


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
        it.current = it.current->prev;
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
 * @brief Advances the reverse iterator to the next item in the list and
 * returns an iterator to the new current item
 */
static inline struct cdc_list_reverse_iterator cdc_list_riter_next(
                struct cdc_list_reverse_iterator it)
{
        it.current = it.current->prev;
        return it;
}

/**
 * @brief Makes the preceding item current and returns an iterator to the new
 * current item.
 */
static inline struct cdc_list_reverse_iterator cdc_list_riter_prev(
                struct cdc_list_reverse_iterator it)
{
        it.current = it.current->next;
        return it;
}

/**
 * @brief Returns true if there is at least one item ahead of the reverse
 * iterator, i.e. the reverse iterator is not at the back of the container;
 * otherwise returns false.
 */
static inline bool cdc_list_riter_has_next(struct cdc_list_reverse_iterator it)
{
        return it.current->prev != NULL;
}

/**
 * @brief Returns true if there is at least one item behind the reverse iterator,
 * i.e. the reverse iterator is not at the front of the container; otherwise
 * returns false.
 */
static inline bool cdc_list_riter_has_prev(struct cdc_list_reverse_iterator it)
{
        return it.current->next != NULL;
}

/**
 * @brief Returns a pointer to the current item.
 */
static inline void *cdc_list_riter_data(struct cdc_list_reverse_iterator it)
{
        return it.current->data;
}

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_list list_t;

#define list_ctor(...)       cdc_list_ctor(__VA_ARGS__)
#define list_ctorl(...)      cdc_list_ctorl(__VA_ARGS__)
#define list_ctorv(...)      cdc_list_ctorv(__VA_ARGS__)
#define list_dtor(...)       cdc_list_dtor(__VA_ARGS__)

// Element access
#define list_at(...)         cdc_list_at(__VA_ARGS__)
#define list_front(...)      cdc_list_front(__VA_ARGS__)
#define list_back(...)       cdc_list_back(__VA_ARGS__)

// Iterators
#define list_begin(...)      cdc_list_begin(__VA_ARGS__)
#define list_end(...)        cdc_list_end(__VA_ARGS__)
#define list_rbegin(...)     cdc_list_rbegin(__VA_ARGS__)
#define list_rend(...)       cdc_list_rend(__VA_ARGS__)

// Capacity
#define list_empty(...)      cdc_list_empty(__VA_ARGS__)
#define list_size(...)       cdc_list_size(__VA_ARGS__)

// Modifiers
#define list_insert(...)     cdc_list_insert(__VA_ARGS__)
#define list_erase(...)      cdc_list_erase(__VA_ARGS__)
#define list_clear(...)      cdc_list_clear(__VA_ARGS__)
#define list_push_back(...)  cdc_list_push_back(__VA_ARGS__)
#define list_pop_back(...)   cdc_list_pop_back(__VA_ARGS__)
#define list_push_front(...) cdc_list_push_front(__VA_ARGS__)
#define list_pop_front(...)  cdc_list_pop_back(__VA_ARGS__)
#define list_swap(...)       cdc_list_swap(__VA_ARGS__)

#define list_foreach(...)    cdc_list_foreach(__VA_ARGS__)

// Iterators
#define list_iter_next(...)      cdc_list_iter_next(__VA_ARGS__)
#define list_iter_prev(...)      cdc_list_iter_prev(__VA_ARGS__)
#define list_iter_has_next(...)  cdc_list_iter_has_next(__VA_ARGS__)
#define list_iter_has_prev(...)  cdc_list_iter_has_prev(__VA_ARGS__)
#define list_iter_data(...)      cdc_list_iter_data(__VA_ARGS__)

#define list_riter_next(...)     cdc_list_riter_next(__VA_ARGS__)
#define list_riter_prev(...)     cdc_list_riter_prev(__VA_ARGS__)
#define list_riter_has_next(...) cdc_list_riter_has_next(__VA_ARGS__)
#define list_riter_has_prev(...) cdc_list_riter_has_prev(__VA_ARGS__)
#define list_riter_data(...)     cdc_list_riter_data(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H
