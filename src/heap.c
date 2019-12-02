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
#include "cdcontainers/heap.h"

#include "cdcontainers/data-info.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static size_t parent(size_t i) { return (i - 1) / 2; }

static size_t left(size_t i) { return 2 * i + 1; }

static size_t right(size_t i) { return 2 * i + 2; }

static size_t sift_down(struct cdc_heap *h, size_t i)
{
  size_t size = cdc_array_size(h->array);
  void **data = cdc_array_data(h->array);
  bool cond = false;
  do {
    size_t largest;
    size_t l = left(i);
    size_t r = right(i);

    if (l < size && h->array->dinfo->cmp(data[l], data[i])) {
      largest = l;
    } else {
      largest = i;
    }

    if (r < size && h->array->dinfo->cmp(data[r], data[largest])) {
      largest = r;
    }

    cond = largest != i;
    if (cond) {
      CDC_SWAP(void *, data[i], data[largest]);
      i = largest;
    }

  } while (cond);
  return i;
}

static size_t sift_up(struct cdc_heap *h, size_t i)
{
  void **data = cdc_array_data(h->array);
  size_t p = parent(i);
  while (i > 0 && h->array->dinfo->cmp(data[i], data[p])) {
    CDC_SWAP(void *, data[i], data[p]);
    i = parent(i);
    p = parent(i);
  }

  return i;
}

static void build_heap(struct cdc_heap *h)
{
  size_t i = cdc_array_size(h->array) / 2;
  while (i--) {
    sift_down(h, i);
  }
}

static enum cdc_stat init_varg(struct cdc_heap *h, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    enum cdc_stat ret = cdc_array_push_back(h->array, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  build_heap(h);
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_heap_ctor(struct cdc_heap **h, struct cdc_data_info *info)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  struct cdc_heap *tmp = (struct cdc_heap *)calloc(sizeof(struct cdc_heap), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  enum cdc_stat ret = cdc_array_ctor(&tmp->array, info);
  if (ret != CDC_STATUS_OK) {
    free(tmp);
    return ret;
  }

  *h = tmp;
  return ret;
}

enum cdc_stat cdc_heap_ctorl(struct cdc_heap **h, struct cdc_data_info *info,
                             ...)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  enum cdc_stat ret = cdc_heap_ctorv(h, info, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_heap_ctorv(struct cdc_heap **h, struct cdc_data_info *info,
                             va_list args)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  enum cdc_stat ret = cdc_heap_ctor(h, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  return init_varg(*h, args);
}

void cdc_heap_dtor(struct cdc_heap *h)
{
  assert(h != NULL);

  cdc_array_dtor(h->array);
  free(h);
}

void cdc_heap_extract_top(struct cdc_heap *h)
{
  assert(h != NULL);
  assert(cdc_heap_size(h) > 0);

  void *elem = cdc_array_back(h->array);
  cdc_array_pop_back(h->array);

  if (cdc_array_empty(h->array)) {
    return;
  }

  cdc_array_set(h->array, 0, elem);
  sift_down(h, 0);
}

void cdc_heap_change_key(struct cdc_heap *h, struct cdc_heap_iter *pos,
                         void *key)
{
  assert(h != NULL);
  assert(pos != NULL);
  assert(h->array == pos->container);

  void **data = cdc_array_data(h->array);
  bool is_sift_down = h->array->dinfo->cmp(data[pos->current], key);
  cdc_array_set(h->array, pos->current, key);
  if (is_sift_down) {
    pos->current = sift_down(h, pos->current);
  } else {
    pos->current = sift_up(h, pos->current);
  }
}

enum cdc_stat cdc_heap_riinsert(struct cdc_heap *h, void *key,
                                struct cdc_heap_iter *ret)
{
  assert(h != NULL);

  enum cdc_stat stat = cdc_array_push_back(h->array, key);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  size_t i = cdc_array_size(h->array) - 1;
  i = sift_up(h, i);
  if (ret) {
    ret->container = h->array;
    ret->current = i;
  }

  return stat;
}

void cdc_heap_swap(struct cdc_heap *a, struct cdc_heap *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(struct cdc_array *, a->array, b->array);
}

enum cdc_stat cdc_heap_merge(struct cdc_heap *h, struct cdc_heap *other)
{
  assert(h != NULL);
  assert(other != NULL);

  enum cdc_stat ret = cdc_array_append_move(h->array, other->array);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  build_heap(h);
  return CDC_STATUS_OK;
}

bool cdc_heap_is_heap(struct cdc_heap *h)
{
  assert(h != NULL);

  size_t size = cdc_heap_size(h);
  if (size == 0) {
    return true;
  }

  void **data = cdc_array_data(h->array);
  for (size_t i = 1; i < size; ++i) {
    if (h->array->dinfo->cmp(data[i], data[parent(i)])) {
      return false;
    }
  }

  return true;
}
