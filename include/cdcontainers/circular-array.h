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
 * @brief The cdc_circular_array is a struct and functions that provide a
 * circular array.
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_CIRCULAR_ARRAY_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_CIRCULAR_ARRAY_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @defgroup cdc_circular_array
 * @brief The cdc_circular_array is a struct and functions that provide a circular array.
 * @{
 */
/**
 * @brief The cdc_circular_array is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_circular_array {
  void **buffer;
  size_t head;
  size_t tail;
  size_t size;
  size_t capacity;
  struct cdc_data_info *dinfo;
};

// Base
/**
 * @defgroup cdc_circular_array_base Base
 * @{
 */
/**
 * @brief Constructs an empty circular array.
 * @param[out] d - cdc_circular_array
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_circular_array_ctor(struct cdc_circular_array **d, struct cdc_data_info *info);

/**
 * @brief Constructs a circular array, initialized by an variable number of pointers.
 * The last pointer must be CDC_END.
 * @param[out] d - cdc_circular_array
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 *
 * Example:
 * @code{.c}
 * struct cdc_circular_array *carr = NULL;
 * if (cdc_circular_array_ctorl(&carr, NULL, CDC_FROM_INT(1),
 *                              CDC_FROM_INT(2), CDC_END) != CDC_STATUS_OK) {
 *   // handle error
 * }
 * @endcode
 */
enum cdc_stat cdc_circular_array_ctorl(struct cdc_circular_array **d, struct cdc_data_info *info,
                                       ...);

/**
 * @brief Constructs a circular array, initialized by args. The last pointer must be CDC_END.
 * @param[out] d - cdc_circular_array
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_circular_array_ctorv(struct cdc_circular_array **d, struct cdc_data_info *info,
                                       va_list args);

/**
 * @brief Destroys the circular array.
 * @param[in] d - cdc_circular_array
 */
void cdc_circular_array_dtor(struct cdc_circular_array *d);
/** @} */

// Element access
/**
 * @defgroup cdc_circular_array_element_access Element access
 * @{
 */
/**
 * @brief Returns an element at index position in the circular array.
 * @param[in] d - cdc_circular_array
 * @param[in] index - index of an element to return
 * @return element from |index| position.
 */
static inline void *cdc_circular_array_get(struct cdc_circular_array *d, size_t index)
{
  assert(d != NULL);
  assert(index < d->size);

  size_t real_index = (d->head + index) & (d->capacity - 1);
  return d->buffer[real_index];
}

/**
 * @brief Writes to pointer an element from specified position in the circular array.
 * Bounds checking is performed.
 * @param[in] d - cdc_circular_array
 * @param[in] index - index of an element to write at elem
 * @param[out] elem - pointer where an element will be written
 * @return DC_STATUS_OK in a successful case or CDC_STATUS_OUT_OF_RANGE if the
 * index is incorrect.
 */
enum cdc_stat cdc_circular_array_at(struct cdc_circular_array *d, size_t index, void **elem);

/**
 * @brief Returns a first element in the circular array.
 * @param[in] d - cdc_circular_array
 * @return first element in the circular array.
 */
static inline void *cdc_circular_array_front(struct cdc_circular_array *d)
{
  assert(d != NULL);
  assert(d->size > 0);

  return d->buffer[d->head];
}

/**
 * @brief Returns a last element in the circular array.
 * @param[in] d - cdc_circular_array
 * @return last element in the circular array.
 */
static inline void *cdc_circular_array_back(struct cdc_circular_array *d)
{
  assert(d != NULL);
  assert(d->size > 0);

  size_t real_index = (d->tail - 1 + d->capacity) & (d->capacity - 1);
  return d->buffer[real_index];
}
/** @} */

// Capacity
/**
 * @defgroup cdc_circular_array_capacity Capacity
 * @{
 */
/**
 * @brief Checks if the circular array has no elements.
 * @param[in] d - cdc_circular_array
 * @return true if the circular array is empty, false otherwise.
 */
static inline bool cdc_circular_array_empty(struct cdc_circular_array *d)
{
  assert(d != NULL);

  return d->size == 0;
}

/**
 * @brief Returns the number of elements in the circular array.
 * @param[in] d - cdc_circular_array
 * @return the number of elements in the circular array.
 */
static inline size_t cdc_circular_array_size(struct cdc_circular_array *d)
{
  assert(d != NULL);

  return d->size;
}
/** @} */

// Modifiers
/**
 * @defgroup cdc_circular_array_modifiers Modifiers
 * @{
 */
/**
 * @brief Sets an element at index position to the value. The function is not
 * called to free memory.
 * @param[in] d - cdc_circular_array
 * @param[in] index - index position where the value will be written
 * @param[in] value - value
 */
static inline void cdc_circular_array_set(struct cdc_circular_array *d, size_t index, void *value)
{
  assert(d != NULL);
  assert(index < d->size);

  size_t real_index = (d->head + index) & (d->capacity - 1);
  d->buffer[real_index] = value;
}

/**
 * @brief Inserts value at |index| position in the circular array. If index is 0, the
 * value is prepended to the circular array. If index is cdc_circular_array_size(), the value is
 * appended to the circular array.
 * @param[in] d - cdc_circular_array
 * @param[in] index - index position where an element will be inserted
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_circular_array_insert(struct cdc_circular_array *d, size_t index, void *value);

/**
 * @brief Removes an element at index position in the circular arrray.
 * @param[in] d - cdc_circular_array
 * @param[in] index - index position where an element will be removed
 */
void cdc_circular_array_erase(struct cdc_circular_array *d, size_t index);

/**
 * @brief Removes all the elements from the circular array.
 * @param[in] d - cdc_circular_array
 */
void cdc_circular_array_clear(struct cdc_circular_array *d);

/**
 * @brief Inserts an element at the end of the circular array.
 * @param[in] d - cdc_circular_array
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_circular_array_push_back(struct cdc_circular_array *d, void *value);

/**
 * @brief Removes a last element in the circular array.
 * @param[in] d - cdc_circular_array
 */
void cdc_circular_array_pop_back(struct cdc_circular_array *d);

/**
 * @brief Inserts an element at the beginning of the circular array.
 * @param[in] d - cdc_circular_array
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_circular_array_push_front(struct cdc_circular_array *d, void *value);

/**
 * @brief Removes a first element in the circular array.
 * @param[in] d - cdc_circular_array
 */
void cdc_circular_array_pop_front(struct cdc_circular_array *d);

/**
 * @brief Swaps circular arrays a and b. This operation is very fast and never
 * fails.
 * @param a - cdc_circular_array
 * @param b - cdc_circular_array
 */
void cdc_circular_array_swap(struct cdc_circular_array *a, struct cdc_circular_array *b);
/** @} */

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_circular_array circular_array_t;

// Base
#define circular_array_ctor(...) cdc_circular_array_ctor(__VA_ARGS__)
#define circular_array_ctorl(...) cdc_circular_array_ctorl(__VA_ARGS__)
#define circular_array_ctorv(...) cdc_circular_array_ctorv(__VA_ARGS__)
#define circular_array_dtor(...) cdc_circular_array_dtor(__VA_ARGS__)

// Element access
#define circular_array_get(...) cdc_circular_array_get(__VA_ARGS__)
#define circular_array_at(...) cdc_circular_array_at(__VA_ARGS__)
#define circular_array_front(...) cdc_circular_array_front(__VA_ARGS__)
#define circular_array_back(...) cdc_circular_array_back(__VA_ARGS__)

// Capacity
#define circular_array_empty(...) cdc_circular_array_empty(__VA_ARGS__)
#define circular_array_size(...) cdc_circular_array_size(__VA_ARGS__)

// Modifiers
#define circular_array_set(...) cdc_circular_array_set(__VA_ARGS__)
#define circular_array_insert(...) cdc_circular_array_insert(__VA_ARGS__)
#define circular_array_erase(...) cdc_circular_array_erase(__VA_ARGS__)
#define circular_array_clear(...) cdc_circular_array_clear(__VA_ARGS__)
#define circular_array_push_back(...) cdc_circular_array_push_back(__VA_ARGS__)
#define circular_array_pop_back(...) cdc_circular_array_pop_back(__VA_ARGS__)
#define circular_array_push_front(...) cdc_circular_array_push_front(__VA_ARGS__)
#define circular_array_pop_front(...) cdc_circular_array_pop_front(__VA_ARGS__)
#define circular_array_swap(...) cdc_circular_array_swap(__VA_ARGS__)
#endif
/** @} */
#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_CIRCULAR_ARRAY_H
