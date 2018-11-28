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
  * @brief The cdc_vector is a struct and functions that provide a dynamic array
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_VECTOR_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_VECTOR_H

#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

/**
 * @brief The cdc_vector struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_vector {
  size_t size;
  size_t capacity;
  void **buffer;
  struct cdc_data_info *dinfo;
};

/**
 * @brief Constructs an empty vector.
 * @param v - cdc_vector
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_ctor(struct cdc_vector **v,
                              struct cdc_data_info *info);

/**
 * @brief Constructs a vector, initialized by an arbitrary number of pointers.
 * The last item must be NULL.
 * @param v - cdc_vector
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_ctorl(struct cdc_vector **v,
                               struct cdc_data_info *info, ...);

/**
 * @brief Constructs a vector, initialized by args. The last item must be NULL.
 * @param v - cdc_vector
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_ctorv(struct cdc_vector **v,
                               struct cdc_data_info *info, va_list args);

/**
 * @brief Destroys the vector.
 * @param v - cdc_vector
 */
void cdc_vector_dtor(struct cdc_vector *v);

// Element access
/**
 * @brief Returns the item at index position index in the vector. Index must be a
 * valid index position in the vector.
 * @param v - cdc_vector
 * @param index - index of the item to return
 * @return item at the index position
 */
static inline void *cdc_vector_get(struct cdc_vector *v, size_t index)
{
  assert(v != NULL);
  assert(index < v->size);

  return v->buffer[index];
}

/**
 * @brief Writes to a elem the item at index position in the vector.
 * @param v - cdc_vector
 * @param index - index of the item to write at elem
 * @param elem - pointer where the item will be written
 * @return DC_STATUS_OK in a successful case or CDC_STATUS_OUT_OF_RANGE if the
 * index is incorrect
 */
enum cdc_stat cdc_vector_at(struct cdc_vector *v, size_t index, void **elem);

/**
 * @brief Returns a pointer to the first item in the vector.
 * This function assumes that the vector isn't empty.
 * @param v - cdc_vector
 * @return pointer to the first item in the vector
 */
static inline void *cdc_vector_front(struct cdc_vector *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  return v->buffer[0];
}

/**
 * @brief Returns a pointer to the last item in the vector.
 * This function assumes that the vector isn't empty.
 * @param v - cdc_vector
 * @return pointer to the last item in the vector
 */
static inline void *cdc_vector_back(struct cdc_vector *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  return v->buffer[v->size - 1];
}

/**
 * @brief Returns a pointer to the data stored in the vector.
 * This function assumes that the vector isn't empty.
 * @param v - cdc_vector
 * @return pointer to the data stored in the vectore
 */
static inline void **cdc_vector_data(struct cdc_vector *v)
{
  assert(v != NULL);

  return v->buffer;
}

// Capacity
/**
 * @brief Attempts to allocate memory for at least size elements.
 * If you know in advance how large the vector will be, you should call this
 * function to prevent reallocations and memory fragmentation.
 * @param v - cdc_vector
 * @param capacity
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_reserve(struct cdc_vector *v, size_t capacity);

/**
 * @brief Returns true if the vector has size 0; otherwise returns false.
 * @param v - cdc_vector
 * @return true if the vector has size 0; otherwise returns false
 */
static inline bool cdc_vector_empty(struct cdc_vector *v)
{
  assert(v != NULL);

  return v->size == 0;
}

/**
 * @brief Returns the number of items in the vector.
 * @param v - cdc_vector
 * @return size
 */
static inline size_t cdc_vector_size(struct cdc_vector *v)
{
  assert(v != NULL);

  return v->size;
}

/**
 * @brief Returns the maximum number of items that can be stored in the vector
 * without forcing a reallocation.
 * @param v - cdc_vector
 * @return capacity
 */
static inline size_t cdc_vector_capacity(struct cdc_vector *v)
{
  assert(v != NULL);

  return v->capacity;
}

// Modifiers
/**
 * @brief Sets the vector at index position to the value. The function is not
 * called to free memory.
 * @param v - cdc_vector
 * @param index - index position where the value will be written
 * @param value
 */
static inline void cdc_vector_set(struct cdc_vector *v, size_t index, void *value)
{
  assert(v != NULL);
  assert(index < v->size);

  v->buffer[index] = value;
}

