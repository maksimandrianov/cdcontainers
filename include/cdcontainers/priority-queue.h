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
  * @brief The cdc_priority_queue is a struct and functions that provide a priority queue
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

/**
 * @brief The cdc_priority_queue_table struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_priority_queue_table {
        enum cdc_stat (*ctor)(void **cntr, struct cdc_data_info *info,
                              cdc_compar_fn_t compar);
        enum cdc_stat (*ctorv)(void **cntr, struct cdc_data_info *info,
                               cdc_compar_fn_t compar, va_list args);
        void (*dtor)(void *cntr);
        void *(*top)(void *cntr);
        bool (*empty)(void *cntr);
        size_t (*size)(void *cntr);
        enum cdc_stat (*push)(void *cntr, void *elem);
        enum cdc_stat (*pop)(void *cntr);
};

/**
 * @brief The cdc_priority_queue struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_priority_queue {
        void *container;
        const struct cdc_priority_queue_table *table;
};

/**
 * @brief Table for the priority queue based on the heap
 */
extern const void *cdc_priority_queueh_table;

/**
 * @brief Constructs an empty priority queue.
 * The function compar specifies the ordering of items.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_priority_queue_ctor(const struct cdc_priority_queue_table *table,
                                      struct cdc_priority_queue **q,
                                      struct cdc_data_info *info,
                                      cdc_compar_fn_t compar);

/**
 * @brief Constructs a priority queue, initialized by an arbitrary number of pointers.
 * The function compar specifies the ordering of items.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_priority_queue_ctorl(const struct cdc_priority_queue_table *table,
                                       struct cdc_priority_queue **q,
                                       struct cdc_data_info *info,
                                       cdc_compar_fn_t compar, ...);

/**
 * @brief Constructs a priority queue, initialized by args.
 * The function compar specifies the ordering of items.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_priority_queue_ctorv(const struct cdc_priority_queue_table *table,
                                       struct cdc_priority_queue **q,
                                       struct cdc_data_info *info,
                                       cdc_compar_fn_t compar, va_list args);

/**
 * @brief Constructs an empty priority queue based on heap.
 * The function compar specifies the ordering of items.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
static inline enum cdc_stat cdc_priority_queueh_ctor(struct cdc_priority_queue **q,
                                                     struct cdc_data_info *info,
                                                     cdc_compar_fn_t compar)
{
        assert(q != NULL);

        return cdc_priority_queue_ctor(cdc_priority_queueh_table, q, info,
                                       compar);
}

/**
 * @brief Constructs a priority queue based on heap, initialized by an arbitrary
 * number of pointers.
 * The function compar specifies the ordering of items.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_priority_queueh_ctorl(struct cdc_priority_queue **q,
                                        struct cdc_data_info *info,
                                        cdc_compar_fn_t compar, ...);

/**
 * @brief Constructs a priority queue based on heap, initialized by args.
 * The function compar specifies the ordering of items.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
static inline enum cdc_stat cdc_priority_queueh_ctorv(struct cdc_priority_queue **q,
                                                      struct cdc_data_info *info,
                                                      cdc_compar_fn_t compar,
                                                      va_list args)
{
        assert(q != NULL);

        return cdc_priority_queue_ctorv(cdc_priority_queueh_table, q, info,
                                        compar, args);
}

/**
 * @brief Destroys the priority queue.
 */
void cdc_priority_queue_dtor(struct cdc_priority_queue *q);

// Element access
/**
 * @brief Returns a pointer to the priority queue's top item. This function assumes
 * that the priority queue isn't empty.
 */
static inline void *cdc_priority_queue_top(struct cdc_priority_queue *q)
{
        assert(q != NULL);

        return q->table->top(q->container);
}

// Capacity
/**
 * @brief Returns true if the priority queue has size 0; otherwise returns false.
 */
static inline bool cdc_priority_queue_empty(struct cdc_priority_queue *q)
{
        assert(q != NULL);

        return q->table->empty(q->container);
}

/**
 * @brief Returns the number of items in the priority queue.
 */
static inline size_t cdc_priority_queue_size(struct cdc_priority_queue *q)
{
        assert(q != NULL);

        return q->table->size(q->container);
}

// Modifiers
/**
 * @brief Adds element elem to the priority queue. Returned CDC_STATUS_OK in a
 * successful case or an excellent value indicating an error
 */
static inline enum cdc_stat cdc_priority_queue_push(struct cdc_priority_queue *q,
                                                    void *elem)
{
        assert(q != NULL);

        return q->table->push(q->container, elem);
}

/**
 * @briefReturns a pointer to the stack's top item.
 * This function assumes that the priority queue isn't empty. Returned CDC_STATUS_OK in
 * a successful case or an excellent value indicating an error
 */
static inline enum cdc_stat cdc_priority_queue_pop(struct cdc_priority_queue *q)
{
        assert(q != NULL);

        return q->table->pop(q->container);
}

/**
 * @brief Swaps priority queues a and b. This operation is very fast and never fails.
 */
void cdc_priority_queue_swap(struct cdc_priority_queue *a,
                             struct cdc_priority_queue *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_priority_queue priority_queue_t;

#define priority_queue_ctor(...)  cdc_priority_queue_ctor(__VA_ARGS__)
#define priority_queue_ctorl(...) cdc_priority_queue_ctorl(__VA_ARGS__)
#define priority_queue_ctorv(...) cdc_priority_queue_ctorv(__VA_ARGS__)
#define priority_queue_dtor(...)  cdc_priority_queue_dtor(__VA_ARGS__)

// Element access
#define priority_queue_top(...)   cdc_priority_queue_top(__VA_ARGS__)

// Capacity
#define priority_queue_empty(...) cdc_priority_queue_empty(__VA_ARGS__)
#define priority_queue_size(...)  cdc_priority_queue_size(__VA_ARGS__)

// Modifiers
#define priority_queue_push(...)  cdc_priority_queue_push(__VA_ARGS__)
#define priority_queue_pop(...)   cdc_priority_queue_pop(__VA_ARGS__)
#define priority_queue_swap(...)  cdc_priority_queue_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H
