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
 * @brief The cdc_deque is a struct and functions that provide a
 * double-ended queue
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief The cdc_deque struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_deque {
  void **buffer;
  size_t head;
  size_t tail;
  size_t size;
  size_t capacity;
  struct cdc_data_info *dinfo;
};

/**
 * @brief Constructs an empty deque.
 * @param d - cdc_deque
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_deque_ctor(struct cdc_deque **d, struct cdc_data_info *info);

/**
 * @brief Constructs a deque, initialized by an arbitrary number of pointers.
 * The last item must be NULL.
 * @param d - cdc_deque
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_deque_ctorl(struct cdc_deque **d, struct cdc_data_info *info,
                              ...);

/**
 * @brief Constructs a deque, initialized by args. The last item must be NULL.
 * @param d - cdc_deque
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_deque_ctorv(struct cdc_deque **d, struct cdc_data_info *info,
                              va_list args);

/**
 * @brief Destroys the deque.
 * @param d - cdc_deque
 */
void cdc_deque_dtor(struct cdc_deque *d);

// Element access
/**
 * @brief Returns the item at index position index in the deque. Index must be a
 * valid index position in the deque.
 * @param d - cdc_deque
 * @param index - index of the item to return
 * @return item at the index position
 */
static inline void *cdc_deque_get(struct cdc_deque *d, size_t index)
{
  assert(d != NULL);
  assert(index < d->size);

  size_t real_index = (d->head + index) & (d->capacity - 1);
  return d->buffer[real_index];
}

/**
 * @brief Writes to a elem the item at index position in the deque.
 * @param d - cdc_deque
 * @param index - index of the item to write at elem
 * @param elem - pointer where the item will be written
 * @return DC_STATUS_OK in a successful case or CDC_STATUS_OUT_OF_RANGE if the
 * index is incorrect
 */
enum cdc_stat cdc_deque_at(struct cdc_deque *d, size_t index, void **elem);

/**
 * @brief Returns a pointer to the first item in the deque.
 * This function assumes that the deque isn't empty.
 * @param d - cdc_deque
 * @return pointer to the first item in the deque
 */
static inline void *cdc_deque_front(struct cdc_deque *d)
{
  assert(d != NULL);
  assert(d->size > 0);

  return d->buffer[d->head];
}

/**
 * @brief Returns a pointer to the last item in the deque.
 * This function assumes that the deque isn't empty.
 * @param d - cdc_deque
 * @return pointer to the last item in the deque
 */
static inline void *cdc_deque_back(struct cdc_deque *d)
{
  assert(d != NULL);
  assert(d->size > 0);

  size_t real_index = (d->tail - 1 + d->capacity) & (d->capacity - 1);
  return d->buffer[real_index];
}

// Capacity
/**
 * @brief Returns true if the deque has size 0; otherwise returns false.
 * @param d - cdc_deque
 * @return true if the deque has size 0; otherwise returns false
 */
static inline bool cdc_deque_empty(struct cdc_deque *d)
{
  assert(d != NULL);

  return d->size == 0;
}

/**
 * @brief Returns the number of items in the deque.
 * @param d - cdc_deque
 * @return size
 */
static inline size_t cdc_deque_size(struct cdc_deque *d)
{
  assert(d != NULL);

  return d->size;
}

// Modifiers
/**
 * @brief Sets the deque at index position to the value. The function is not
 * called to free memory.
 * @param d - cdc_deque
 * @param index - index position where the value will be written
 * @param value
 */
static inline void cdc_deque_set(struct cdc_deque *d, size_t index, void *value)
{
  assert(d != NULL);
  assert(index < d->size);

  size_t real_index = (d->head + index) & (d->capacity - 1);
  d->buffer[real_index] = value;
}

/**
 * @brief Inserts value at index position in the deque. If index is 0, the value
 * is prepended to the deque. If index is cdc_deque_size(), the value is
 * appended to the deque.
 * @param d - cdc_deque
 * @param index - index position where the value will be inserted
 * @param value
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_deque_insert(struct cdc_deque *d, size_t index, void *value);

/**
 * @brief Removes the element at index position. Index must be a valid index
 * position in the deque.
 * @param d - cdc_deque
 * @param index - index position where the item will be removed
 */
void cdc_deque_erase(struct cdc_deque *d, size_t index);

/**
 * @brief Removes all the elements from the deque.
 * @param d - cdc_deque
 */
void cdc_deque_clear(struct cdc_deque *d);

/**
 * @brief Inserts value at the end of the deque.
 * @param d - cdc_deque
 * @param value
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_deque_push_back(struct cdc_deque *d, void *value);

/**
 * @brief Removes the last item in the deque.
 * @param d - cdc_deque
 */
void cdc_deque_pop_back(struct cdc_deque *d);

/**
 * @brief Inserts value at the beginning of the deque.
 * @param d - cdc_deque
 * @param value
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_deque_push_front(struct cdc_deque *d, void *value);

/**
 * @brief Removes the first item in the deque.
 * @param d - cdc_deque
 */
void cdc_deque_pop_front(struct cdc_deque *d);

/**
 * @brief Swaps deques a and b. This operation is very fast and never fails.
 * @param a - cdc_deque
 * @param b - cdc_deque
 */
void cdc_deque_swap(struct cdc_deque *a, struct cdc_deque *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_deque deque_t;

#define deque_ctor(...) cdc_deque_ctor(__VA_ARGS__)
#define deque_ctorl(...) cdc_deque_ctorl(__VA_ARGS__)
#define deque_ctorv(...) cdc_deque_ctorv(__VA_ARGS__)
#define deque_dtor(...) cdc_deque_dtor(__VA_ARGS__)

// Element access
#define deque_get(...) cdc_deque_get(__VA_ARGS__)
#define deque_at(...) cdc_deque_at(__VA_ARGS__)
#define deque_front(...) cdc_deque_front(__VA_ARGS__)
#define deque_back(...) cdc_deque_back(__VA_ARGS__)

// Capacity
#define deque_empty(...) cdc_deque_empty(__VA_ARGS__)
#define deque_size(...) cdc_deque_size(__VA_ARGS__)

// Modifiers
#define deque_set(...) cdc_deque_set(__VA_ARGS__)
#define deque_insert(...) cdc_deque_insert(__VA_ARGS__)
#define deque_erase(...) cdc_deque_erase(__VA_ARGS__)
#define deque_clear(...) cdc_deque_clear(__VA_ARGS__)
#define deque_push_back(...) cdc_deque_push_back(__VA_ARGS__)
#define deque_pop_back(...) cdc_deque_pop_back(__VA_ARGS__)
#define deque_push_front(...) cdc_deque_push_front(__VA_ARGS__)
#define deque_pop_front(...) cdc_deque_pop_back(__VA_ARGS__)
#define deque_swap(...) cdc_deque_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H