/**
 * @brief Inserts value at index position in the vector. If index is 0, the value
 * is prepended to the vector. If index is cdc_vector_size(), the value is
 * appended to the vector.
 * @param v - cdc_vector
 * @param index - index position where the value will be inserted
 * @param value
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_insert(struct cdc_vector *v, size_t index, void *value);

/**
 * @brief Removes the element at index position.
 * The pointer will be written in elem. Index must be a valid index position
 * in the vector. The function is not called to free memory.
 * @param v - cdc_vector
 * @param index - index position where the item will be removed
 * @param elem - pointer where the removed item will be written
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_remove(struct cdc_vector *v, size_t index, void **elem);

/**
 * @brief Removes the element at index position. Index must be a valid index
 * position in the vector.
 * @param v - cdc_vector
 * @param index - index position where the item will be removed
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_vector_erase(struct cdc_vector *v, size_t index)
{
  assert(v != NULL);

  return cdc_vector_remove(v, index, NULL);
}

/**
 * @brief Removes all the elements from the vector.
 * @param v - cdc_vector
 */
void cdc_vector_clear(struct cdc_vector *v);

/**
 * @brief Inserts value at the end of the vector.
 * @param v - cdc_vector
 * @param value
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_push_back(struct cdc_vector *v, void *value);

/**
 * @brief Removes the last item in the vector.
 * @param v - cdc_vector
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_pop_back(struct cdc_vector *v);

/**
 * @brief Appends the data onto the end of cdc_vector v.
 * @param v - cdc_vector
 * @param data - pointer on data
 * @param len - count of data elements
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_append(struct cdc_vector *v, void **data, size_t len);

/**
 * @brief Appends the cdc_vector other onto the end of cdc_vector v.
 * @param v - cdc_vector
 * @param other - cdc_vector
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_vector_append_move(struct cdc_vector *v,
                                     struct cdc_vector *other);

/**
 * @brief Swaps vectors a and b. This operation is very fast and never fails.
 * @param a - cdc_vector
 * @param b - cdc_vector
 */
void cdc_vector_swap(struct cdc_vector *a, struct cdc_vector *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_vector vector_t;

#define vector_ctor(...)        cdc_vector_ctor(__VA_ARGS__)
#define vector_ctorl(...)       cdc_vector_ctorl(__VA_ARGS__)
#define vector_ctorv(...)       cdc_vector_ctorv(__VA_ARGS__)
#define vector_dtor(...)        cdc_vector_dtor(__VA_ARGS__)

// Element access
#define vector_get(...)         cdc_vector_get(__VA_ARGS__)
#define vector_at(...)          cdc_vector_at(__VA_ARGS__)
#define vector_front(...)       cdc_vector_front(__VA_ARGS__)
#define vector_back(...)        cdc_vector_back(__VA_ARGS__)
#define vector_data(...)        cdc_vector_data(__VA_ARGS__)

// Capacity
#define vector_reserve(...)     cdc_vector_reserve(__VA_ARGS__)
#define vector_empty(...)       cdc_vector_empty(__VA_ARGS__)
#define vector_size(...)        cdc_vector_size(__VA_ARGS__)
#define vector_capacity(...)    cdc_vector_capacity(__VA_ARGS__)
#define vector_cap_exp(...)     cdc_vector_cap_exp(__VA_ARGS__)

// Modifiers
#define vector_set(...)         cdc_vector_set(__VA_ARGS__)
#define vector_insert(...)      cdc_vector_insert(__VA_ARGS__)
#define vector_erase(...)       cdc_vector_erase(__VA_ARGS__)
#define vector_clear(...)       cdc_vector_clear(__VA_ARGS__)
#define vector_push_back(...)   cdc_vector_push_back(__VA_ARGS__)
#define vector_pop_back(...)    cdc_vector_pop_back(__VA_ARGS__)
#define vector_append(...)      cdc_vector_append(__VA_ARGS__)
#define vector_append_move(...) cdc_vector_append_move(__VA_ARGS__)
#define vector_swap(...)        cdc_vector_swap(__VA_ARGS__)
#endif

#endif  // CDSTRUCTURES_INCLUDE_CDCONTAINERS_VECTOR_H
