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
#define CDC_USE_SHORT_NAMES
#include "cdcontainers/array.h"

#include "cdcontainers/data-info.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_MIN_CAPACITY 4
#define ARRAY_COPACITY_EXP 2.0f

static bool should_grow(array_t *v)
{
  return v->size == v->capacity;
}

static stat_t reallocate(array_t *v, size_t capacity)
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

static stat_t grow(array_t *v)
{
  return reallocate(v, (size_t)(v->capacity * ARRAY_COPACITY_EXP));
}

static void free_data(array_t *v)
{
  if (CDC_HAS_DFREE(v->dinfo)) {
    for (size_t i = 0; i < v->size; ++i) {
      v->dinfo->dfree(v->buffer[i]);
    }
  }
}

static void free_buffer(array_t *v)
{
  free_data(v);
  free(v->buffer);
  v->buffer = NULL;
}

static void move_left(array_t *v, size_t index)
{
  size_t count_bytes = (v->size - index - 1) * sizeof(void *);
  memmove(v->buffer + index, v->buffer + index + 1, count_bytes);
}

static void move_right(array_t *v, size_t index)
{
  size_t count_bytes = (v->size - index) * sizeof(void *);
  memmove(v->buffer + index + 1, v->buffer + index, count_bytes);
}

static stat_t init_varg(array_t *v, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    stat_t ret = array_push_back(v, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }
  return CDC_STATUS_OK;
}

stat_t array_ctor(array_t **v, data_info_t *info)
{
  assert(v != NULL);

  array_t *tmp = (array_t *)calloc(sizeof(array_t), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  stat_t ret = CDC_STATUS_OK;
  if (info && !(tmp->dinfo = di_shared_ctorc(info))) {
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
  di_shared_dtor(tmp->dinfo);
free_array:
  free(tmp);
  return ret;
}

stat_t array_ctorl(array_t **v, data_info_t *info, ...)
{
  assert(v != NULL);

  va_list args;
  va_start(args, info);
  stat_t ret = array_ctorv(v, info, args);
  va_end(args);
  return ret;
}

stat_t array_ctorv(array_t **v, data_info_t *info, va_list args)
{
  assert(v != NULL);

  stat_t ret = array_ctor(v, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  return init_varg(*v, args);
}

void array_dtor(array_t *v)
{
  assert(v != NULL);

  free_buffer(v);
  di_shared_dtor(v->dinfo);
  free(v);
}

stat_t array_insert(array_t *v, size_t index, void *value)
{
  assert(v != NULL);
  assert(index <= v->size);

  if (should_grow(v)) {
    stat_t ret = grow(v);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  move_right(v, index);
  v->buffer[index] = value;
  ++v->size;
  return CDC_STATUS_OK;
}

void array_clear(array_t *v)
{
  assert(v != NULL);

  free_data(v);
  v->size = 0;
}

void array_erase(array_t *v, size_t index)
{
  assert(v != NULL);
  assert(index < v->size);

  if (CDC_HAS_DFREE(v->dinfo)) {
    v->dinfo->dfree(v->buffer[index]);
  }

  move_left(v, index);
  --v->size;
}

stat_t array_reserve(array_t *v, size_t capacity)
{
  assert(v != NULL);

  if (capacity > v->capacity) {
    return reallocate(v, capacity);
  }

  return CDC_STATUS_OK;
}

stat_t array_shrink_to_fit(array_t *v)
{
  assert(v != NULL);

  return reallocate(v, v->size);
}

stat_t array_push_back(array_t *v, void *value)
{
  assert(v != NULL);

  return array_insert(v, v->size, value);
}

void array_swap(array_t *a, array_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(size_t, a->capacity, b->capacity);
  CDC_SWAP(void **, a->buffer, b->buffer);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}

stat_t array_at(array_t *v, size_t index, void **elem)
{
  assert(v != NULL);
  assert(elem != NULL);

  if (index > v->size) {
    return CDC_STATUS_OUT_OF_RANGE;
  }

  *elem = v->buffer[index];
  return CDC_STATUS_OK;
}

stat_t array_append(array_t *v, void **data, size_t len)
{
  assert(v != NULL);

  size_t new_capacity = v->size + len;
  if (new_capacity > v->capacity) {
    stat_t ret = reallocate(v, (size_t)(new_capacity * ARRAY_COPACITY_EXP));
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  memcpy(v->buffer + v->size, data, len * sizeof(void *));
  v->size += len;
  return CDC_STATUS_OK;
}

stat_t array_append_move(array_t *v, array_t *other)
{
  assert(v != NULL);
  assert(other != NULL);

  stat_t stat = array_append(v, other->buffer, other->size);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  other->size = 0;
  return CDC_STATUS_OK;
}
