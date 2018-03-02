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

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

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
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_deque_ctor(struct cdc_deque **d, struct cdc_data_info *info);

/**
 * @brief Constructs a deque, initialized by an arbitrary number of pointers.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_deque_ctorl(struct cdc_deque **d,
                              struct cdc_data_info *info, ...);

/**
 * @brief Constructs a deque, initialized by args
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_deque_ctorv(struct cdc_deque **d,
                              struct cdc_data_info *info, va_list args);

/**
 * @brief Destroys the deque.
 */
void cdc_deque_dtor(struct cdc_deque *d);

// Element access
/**
 * @brief Returns the item at index position index in the deque.
 * Index must be a valid index position in the deque.
 */
static inline void *cdc_deque_get(struct cdc_deque *d, size_t index)
{
        assert(d != NULL);
        assert(index < d->size);

        size_t idx = (d->head + index) % d->capacity;

        return d->buffer[idx];
}

/**
 * @brief Writes to a elem the item at index position in the deque.
 * Index must be a valid index position in the deque. Returned CDC_STATUS_OK
 * in a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_deque_at(struct cdc_deque *d, size_t index, void **elem);

/**
 * @brief Returns a pointer to the first item in the deque.
 * This function assumes that the deque isn't empty.
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
 */
static inline void *cdc_deque_back(struct cdc_deque *d)
{
        assert(d != NULL);
        assert(d->size > 0);

        ssize_t idx = (d->tail - 1 + d->capacity) % d->capacity;

        return d->buffer[idx];
}

// Capacity
/**
 * @brief Returns true if the deque has size 0; otherwise returns false.
 */
static inline bool cdc_deque_empty(struct cdc_deque *d)
{
        assert(d != NULL);

        return d->size == 0;
}

/**
 * @brief Returns the number of items in the deque.
 */
static inline size_t cdc_deque_size(struct cdc_deque *d)
{
        assert(d != NULL);

        return d->size;
}

// Modifiers
/**
 * @brief Sets the deque at index position to the value
 */
static inline void cdc_deque_set(struct cdc_deque *d, size_t index, void *value)
{
        assert(d != NULL);
        assert(index < d->size);

        size_t idx = (d->head + index) % d->capacity;

        d->buffer[idx] = value;
}

/**
 * @brief Inserts value at index position in the deque. If index is 0, the value
 * is prepended to the deque. If index is cdc_deque_size(), the value is appended
 * to the deque.
 */
enum cdc_stat cdc_deque_insert(struct cdc_deque *d, size_t index, void *value);

/**
 * @brief Removes the element at index position.
 * The pointer will be written in elem. Index must be a valid index position
 * in the deque. The function is not called to free memory.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_deque_remove(struct cdc_deque *d, size_t index, void **elem);

static inline enum cdc_stat cdc_deque_erase(struct cdc_deque *d, size_t index)
{
        assert(d != NULL);

        return cdc_deque_remove(d, index, NULL);
}
/**
 * @brief Removes all the elements from the deque. If a function has been
 * installed to delete an item, it will be called for each item. Index must be
 * a valid index position in the deque
 */
void cdc_deque_clear(struct cdc_deque *d);

/**
 * @brief Inserts value at the end of the deque. Returned CDC_STATUS_OK in a
 * successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_deque_push_back(struct cdc_deque *d, void *value);

/**
 * @brief Removes the last item in the deque. If a function has been installed
 * to delete an item, it will be called for last item. Returned CDC_STATUS_OK
 * in a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_deque_pop_back(struct cdc_deque *d);

/**
 * @brief Inserts value at the beginning of the deque. Returned CDC_STATUS_OK in
 * a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_deque_push_front(struct cdc_deque *d, void *value);

/**
 * @brief Removes the first item in the deque. If a function has been installed
 * to delete an item, it will be called for last item. Returned CDC_STATUS_OK
 * in a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_deque_pop_front(struct cdc_deque *d);

/**
 * @brief Swaps deques a and b. This operation is very fast and never fails.
 */
void cdc_deque_swap(struct cdc_deque *a, struct cdc_deque *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_deque deque_t;

#define deque_ctor(...)       cdc_deque_ctor(__VA_ARGS__)
#define deque_ctorl(...)      cdc_deque_ctorl(__VA_ARGS__)
#define deque_ctorv(...)      cdc_deque_ctorv(__VA_ARGS__)
#define deque_dtor(...)       cdc_deque_dtor(__VA_ARGS__)

// Element access
#define deque_at(...)         cdc_deque_at(__VA_ARGS__)
#define deque_front(...)      cdc_deque_front(__VA_ARGS__)
#define deque_back(...)       cdc_deque_back(__VA_ARGS__)

// Capacity
#define deque_empty(...)      cdc_deque_empty(__VA_ARGS__)
#define deque_size(...)       cdc_deque_size(__VA_ARGS__)

// Modifiers
#define deque_set(...)        cdc_deque_set(__VA_ARGS__)
#define deque_insert(...)     cdc_deque_insert(__VA_ARGS__)
#define deque_erase(...)      cdc_deque_erase(__VA_ARGS__)
#define deque_clear(...)      cdc_deque_clear(__VA_ARGS__)
#define deque_push_back(...)  cdc_deque_push_back(__VA_ARGS__)
#define deque_pop_back(...)   cdc_deque_pop_back(__VA_ARGS__)
#define deque_push_front(...) cdc_deque_push_front(__VA_ARGS__)
#define deque_pop_front(...)  cdc_deque_pop_back(__VA_ARGS__)
#define deque_swap(...)       cdc_deque_swap(__VA_ARGS__)

#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H
