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
#include "cdcontainers/circular-array.h"

#include "cdcontainers/data-info.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CDC_CIRCULAR_ARRAY_MIN_CAPACITY 4  // must be pow 2
#define CDC_CIRCULAR_ARRAY_COPACITY_SHIFT 1

static size_t get_real_index(circular_array_t *d, size_t index)
{
  return (d->head + index) & (d->capacity - 1);
}

static bool should_grow(circular_array_t *d)
{
  return d->size == d->capacity - 1;
}

static stat_t reallocate(circular_array_t *d, size_t capacity)
{
  if (capacity < CDC_CIRCULAR_ARRAY_MIN_CAPACITY) {
    capacity = CDC_CIRCULAR_ARRAY_MIN_CAPACITY;
  }

  if (capacity < d->size) {
    return CDC_STATUS_OK;
  }

  void **tmp = (void **)malloc(capacity * sizeof(void *));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (d->head > d->tail) {
    //  _________________________________
    //  | 4 | 5 | 6 | . | . | 1 | 2 | 3 |
    //  ---------------------------------
    size_t len = d->capacity - d->head;
    memcpy(tmp, d->buffer + d->head, len * sizeof(void *));
    memcpy(tmp + len, d->buffer, d->tail * sizeof(void *));
  } else {
    //  _________________________________
    //  | 1 | 2 | 3 | 4 | 5 | 6 | . | . |
    //  ---------------------------------
    memcpy(tmp, d->buffer + d->head, d->size * sizeof(void *));
  }

  free(d->buffer);
  d->tail = d->size;
  d->head = 0;
  d->capacity = capacity;
  d->buffer = tmp;
  return CDC_STATUS_OK;
}

static stat_t grow(circular_array_t *d)
{
  return reallocate(d, d->capacity << CDC_CIRCULAR_ARRAY_COPACITY_SHIFT);
}

static void free_data(circular_array_t *d)
{
  if (!CDC_HAS_DFREE(d->dinfo)) {
    return;
  }

  for (size_t i = 0; i < d->size; ++i) {
    size_t real_index = get_real_index(d, i);
    d->dinfo->dfree(d->buffer[real_index]);
  }
}

static stat_t init_varg(circular_array_t *d, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    stat_t stat = circular_array_push_back(d, elem);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  return CDC_STATUS_OK;
}

