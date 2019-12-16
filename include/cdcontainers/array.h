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
 * @file array.h
 * @brief The cdc_array is a struct and functions that provide a dynamic array.
 *
 * Example usage array:
 * @include array.c
 *
 * @author Maksim Andrianov <maksimandrianov1@yandex.ru>
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_VECTOR_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_VECTOR_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * @brief The cdc_array is service struct.
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_array {
  size_t size;
  size_t capacity;
  void **buffer;
  struct cdc_data_info *dinfo;
};

/**
 * @brief Constructs an empty array.
 * @param[out] v - cdc_array
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_array_ctor(struct cdc_array **v, struct cdc_data_info *info);

/**
 * @brief Constructs an array, initialized by an variable number of pointers.
 * The last pointer must be CDC_END.
 * @param[out] v - cdc_array
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 *
 * Example:
 * @code{.c}
 * struct cdc_array *vec = NULL;
 * if (cdc_array_ctorl(&vec, NULL, CDC_FROM_INT(1),
 *                     CDC_FROM_INT(2), CDC_END) != CDC_STATUS_OK) {
 *   // handle error
 * }
 * @endcode
 */
enum cdc_stat cdc_array_ctorl(struct cdc_array **v, struct cdc_data_info *info,
                              ...);

/**
 * @brief Constructs an array, initialized by args. The last pointer must be CDC_END.
 * @param[out] v - cdc_array
 * @param[in] info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_array_ctorv(struct cdc_array **v, struct cdc_data_info *info,
                              va_list args);

/**
 * @brief Destroys the array.
 * @param[in] v - cdc_array
 */
void cdc_array_dtor(struct cdc_array *v);

// Element access
/**
 * @brief Returns an element at index position in the array
 * @param[in] v - cdc_array
 * @param[in] index - index of an element to return
 * @return element from |index| position.
 */
static inline void *cdc_array_get(struct cdc_array *v, size_t index)
{
  assert(v != NULL);
  assert(index < v->size);

  return v->buffer[index];
}

/**
 * @brief Writes to pointer an element from specified position in the array.
 * Bounds checking is performed.
 * @param[in] v - cdc_array
 * @param[in] index - index of an element to write at elem
 * @param[out] elem - pointer where an element will be written
 * @return DC_STATUS_OK in a successful case or CDC_STATUS_OUT_OF_RANGE if the
 * index is incorrect.
 */
enum cdc_stat cdc_array_at(struct cdc_array *v, size_t index, void **elem);

/**
 * @brief Returns a first element in the array.
 * @param[in] v - cdc_array
 * @return first element in the array.
 */
static inline void *cdc_array_front(struct cdc_array *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  return v->buffer[0];
}

/**
 * @brief Returns a last element in the array.
 * @param[in] v - cdc_array
 * @return last element in the array.
 */
static inline void *cdc_array_back(struct cdc_array *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  return v->buffer[v->size - 1];
}

/**
 * @brief Returns a pointer to the data stored in the array.
 * @param[in] v - cdc_array
 * @return pointer to the data stored in the array.
 */
static inline void **cdc_array_data(struct cdc_array *v)
{
  assert(v != NULL);

  return v->buffer;
}

// Capacity
/**
 * @brief Attempts to allocate memory for at least size elements.
 * If you know in advance how large the array will be, you should call this
 * function to prevent reallocations and memory fragmentation.
 * @param[in] v - cdc_array
 * @param[in] capacity - new capacity
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_array_reserve(struct cdc_array *v, size_t capacity);

/**
 * @brief Checks if the array has no elements.
 * @param[in] v - cdc_array
 * @return true if the array is empty, false otherwise.
 */
static inline bool cdc_array_empty(struct cdc_array *v)
{
  assert(v != NULL);

  return v->size == 0;
}

/**
 * @brief Returns the number of elements in the array.
 * @param[in] v - cdc_array
 * @return the number of elements in the array.
 */
static inline size_t cdc_array_size(struct cdc_array *v)
{
  assert(v != NULL);

  return v->size;
}

/**
 * @brief Returns the number of elements that the container has currently allocated space for.
 * @param[in] v - cdc_array
 * @return capacity of the currently allocated storage.
 */
static inline size_t cdc_array_capacity(struct cdc_array *v)
{
  assert(v != NULL);

  return v->capacity;
}

/**
 * @brief Requests the container to reduce its capacity to fit its size.
 * @param[in] v - cdc_array
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error
 */
