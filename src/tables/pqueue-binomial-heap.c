// The MIT License (MIT)
// Copyright (c) 2018 Maksim Andrianov
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
#include "cdcontainers/binomial-heap.h"
#include "cdcontainers/tables/ipqueue.h"

#include <assert.h>

static stat_t ctor(void **cntr, data_info_t *info)
{
  assert(cntr != NULL);

  binomial_heap_t **heap = (binomial_heap_t **)cntr;
  return binomial_heap_ctor(heap, info);
}

static stat_t ctorv(void **cntr, data_info_t *info, va_list args)
{
  assert(cntr != NULL);

  binomial_heap_t **heap = (binomial_heap_t **)cntr;
  return binomial_heap_ctorv(heap, info, args);
}

static void dtor(void *cntr)
{
  assert(cntr != NULL);

  binomial_heap_t *heap = (binomial_heap_t *)cntr;
  binomial_heap_dtor(heap);
}

static void *top(void *cntr)
{
  assert(cntr != NULL);

  binomial_heap_t *heap = (binomial_heap_t *)cntr;
  return binomial_heap_top(heap);
}

static bool empty(void *cntr)
{
  assert(cntr != NULL);

  binomial_heap_t *heap = (binomial_heap_t *)cntr;
  return binomial_heap_empty(heap);
}

static size_t size(void *cntr)
{
  assert(cntr != NULL);

  binomial_heap_t *heap = (binomial_heap_t *)cntr;
  return binomial_heap_size(heap);
}

static stat_t push(void *cntr, void *elem)
{
  assert(cntr != NULL);

  binomial_heap_t *heap = (binomial_heap_t *)cntr;
  return binomial_heap_insert(heap, elem);
}

static void pop(void *cntr)
{
  assert(cntr != NULL);

  binomial_heap_t *heap = (binomial_heap_t *)cntr;
  binomial_heap_extract_top(heap);
}

static const priority_queue_table_t _table = {.ctor = ctor,
                                              .ctorv = ctorv,
                                              .dtor = dtor,
                                              .top = top,
                                              .empty = empty,
                                              .size = size,
                                              .push = push,
                                              .pop = pop};

const priority_queue_table_t *cdc_pq_binheap = &_table;