stat_t circular_array_ctor(circular_array_t **d, data_info_t *info)
{
  assert(d != NULL);

  circular_array_t *tmp = (circular_array_t *)calloc(sizeof(circular_array_t), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  stat_t ret = CDC_STATUS_OK;
  if (info && !(tmp->dinfo = di_shared_ctorc(info))) {
    ret = CDC_STATUS_BAD_ALLOC;
    goto free_di;
  }

  ret = reallocate(tmp, CDC_CIRCULAR_ARRAY_MIN_CAPACITY);
  if (ret != CDC_STATUS_OK) {
    goto free_circular_array;
  }

  *d = tmp;
  return ret;

free_di:
  di_shared_dtor(tmp->dinfo);
free_circular_array:
  free(tmp);
  return ret;
}

stat_t circular_array_ctorl(circular_array_t **d, data_info_t *info, ...)
{
  assert(d != NULL);

  va_list args;
  va_start(args, info);
  stat_t stat = circular_array_ctorv(d, info, args);
  va_end(args);
  return stat;
}

stat_t circular_array_ctorv(circular_array_t **d, data_info_t *info, va_list args)
{
  assert(d != NULL);

  stat_t stat = circular_array_ctor(d, info);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  return init_varg(*d, args);
}

void circular_array_dtor(circular_array_t *d)
{
  assert(d != NULL);

  free_data(d);
  free(d->buffer);
  di_shared_dtor(d->dinfo);
  free(d);
}

stat_t circular_array_at(circular_array_t *d, size_t index, void **elem)
{
  assert(d != NULL);
  assert(elem != NULL);

  if (index > d->size) {
    return CDC_STATUS_OUT_OF_RANGE;
  }

  size_t real_index = get_real_index(d, index);
  *elem = d->buffer[real_index];
  return CDC_STATUS_OK;
}

stat_t circular_array_insert(circular_array_t *d, size_t index, void *value)
{
  assert(d != NULL);
  assert(index <= d->size);

  if (index == 0) {
    return circular_array_push_front(d, value);
  }

  if (index == d->size) {
    return circular_array_push_back(d, value);
  }

  if (should_grow(d)) {
    stat_t stat = grow(d);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  size_t real_index = get_real_index(d, index);
  size_t mcapacity = d->capacity - 1;
  if (index < d->size / 2) {
    // New head.
    if (d->head < d->tail) {
      if (d->head == 0) {
        //  _________________________________
        //  | 1 | 2 | 3 | 4 | 5 | 6 | . | . |
        //  ---------------------------------
        //  <---------x
        d->buffer[d->capacity - 1] = d->buffer[0];
        memmove(d->buffer, d->buffer + 1, (real_index - d->head - 1) * sizeof(void *));
      } else {
        //  _________________________________
        //  | . | 1 | 2 | 3 | 4 | 5 | 6 | . |
        //  ---------------------------------
        //  <--------x
        void **start = d->buffer + d->head;
        memmove(start - 1, start, (real_index - d->head) * sizeof(void *));
      }
    } else {
      if (real_index > d->head) {
        //  _________________________________
        //  | 4 | 5 | 6 | . | . | 1 | 2 | 3 |
        //  ---------------------------------
        //  <-------------------------x
        void **start = d->buffer + d->head;
        memmove(start - 1, start, (real_index - d->head) * sizeof(void *));
      } else {
        //  _________________________________
        //  | 2 | 3 | 4 | 5 | 6 | . | . | 1 |
        //  ---------------------------------
        //  <-----x
        void **start = d->buffer + d->head;
        memmove(start - 1, start, (d->capacity - d->head) * sizeof(void *));
        d->buffer[d->capacity - 1] = d->buffer[0];
        memmove(d->buffer, d->buffer + 1, real_index * sizeof(void *));
      }
    }

    real_index = (real_index + mcapacity) & mcapacity;
    d->head = (d->head + mcapacity) & mcapacity;
  } else {
    // New tail.
    if (d->head < d->tail) {
      if (d->tail + 1 == d->capacity) {
        //  _________________________________
        //  | . | . | 1 | 2 | 3 | 4 | 5 | 6 |
        //  ---------------------------------
        //                        x------->
        d->buffer[0] = d->buffer[d->capacity - 1];
      }
      //  ______________________________________
      //  | . | 1 | 2 | 3 | 4 | 5 | 6 | . or 7 |
      //  --------------------------------------
      //                    x----------->
      void **start = d->buffer + real_index;
      memmove(start + 1, start, (d->tail - real_index) * sizeof(void *));
    } else {
      if (real_index < d->tail) {
        //  _________________________________
        //  | 4 | 5 | 6 | . | . | 1 | 2 | 3 |
        //  ---------------------------------
        //    x------->
        void **start = d->buffer + real_index;
        memmove(start + 1, start, (d->tail - real_index) * sizeof(void *));
      } else {
        //  _________________________________
        //  | 6 | . | . | 1 | 2 | 3 | 4 | 5 |
        //  ---------------------------------
        //                            x---->
        memmove(d->buffer + 1, d->buffer, d->tail * sizeof(void *));
        d->buffer[0] = d->buffer[d->capacity - 1];
        void **start = d->buffer + real_index;
        memmove(start + 1, start, (d->capacity - real_index - 1) * sizeof(void *));
      }
    }

    d->tail = (d->tail + 1) & mcapacity;
  }

  d->buffer[real_index] = value;
  ++d->size;
  return CDC_STATUS_OK;
}

void circular_array_erase(circular_array_t *d, size_t index)
{
  assert(d != NULL);
  assert(index < d->size);

  if (index == 0) {
    circular_array_pop_front(d);
    return;
  }

  if (index + 1 == d->size) {
    circular_array_pop_back(d);
    return;
  }

  size_t real_index = get_real_index(d, index);
  if (CDC_HAS_DFREE(d->dinfo)) {
    d->dinfo->dfree(d->buffer[real_index]);
  }

  size_t mcapacity = d->capacity - 1;
  if (index < d->size / 2) {
    // New head.
    if (d->head < d->tail) {
      //  _________________________________
      //  | 1 | 2 | 3 | 4 | 5 | 6 | . | . |
      //  ---------------------------------
      //  ----->x
      void **start = d->buffer + d->head;
      memmove(start + 1, start, (real_index - d->head) * sizeof(void *));
    } else {
      if (real_index > d->head) {
        //  _________________________________
        //  | 4 | 5 | 6 | . | . | 1 | 2 | 3 |
        //  ---------------------------------
        //                      ----->x
        void **start = d->buffer + d->head;
        memmove(start + 1, start, (real_index - d->head) * sizeof(void *));
      } else {
        //  _________________________________
        //  | 2 | 3 | 4 | 5 | 6 | . | . | 1 |
        //  ---------------------------------
        //  ----->x
        memmove(d->buffer + 1, d->buffer, real_index * sizeof(void *));
        d->buffer[0] = d->buffer[d->capacity - 1];
        void **start = d->buffer + d->head;
        memmove(start + 1, start, (mcapacity - d->head) * sizeof(void *));
      }
    }

    d->head = (d->head + 1) & mcapacity;
  } else {
    // New tail.
    if (d->head < d->tail) {
      //  _________________________________
      //  | . | . | 1 | 2 | 3 | 4 | 5 | 6 |
      //  ---------------------------------
      //                        x<--------
      void **start = d->buffer + real_index;
      memmove(start, start + 1, (d->tail - real_index) * sizeof(void *));
    } else {
      if (real_index < d->tail) {
        //  _________________________________
        //  | 4 | 5 | 6 | . | . | 1 | 2 | 3 |
        //  ---------------------------------
        //    x<-------
        void **start = d->buffer + real_index;
        memmove(start, start + 1, (d->tail - real_index) * sizeof(void *));
      } else {
        //  _________________________________
        //  | 6 | . | . | 1 | 2 | 3 | 4 | 5 |
        //  ---------------------------------
        //                            x<----
        void **start = d->buffer + real_index;
        memmove(start, start + 1, (mcapacity - real_index) * sizeof(void *));
        d->buffer[d->capacity - 1] = d->buffer[0];
        memmove(d->buffer, d->buffer + 1, d->tail * sizeof(void *));
      }
    }

    d->tail = (d->tail + mcapacity) & mcapacity;
  }

  --d->size;
}

void circular_array_clear(circular_array_t *d)
{
  assert(d != NULL);

  free_data(d);
  d->tail = 0;
  d->head = 0;
  d->size = 0;
}

stat_t circular_array_push_back(circular_array_t *d, void *value)
{
  assert(d != NULL);

  if (should_grow(d)) {
    stat_t stat = grow(d);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  d->buffer[d->tail] = value;
  d->tail = (d->tail + 1) & (d->capacity - 1);
  ++d->size;
  return CDC_STATUS_OK;
}

void circular_array_pop_back(circular_array_t *d)
{
  assert(d != NULL);
  assert(d->size > 0);

  if (CDC_HAS_DFREE(d->dinfo)) {
    d->dinfo->dfree(d->buffer[d->tail]);
  }

  d->tail = (d->tail + d->capacity - 1) & (d->capacity - 1);
  --d->size;
}

stat_t circular_array_push_front(circular_array_t *d, void *value)
{
  assert(d != NULL);

  if (should_grow(d)) {
    stat_t stat = grow(d);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  size_t new_head = (d->head + d->capacity - 1) & (d->capacity - 1);
  d->buffer[new_head] = value;
  d->head = new_head;
  ++d->size;
  return CDC_STATUS_OK;
}

void circular_array_pop_front(circular_array_t *d)
{
  assert(d != NULL);
  assert(d->size > 0);

  if (CDC_HAS_DFREE(d->dinfo)) {
    d->dinfo->dfree(d->buffer[d->head]);
  }

  d->head = (d->head + 1) & (d->capacity - 1);
  --d->size;
}

void circular_array_swap(circular_array_t *a, circular_array_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(size_t, a->capacity, b->capacity);
  CDC_SWAP(size_t, a->head, b->head);
  CDC_SWAP(size_t, a->tail, b->tail);
  CDC_SWAP(void **, a->buffer, b->buffer);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}
