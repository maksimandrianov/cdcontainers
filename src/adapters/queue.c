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
#include "cdcontainers/adapters/queue.h"

stat_t queue_ctor(const sequence_table_t *table, queue_t **q, data_info_t *info)
{
  assert(table != NULL);
  assert(q != NULL);

  queue_t *tmp = (queue_t *)malloc(sizeof(queue_t));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->table = table;
  stat_t ret = tmp->table->ctor(&tmp->container, info);
  if (ret != CDC_STATUS_OK) {
    free(tmp);
    return ret;
  }

  *q = tmp;
  return CDC_STATUS_OK;
}

stat_t queue_ctorl(const sequence_table_t *table, queue_t **q, data_info_t *info, ...)
{
  assert(table != NULL);
  assert(q != NULL);

  va_list args;
  va_start(args, info);
  stat_t ret = queue_ctorv(table, q, info, args);
  va_end(args);
  return ret;
}

stat_t queue_ctorv(const sequence_table_t *table, queue_t **q, data_info_t *info, va_list args)
{
  assert(table != NULL);
  assert(q != NULL);

  queue_t *tmp = (queue_t *)malloc(sizeof(queue_t));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->table = table;
  stat_t ret = tmp->table->ctorv(&tmp->container, info, args);
  if (ret != CDC_STATUS_OK) {
    free(tmp);
    return ret;
  }

  *q = tmp;
  return ret;
}

void queue_dtor(queue_t *q)
{
  assert(q != NULL);

  q->table->dtor(q->container);
  free(q);
}

void queue_swap(queue_t *a, queue_t *b)
{
  assert(a != NULL);
  assert(b != NULL);
  assert(a->table == b->table);

  CDC_SWAP(void *, a->container, b->container);
}
