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
#include "cdcontainers/priority-queue.h"
#include "data-info.h"

enum cdc_stat cdc_priority_queue_ctor1(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info, cdc_binary_pred_fn_t compar)
{
  assert(table != NULL);
  assert(q != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  struct cdc_priority_queue *tmp =
      (struct cdc_priority_queue *)malloc(sizeof(struct cdc_priority_queue));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->table = table;
  enum cdc_stat ret = tmp->table->ctor(&tmp->container, info, compar);
  if (ret != CDC_STATUS_OK) {
    free(tmp);
    return ret;
  }

  *q = tmp;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_priority_queue_ctorl1(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info, cdc_binary_pred_fn_t compar, ...)
{
  assert(table != NULL);
  assert(q != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  va_list args;
  va_start(args, compar);
  enum cdc_stat ret = cdc_priority_queue_ctorv1(table, q, info, compar, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_priority_queue_ctorv1(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info, cdc_binary_pred_fn_t compar, va_list args)
{
  assert(table != NULL);
  assert(q != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  struct cdc_priority_queue *tmp =
      (struct cdc_priority_queue *)malloc(sizeof(struct cdc_priority_queue));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->table = table;
  enum cdc_stat ret = tmp->table->ctorv(&tmp->container, info, compar, args);
  if (ret != CDC_STATUS_OK) {
    free(tmp);
    return ret;
  }

  *q = tmp;
  return ret;
}

enum cdc_stat cdc_priority_queue_ctor(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info)
{
  assert(table != NULL);
  assert(q != NULL);
  assert(CDC_HAS_LT(info));

  return cdc_priority_queue_ctor1(table, q, info, NULL);
}

enum cdc_stat cdc_priority_queue_ctorl(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info, ...)
{
  assert(table != NULL);
  assert(q != NULL);
  assert(CDC_HAS_LT(info));

  va_list args;
  va_start(args, info);
  enum cdc_stat ret = cdc_priority_queue_ctorv(table, q, info, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_priority_queue_ctorv(
    const struct cdc_priority_queue_table *table, struct cdc_priority_queue **q,
    struct cdc_data_info *info, va_list args)
{
  assert(table != NULL);
  assert(q != NULL);
  assert(CDC_HAS_LT(info));

  return cdc_priority_queue_ctorv1(table, q, info, NULL, args);
}

void cdc_priority_queue_dtor(struct cdc_priority_queue *q)
{
  assert(q != NULL);

  q->table->dtor(q->container);
  free(q);
}

void cdc_priority_queue_swap(struct cdc_priority_queue *a,
                             struct cdc_priority_queue *b)
{
  assert(a != NULL);
  assert(b != NULL);
  assert(a->table == b->table);

  CDC_SWAP(void *, a->container, b->container);
}

enum cdc_stat cdc_priority_queueh_ctor1(struct cdc_priority_queue **q,
                                        struct cdc_data_info *info,
                                        cdc_binary_pred_fn_t compar)
{
  assert(q != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  return cdc_priority_queue_ctor1(cdc_pq_heap, q, info, compar);
}

enum cdc_stat cdc_priority_queueh_ctorl1(struct cdc_priority_queue **q,
                                         struct cdc_data_info *info,
                                         cdc_binary_pred_fn_t compar, ...)
{
  assert(q != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  va_list args;
  va_start(args, compar);
  enum cdc_stat ret = cdc_priority_queueh_ctorv1(q, info, compar, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_priority_queueh_ctorv1(struct cdc_priority_queue **q,
                                         struct cdc_data_info *info,
                                         cdc_binary_pred_fn_t compar,
                                         va_list args)
{
  assert(q != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  return cdc_priority_queue_ctorv1(cdc_pq_heap, q, info, compar, args);
}

enum cdc_stat cdc_priority_queueh_ctor(struct cdc_priority_queue **q,
                                       struct cdc_data_info *info)
{
  assert(q != NULL);
  assert(CDC_HAS_LT(info));

  return cdc_priority_queueh_ctor1(q, info, NULL);
}

enum cdc_stat cdc_priority_queueh_ctorl(struct cdc_priority_queue **q,
                                        struct cdc_data_info *info, ...)
{
  assert(q != NULL);
  assert(CDC_HAS_LT(info));

  va_list args;
  va_start(args, info);
  enum cdc_stat ret = cdc_priority_queueh_ctorv(q, info, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_priority_queueh_ctorv(struct cdc_priority_queue **q,
                                        struct cdc_data_info *info,
                                        va_list args)
{
  assert(q != NULL);
  assert(CDC_HAS_LT(info));

  return cdc_priority_queueh_ctorv1(q, info, NULL, args);
}
