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
#include "cdcontainers/array.h"

#include "data-info.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_MIN_CAPACITY 4
#define ARRAY_COPACITY_EXP 2.0f

static bool should_grow(struct cdc_array *v) { return v->size == v->capacity; }

static enum cdc_stat reallocate(struct cdc_array *v, size_t capacity)
{
  if (capacity < ARRAY_MIN_CAPACITY) {
    capacity = ARRAY_MIN_CAPACITY;
  }

  if (capacity < v->size) {
    return CDC_STATUS_OK;
  }

  void **tmp = (void **)malloc(capacity * sizeof(void *));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  memcpy(tmp, v->buffer, v->size * sizeof(void *));
  free(v->buffer);
  v->capacity = capacity;
  v->buffer = tmp;
  return CDC_STATUS_OK;
}

static enum cdc_stat grow(struct cdc_array *v)
{
  return reallocate(v, (size_t)(v->capacity * ARRAY_COPACITY_EXP));
}

static void free_data(struct cdc_array *v)
{
  if (CDC_HAS_DFREE(v->dinfo)) {
    for (size_t i = 0; i < v->size; ++i) {
      v->dinfo->dfree(v->buffer[i]);
    }
  }
}

static void free_buffer(struct cdc_array *v)
{
  free_data(v);
  free(v->buffer);
  v->buffer = NULL;
}

static void move_left(struct cdc_array *v, size_t index)
{
  size_t count_bytes = (v->size - index - 1) * sizeof(void *);
  memmove(v->buffer + index, v->buffer + index + 1, count_bytes);
}

static void move_right(struct cdc_array *v, size_t index)
{
  size_t count_bytes = (v->size - index) * sizeof(void *);
  memmove(v->buffer + index + 1, v->buffer + index, count_bytes);
}

static enum cdc_stat init_varg(struct cdc_array *v, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    enum cdc_stat ret = cdc_array_push_back(v, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_array_ctor(struct cdc_array **v, struct cdc_data_info *info)
{
  assert(v != NULL);

  struct cdc_array *tmp =
      (struct cdc_array *)calloc(sizeof(struct cdc_array), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  enum cdc_stat ret = CDC_STATUS_OK;
  if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
    ret = CDC_STATUS_BAD_ALLOC;
    goto free_di;
  }

  ret = reallocate(tmp, ARRAY_MIN_CAPACITY);
  if (ret != CDC_STATUS_OK) {
    goto free_array;
  }

  *v = tmp;
  return ret;

free_di:
  cdc_di_shared_dtor(tmp->dinfo);
free_array:
  free(tmp);
  return ret;
}

enum cdc_stat cdc_array_ctorl(struct cdc_array **v, struct cdc_data_info *info,
                              ...)
{
  assert(v != NULL);

  va_list args;
  va_start(args, info);
  enum cdc_stat ret = cdc_array_ctorv(v, info, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_array_ctorv(struct cdc_array **v, struct cdc_data_info *info,
                              va_list args)
{
  assert(v != NULL);

  enum cdc_stat ret = cdc_array_ctor(v, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  return init_varg(*v, args);
}

void cdc_array_dtor(struct cdc_array *v)
{
  assert(v != NULL);

  free_buffer(v);
  cdc_di_shared_dtor(v->dinfo);
  free(v);
}

enum cdc_stat cdc_array_insert(struct cdc_array *v, size_t index, void *value)
{
  assert(v != NULL);
  assert(index <= v->size);

  if (should_grow(v)) {
    enum cdc_stat ret = grow(v);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  move_right(v, index);
  v->buffer[index] = value;
  ++v->size;
  return CDC_STATUS_OK;
}

void cdc_array_clear(struct cdc_array *v)
{
  assert(v != NULL);

  free_data(v);
  v->size = 0;
}

void cdc_array_erase(struct cdc_array *v, size_t index)
{
  assert(v != NULL);
  assert(index < v->size);

  if (CDC_HAS_DFREE(v->dinfo)) {
    v->dinfo->dfree(v->buffer[index]);
  }

  move_left(v, index);
  --v->size;
}

enum cdc_stat cdc_array_reserve(struct cdc_array *v, size_t capacity)
{
  assert(v != NULL);

  if (capacity > v->capacity) {
    return reallocate(v, capacity);
  }

  return CDC_STATUS_OK;
}

enum cdc_stat cdc_array_shrink_to_fit(struct cdc_array *v)
{
  assert(v != NULL);

  return reallocate(v, v->size);
}

enum cdc_stat cdc_array_push_back(struct cdc_array *v, void *value)
{
  assert(v != NULL);

  return cdc_array_insert(v, v->size, value);
}

void cdc_array_swap(struct cdc_array *a, struct cdc_array *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(size_t, a->capacity, b->capacity);
  CDC_SWAP(void **, a->buffer, b->buffer);
  CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}

enum cdc_stat cdc_array_at(struct cdc_array *v, size_t index, void **elem)
{
  assert(v != NULL);
  assert(elem != NULL);

  if (index > v->size) {
    return CDC_STATUS_OUT_OF_RANGE;
  }

  *elem = v->buffer[index];
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_array_append(struct cdc_array *v, void **data, size_t len)
{
  assert(v != NULL);

  size_t new_capacity = v->size + len;
  if (new_capacity > v->capacity) {
    enum cdc_stat ret =
        reallocate(v, (size_t)(new_capacity * ARRAY_COPACITY_EXP));
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  memcpy(v->buffer + v->size, data, len * sizeof(void *));
  v->size += len;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_array_append_move(struct cdc_array *v,
                                    struct cdc_array *other)
{
  assert(v != NULL);
  assert(other != NULL);

  enum cdc_stat stat = cdc_array_append(v, other->buffer, other->size);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  other->size = 0;
  return CDC_STATUS_OK;
}