enum cdc_stat cdc_array_shrink_to_fit(struct cdc_array *v);

// Modifiers
/**
 * @brief Sets an element at index position to the value. The function is not
 * called to free memory.
 * @param[in] v - cdc_array
 * @param[in] index - index position where the value will be written
 * @param[in] value - value
 */
static inline void cdc_array_set(struct cdc_array *v, size_t index, void *value)
{
  assert(v != NULL);
  assert(index < v->size);

  v->buffer[index] = value;
}

/**
 * @brief Inserts value at |index| position in the array. If index is 0, the
 * value is prepended to the array. If index is cdc_array_size(), the value is
 * appended to the array.
 * @param[in] v - cdc_array
 * @param[in] index - index position where an element will be inserted
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_array_insert(struct cdc_array *v, size_t index, void *value);

/**
 * @brief Removes an element at index position in the arrray.
 * @param[in] v - cdc_array
 * @param[in] index - index position where an element will be removed
 */
void cdc_array_erase(struct cdc_array *v, size_t index);

/**
 * @brief Removes all the elements from the array.
 * @param[in] v - cdc_array
 */
void cdc_array_clear(struct cdc_array *v);

/**
 * @brief Inserts an element at the end of the array.
 * @param[in] v - cdc_array
 * @param[in] value - value
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_array_push_back(struct cdc_array *v, void *value);

/**
 * @brief Removes a last element in the array.
 * @param[in] v - cdc_array
 */
static inline void cdc_array_pop_back(struct cdc_array *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  cdc_array_erase(v, v->size - 1);
}

/**
 * @brief Appends elements at the end of array.
 * @param[in] v - cdc_array
 * @param[in] data - pointer on data
 * @param[in] len - the number of data elements
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_array_append(struct cdc_array *v, void **data, size_t len);

/**
 * @brief Appends one array to the end of other array.
 * @param[in] v - cdc_array
 * @param[in] other - cdc_array
 * @return CDC_STATUS_OK in a successful case or other value indicating
 * an error.
 */
enum cdc_stat cdc_array_append_move(struct cdc_array *v,
                                    struct cdc_array *other);

/**
 * @brief Swaps arrays. This operation is very fast and never fails.
 * @param[in, out] a - cdc_array
 * @param[in, out] b - cdc_array
 */
void cdc_array_swap(struct cdc_array *a, struct cdc_array *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_array array_t;

#define array_ctor(...) cdc_array_ctor(__VA_ARGS__)
#define array_ctorl(...) cdc_array_ctorl(__VA_ARGS__)
#define array_ctorv(...) cdc_array_ctorv(__VA_ARGS__)
#define array_dtor(...) cdc_array_dtor(__VA_ARGS__)

// Element access
#define array_get(...) cdc_array_get(__VA_ARGS__)
#define array_at(...) cdc_array_at(__VA_ARGS__)
#define array_front(...) cdc_array_front(__VA_ARGS__)
#define array_back(...) cdc_array_back(__VA_ARGS__)
#define array_data(...) cdc_array_data(__VA_ARGS__)

// Capacity
#define array_reserve(...) cdc_array_reserve(__VA_ARGS__)
#define array_empty(...) cdc_array_empty(__VA_ARGS__)
#define array_size(...) cdc_array_size(__VA_ARGS__)
#define array_capacity(...) cdc_array_capacity(__VA_ARGS__)
#define array_cap_exp(...) cdc_array_cap_exp(__VA_ARGS__)
#define array_shrink_to_fit(...) cdc_array_shrink_to_fit(__VA_ARGS__)

// Modifiers
#define array_set(...) cdc_array_set(__VA_ARGS__)
#define array_insert(...) cdc_array_insert(__VA_ARGS__)
#define array_erase(...) cdc_array_erase(__VA_ARGS__)
#define array_clear(...) cdc_array_clear(__VA_ARGS__)
#define array_push_back(...) cdc_array_push_back(__VA_ARGS__)
#define array_pop_back(...) cdc_array_pop_back(__VA_ARGS__)
#define array_append(...) cdc_array_append(__VA_ARGS__)
#define array_append_move(...) cdc_array_append_move(__VA_ARGS__)
#define array_swap(...) cdc_array_swap(__VA_ARGS__)
#endif

#endif  // CDSTRUCTURES_INCLUDE_CDCONTAINERS_VECTOR_H
