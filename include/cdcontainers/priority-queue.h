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
 * @brief The cdc_priority_queue is a struct and functions that provide a
 * priority queue
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>
#include <cdcontainers/tables/ipqueue.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

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
 * @brief Constructs an empty priority queue.
 * @param table - method table for a particular implementation
 * @param q - cdc_priority_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_priority_queue_ctor(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info);

/**
 * @brief Constructs a priority queue, initialized by an arbitrary number of
 * pointers. The last item must be NULL.
 * @param table - method table for a particular implementation
 * @param q - cdc_priority_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_priority_queue_ctorl(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info, ...);

/**
 * @brief Constructs a priority queue, initialized by args.
 * The last item must be NULL.
 * @param table - method table for a particular implementation
 * @param q - cdc_priority_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_priority_queue_ctorv(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info, va_list args);

/**
 * @brief Destroys the priority queue.
 * @param q - cdc_priority_queue
 */
void cdc_priority_queue_dtor(struct cdc_priority_queue *q);

// Element access
/**
 * @brief Returns a pointer to the priority queue's top item. This function
 * assumes that the priority queue isn't empty.
 * @param q - cdc_priority_queue
 * @return top item
 */
static inline void *cdc_priority_queue_top(struct cdc_priority_queue *q)
{
  assert(q != NULL);

  return q->table->top(q->container);
}

// Capacity
/**
 * @brief Returns true if the priority queue has size 0; otherwise returns
 * false.
 * @param q - cdc_priority_queue
 * @return true if the pairing heap has size 0; otherwise returns false
 */
static inline bool cdc_priority_queue_empty(struct cdc_priority_queue *q)
{
  assert(q != NULL);

  return q->table->empty(q->container);
}

/**
 * @brief Returns the number of items in the priority queue.
 * @param q - cdc_priority_queue
 * @return size
 */
static inline size_t cdc_priority_queue_size(struct cdc_priority_queue *q)
{
  assert(q != NULL);

  return q->table->size(q->container);
}

// Modifiers
/**
 * @brief Adds element elem to the priority queue.
 * @param q - cdc_priority_queue
 * @param elem
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_priority_queue_push(
    struct cdc_priority_queue *q, void *elem)
{
  assert(q != NULL);

  return q->table->push(q->container, elem);
}

/**
 * @brief Removes the top element.
 * @param q - cdc_priority_queue
 */
static inline void cdc_priority_queue_pop(struct cdc_priority_queue *q)
{
  assert(q != NULL);

  q->table->pop(q->container);
}

/**
 * @brief Swaps priority queues a and b. This operation is very fast and never
 * fails.
 * @param a - cdc_priority_queue
 * @param b - cdc_priority_queue
 */
void cdc_priority_queue_swap(struct cdc_priority_queue *a,
                             struct cdc_priority_queue *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_priority_queue priority_queue_t;

#define priority_queue_ctor(...) cdc_priority_queue_ctor(__VA_ARGS__)
#define priority_queue_ctorl(...) cdc_priority_queue_ctorl(__VA_ARGS__)
#define priority_queue_ctorv(...) cdc_priority_queue_ctorv(__VA_ARGS__)
#define priority_queue_dtor(...) cdc_priority_queue_dtor(__VA_ARGS__)

// Element access
#define priority_queue_top(...) cdc_priority_queue_top(__VA_ARGS__)

// Capacity
#define priority_queue_empty(...) cdc_priority_queue_empty(__VA_ARGS__)
#define priority_queue_size(...) cdc_priority_queue_size(__VA_ARGS__)

// Modifiers
#define priority_queue_push(...) cdc_priority_queue_push(__VA_ARGS__)
#define priority_queue_pop(...) cdc_priority_queue_pop(__VA_ARGS__)
#define priority_queue_swap(...) cdc_priority_queue_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H
