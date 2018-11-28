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
  * @brief The cdc_queue is a struct and functions that provide a queue
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>
#include <cdcontainers/interfaces/isequence.h>

/**
 * @brief The cdc_queue struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_queue {
  void *container;
  const struct cdc_sequence_table *table;
};

/**
 * @brief Constructs an empty queue.
 * @param table - method table for a particular implementation
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_queue_ctor(const struct cdc_sequence_table *table,
                             struct cdc_queue **q, struct cdc_data_info *info);

/**
 * @brief Constructs a queue, initialized by an arbitrary number of pointers.
 * The last item must be NULL.
 * @param table - method table for a particular implementation
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_queue_ctorl(const struct cdc_sequence_table *table,
                              struct cdc_queue **q,
                              struct cdc_data_info *info, ...);

/**
 * @brief Constructs a queue, initialized by args
 * The last item must be NULL.
 * @param table - method table for a particular implementation
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_queue_ctorv(const struct cdc_sequence_table *table,
                              struct cdc_queue **q,
                              struct cdc_data_info *info, va_list args);

/**
 * @brief Constructs an empty queue based on list.
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_queuel_ctor(struct cdc_queue **q,
                                            struct cdc_data_info *info)
{
  assert(q != NULL);

  return cdc_queue_ctor(cdc_seq_list, q, info);
}

/**
 * @brief Constructs a queue based on list, initialized by an
 * arbitrary number of pointers. The last item must be NULL.
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_queuel_ctorl(struct cdc_queue **q,
                               struct cdc_data_info *info, ...);

/**
 * @brief Constructs a queue based on list, initialized by args
 * The last item must be NULL.
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_queuel_ctorv(struct cdc_queue **q,
                                             struct cdc_data_info *info,
                                             va_list args)
{
  assert(q != NULL);

  return cdc_queue_ctorv(cdc_seq_list, q, info, args);
}

/**
 * @brief Constructs an empty queue based on deque.
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_queued_ctor(struct cdc_queue **q,
                                            struct cdc_data_info *info)
{
  assert(q != NULL);

  return cdc_queue_ctor(cdc_seq_deque, q, info);
}

/**
 * @brief Constructs a queue based on deque, initialized by an
 * arbitrary number of pointers. The last item must be NULL.
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_queued_ctorl(struct cdc_queue **q,
                               struct cdc_data_info *info, ...);

/**
 * @brief Constructs a queue based on deque, initialized by args
 * The last item must be NULL.
 * @param q - cdc_queue
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_queued_ctorv(struct cdc_queue **q,
                                             struct cdc_data_info *info,
                                             va_list args)
{
  assert(q != NULL);

  return cdc_queue_ctorv(cdc_seq_deque, q, info, args);
}

/**
 * @brief Destroys the queue.
 * @param q - cdc_queue
 */
void cdc_queue_dtor(struct cdc_queue *q);

// Element access
/**
 * @brief Returns pointer to the first element in the queue. This function assumes
 * that the queue isn't empty.
 * @param q - cdc_queue
 * @return pointer to the first element in the queue
 */
static inline void *cdc_queue_front(struct cdc_queue *q)
{
  assert(q != NULL);

  return q->table->front(q->container);
}


/**
 * @brief Returns pointer to the last element in the queue. This function assumes
 * that the queue isn't empty.
 * @param q - cdc_queue
 * @return pointer to the last element in the queue
 */
static inline void *cdc_queue_back(struct cdc_queue *q)
{
  assert(q != NULL);

  return q->table->back(q->container);
}

// Capacity
/**
 * @brief Returns true if the queue has size 0; otherwise returns false.
 * @param q - cdc_queue
 * @return true if the list has size 0; otherwise returns false
 */
static inline bool cdc_queue_empty(struct cdc_queue *q)
{
  assert(q != NULL);

  return q->table->empty(q->container);
}

/**
 * @brief Returns the number of items in the queue.
 * @param q - cdc_queue
 * @return size
 */
static inline size_t cdc_queue_size(struct cdc_queue *q)
{
  assert(q != NULL);

  return q->table->size(q->container);
}

// Modifiers
/**
 * @brief Adds value elem to the tail of the queue.
 * @param q - cdc_queue
 * @param elem
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_queue_push(struct cdc_queue *q, void *elem)
{
  assert(q != NULL);

  return q->table->push_back(q->container, elem);
}

/**
 * @brief Removes the head item in the queue. This function assumes that the
 * queue isn't empty.
 * @param q - cdc_queue
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_queue_pop(struct cdc_queue *q)
{
  assert(q != NULL);

  return q->table->pop_front(q->container);
}

/**
 * @brief Swaps queues a and b. This operation is very fast and never fails.
 * @param a - cdc_queue
 * @param b - cdc_queue
 */
void cdc_queue_swap(struct cdc_queue *a, struct cdc_queue *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_queue queue_t;

#define queue_ctor(...)   cdc_queue_ctor(__VA_ARGS__)
#define queue_ctorl(...)  cdc_queue_ctorl(__VA_ARGS__)
#define queue_ctorv(...)  cdc_queue_ctorv(__VA_ARGS__)
#define queue_dtor(...)   cdc_queue_dtor(__VA_ARGS__)

// Element access
#define queue_front(...)  cdc_queue_front(__VA_ARGS__)
#define queue_back(...)   cdc_queue_back(__VA_ARGS__)

// Capacity
#define queue_empty(...)  cdc_queue_empty(__VA_ARGS__)
#define queue_size(...)   cdc_queue_size(__VA_ARGS__)

// Modifiers
#define queue_push(...)   cdc_queue_push(__VA_ARGS__)
#define queue_pop(...)    cdc_queue_pop(__VA_ARGS__)
#define queue_swap(...)   cdc_queue_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUE_H
