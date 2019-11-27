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
 * @brief The cdc_array struct
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
 * @param v - cdc_array
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_ctor(struct cdc_array **v, struct cdc_data_info *info);

/**
 * @brief Constructs a array, initialized by an arbitrary number of pointers.
 * The last item must be NULL.
 * @param v - cdc_array
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_ctorl(struct cdc_array **v, struct cdc_data_info *info,
                              ...);

/**
 * @brief Constructs a array, initialized by args. The last item must be NULL.
 * @param v - cdc_array
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_ctorv(struct cdc_array **v, struct cdc_data_info *info,
                              va_list args);

/**
 * @brief Destroys the array.
 * @param v - cdc_array
 */
void cdc_array_dtor(struct cdc_array *v);

// Element access
/**
 * @brief Returns the item at index position index in the array. Index must be
 * a valid index position in the array.
 * @param v - cdc_array
 * @param index - index of the item to return
 * @return item at the index position
 */
static inline void *cdc_array_get(struct cdc_array *v, size_t index)
{
  assert(v != NULL);
  assert(index < v->size);

  return v->buffer[index];
}

/**
 * @brief Writes to a elem the item at index position in the array.
 * @param v - cdc_array
 * @param index - index of the item to write at elem
 * @param elem - pointer where the item will be written
 * @return DC_STATUS_OK in a successful case or CDC_STATUS_OUT_OF_RANGE if the
 * index is incorrect
 */
enum cdc_stat cdc_array_at(struct cdc_array *v, size_t index, void **elem);

/**
 * @brief Returns a pointer to the first item in the array.
 * This function assumes that the array isn't empty.
 * @param v - cdc_array
 * @return pointer to the first item in the array
 */
static inline void *cdc_array_front(struct cdc_array *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  return v->buffer[0];
}

/**
 * @brief Returns a pointer to the last item in the array.
 * This function assumes that the array isn't empty.
 * @param v - cdc_array
 * @return pointer to the last item in the array
 */
static inline void *cdc_array_back(struct cdc_array *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  return v->buffer[v->size - 1];
}

/**
 * @brief Returns a pointer to the data stored in the array.
 * This function assumes that the array isn't empty.
 * @param v - cdc_array
 * @return pointer to the data stored in the arraye
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
 * @param v - cdc_array
 * @param capacity
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_reserve(struct cdc_array *v, size_t capacity);

/**
 * @brief Returns true if the array has size 0; otherwise returns false.
 * @param v - cdc_array
 * @return true if the array has size 0; otherwise returns false
 */
static inline bool cdc_array_empty(struct cdc_array *v)
{
  assert(v != NULL);

  return v->size == 0;
}

/**
 * @brief Returns the number of items in the array.
 * @param v - cdc_array
 * @return size
 */
static inline size_t cdc_array_size(struct cdc_array *v)
{
  assert(v != NULL);

  return v->size;
}

/**
 * @brief Returns the maximum number of items that can be stored in the array
 * without forcing a reallocation.
 * @param v - cdc_array
 * @return capacity
 */
static inline size_t cdc_array_capacity(struct cdc_array *v)
{
  assert(v != NULL);

  return v->capacity;
}

/**
 * @brief Requests the container to reduce its capacity to fit its size.
 * @param v - cdc_array
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_shrink_to_fit(struct cdc_array *v);

// Modifiers
/**
 * @brief Sets the array at index position to the value. The function is not
 * called to free memory.
 * @param v - cdc_array
 * @param index - index position where the value will be written
 * @param value
 */
static inline void cdc_array_set(struct cdc_array *v, size_t index, void *value)
{
  assert(v != NULL);
  assert(index < v->size);

  v->buffer[index] = value;
}

/**
 * @brief Inserts value at index position in the array. If index is 0, the
 * value is prepended to the array. If index is cdc_array_size(), the value is
 * appended to the array.
 * @param v - cdc_array
 * @param index - index position where the value will be inserted
 * @param value
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_insert(struct cdc_array *v, size_t index, void *value);

/**
 * @brief Removes the element at index position. Index must be a valid index
 * position in the array.
 * @param v - cdc_array
 * @param index - index position where the item will be removed
 */
void cdc_array_erase(struct cdc_array *v, size_t index);

/**
 * @brief Removes all the elements from the array.
 * @param v - cdc_array
 */
void cdc_array_clear(struct cdc_array *v);

/**
 * @brief Inserts value at the end of the array.
 * @param v - cdc_array
 * @param value
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_push_back(struct cdc_array *v, void *value);

/**
 * @brief Removes the last item in the array.
 * @param v - cdc_array
 */
static inline void cdc_array_pop_back(struct cdc_array *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  cdc_array_erase(v, v->size - 1);
}

/**
 * @brief Appends the data onto the end of cdc_array v.
 * @param v - cdc_array
 * @param data - pointer on data
 * @param len - count of data elements
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_append(struct cdc_array *v, void **data, size_t len);

/**
 * @brief Appends the cdc_array other onto the end of cdc_array v.
 * @param v - cdc_array
 * @param other - cdc_array
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_array_append_move(struct cdc_array *v,
                                    struct cdc_array *other);

/**
 * @brief Swaps arrays a and b. This operation is very fast and never fails.
 * @param a - cdc_array
 * @param b - cdc_array
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
