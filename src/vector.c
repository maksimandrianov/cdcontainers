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
#include "cdcontainers/vector.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data-info.h"

#define VECTOR_MIN_CAPACITY 4
#define VECTOR_COPACITY_EXP 2.0f
#define VECTOR_SHRINK_THRESHOLD 4.0f

static bool should_shrink(struct cdc_vector *v)
{
  return v->size * VECTOR_SHRINK_THRESHOLD <= v->capacity;
}

static bool should_grow(struct cdc_vector *v) { return v->size == v->capacity; }

static enum cdc_stat reallocate(struct cdc_vector *v, size_t capacity)
{
  if (capacity < VECTOR_MIN_CAPACITY) {
    if (v->capacity > VECTOR_MIN_CAPACITY) {
      capacity = VECTOR_MIN_CAPACITY;
    } else {
      return CDC_STATUS_OK;
    }
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

static enum cdc_stat grow(struct cdc_vector *v)
{
  return reallocate(v, v->capacity * VECTOR_COPACITY_EXP);
}

static enum cdc_stat shrink(struct cdc_vector *v)
{
  return reallocate(v, v->capacity / VECTOR_COPACITY_EXP);
}

static void move_left(struct cdc_vector *v, size_t index)
{
  size_t count_bytes = (v->size - index - 1) * sizeof(void *);
  memmove(v->buffer + index, v->buffer + index + 1, count_bytes);
}

static void move_right(struct cdc_vector *v, size_t index)
{
  size_t count_bytes = (v->size - index) * sizeof(void *);
  memmove(v->buffer + index + 1, v->buffer + index, count_bytes);
}

static void free_data(struct cdc_vector *v)
{
  if (!CDC_HAS_DFREE(v->dinfo)) {
    return;
  }

  for (size_t i = 0; i < v->size; ++i) {
    v->dinfo->dfree(v->buffer[i]);
  }
}

static enum cdc_stat pop_back(struct cdc_vector *v, bool must_free)
{
  if (must_free && CDC_HAS_DFREE(v->dinfo)) {
    v->dinfo->dfree(v->buffer[v->size - 1]);
  }

  --v->size;
  if (should_shrink(v)) {
    enum cdc_stat ret = shrink(v);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }
  return CDC_STATUS_OK;
}

static enum cdc_stat init_varg(struct cdc_vector *v, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != NULL) {
    enum cdc_stat ret = cdc_vector_push_back(v, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_vector_ctor(struct cdc_vector **v, struct cdc_data_info *info)
{
  assert(v != NULL);

  enum cdc_stat ret = CDC_STATUS_OK;
  struct cdc_vector *tmp =
      (struct cdc_vector *)calloc(sizeof(struct cdc_vector), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
    ret = CDC_STATUS_BAD_ALLOC;
    goto error1;
  }

  ret = reallocate(tmp, VECTOR_MIN_CAPACITY);
  if (ret != CDC_STATUS_OK) {
    goto error2;
  }

  *v = tmp;
  return ret;
error2:
  cdc_di_shared_dtor(tmp->dinfo);
error1:
  free(tmp);
  return ret;
}

enum cdc_stat cdc_vector_ctorl(struct cdc_vector **v,
                               struct cdc_data_info *info, ...)
{
  assert(v != NULL);

  va_list args;
  va_start(args, info);
  enum cdc_stat ret = cdc_vector_ctorv(v, info, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_vector_ctorv(struct cdc_vector **v,
                               struct cdc_data_info *info, va_list args)
{
  assert(v != NULL);

  enum cdc_stat ret = cdc_vector_ctor(v, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }
  return init_varg(*v, args);
}

void cdc_vector_dtor(struct cdc_vector *v)
{
  assert(v != NULL);

  free_data(v);
  free(v->buffer);
  cdc_di_shared_dtor(v->dinfo);
  free(v);
}

enum cdc_stat cdc_vector_insert(struct cdc_vector *v, size_t index, void *value)
{
  assert(v != NULL);
  assert(index <= v->size);

  if (index == v->size) {
    return cdc_vector_push_back(v, value);
  }

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

void cdc_vector_clear(struct cdc_vector *v)
{
  assert(v != NULL);

  free_data(v);
  v->size = 0;
}

enum cdc_stat cdc_vector_remove(struct cdc_vector *v, size_t index, void **elem)
{
  assert(v != NULL);
  assert(index < v->size);

  if (elem) {
    *elem = v->buffer[index];
  } else if (CDC_HAS_DFREE(v->dinfo)) {
    v->dinfo->dfree(v->buffer[index]);
  }

  if (index == v->size - 1) {
    return pop_back(v, false);
  }

  move_left(v, index);
  --v->size;
  if (should_shrink(v)) {
    enum cdc_stat ret = shrink(v);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  return CDC_STATUS_OK;
}

enum cdc_stat cdc_vector_reserve(struct cdc_vector *v, size_t capacity)
{
  assert(v != NULL);

  if (capacity > v->capacity) {
    return reallocate(v, capacity);
  }
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_vector_push_back(struct cdc_vector *v, void *value)
{
  assert(v != NULL);

  if (should_grow(v)) {
    enum cdc_stat ret = grow(v);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  v->buffer[v->size++] = value;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_vector_pop_back(struct cdc_vector *v)
{
  assert(v != NULL);
  assert(v->size > 0);

  return pop_back(v, true);
}

void cdc_vector_swap(struct cdc_vector *a, struct cdc_vector *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(size_t, a->capacity, b->capacity);
  CDC_SWAP(void **, a->buffer, b->buffer);
  CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}

enum cdc_stat cdc_vector_at(struct cdc_vector *v, size_t index, void **elem)
{
  assert(v != NULL);
  assert(elem != NULL);

  if (index > v->size) {
    return CDC_STATUS_OUT_OF_RANGE;
  }

  *elem = v->buffer[index];
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_vector_append(struct cdc_vector *v, void **data, size_t len)
{
  assert(v != NULL);

  size_t new_capacity = v->size + len;
  if (new_capacity > v->capacity) {
    enum cdc_stat ret = reallocate(v, new_capacity * VECTOR_COPACITY_EXP);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  memcpy(v->buffer + v->size, data, len * sizeof(void *));
  v->size += len;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_vector_append_move(struct cdc_vector *v,
                                     struct cdc_vector *other)
{
  assert(v != NULL);
  assert(other != NULL);

  enum cdc_stat stat = cdc_vector_append(v, other->buffer, other->size);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  other->size = 0;
  return CDC_STATUS_OK;
}
