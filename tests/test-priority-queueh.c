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
#include "test-common.h"

#include "cdcontainers/adapters/priority-queue.h"
#include "cdcontainers/casts.h"
#include "cdcontainers/common.h"

#include <assert.h>
#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

static int gt(const void *a, const void *b)
{
  return CDC_TO_INT(a) > CDC_TO_INT(b);
}

void test_priority_queue_ctor()
{
  priority_queue_t *q = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = gt;

  CU_ASSERT_EQUAL(priority_queue_ctor(cdc_pq_heap, &q, &info), CDC_STATUS_OK);
  CU_ASSERT(priority_queue_empty(q));
  priority_queue_dtor(q);
}

void test_priority_queue_ctorl()
{
  priority_queue_t *h = NULL;
  int a = 2;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = gt;

  CU_ASSERT_EQUAL(
      priority_queue_ctorl(cdc_pq_heap, &h, &info, CDC_FROM_INT(a), CDC_FROM_INT(a), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(priority_queue_size(h), 2);

  CU_ASSERT(CDC_TO_INT(priority_queue_top(h)) == a);
  priority_queue_pop(h);
  CU_ASSERT_EQUAL(priority_queue_size(h), 1);

  CU_ASSERT(CDC_TO_INT(priority_queue_top(h)) == a);
  priority_queue_pop(h);
  CU_ASSERT(priority_queue_empty(h));
  priority_queue_dtor(h);
}

void test_priority_queue_push()
{
  priority_queue_t *h = NULL;
  int a = 0, b = 1, c = 2;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = gt;
  CU_ASSERT_EQUAL(priority_queue_ctor(cdc_pq_heap, &h, &info), CDC_STATUS_OK);

  CU_ASSERT_EQUAL(priority_queue_push(h, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(priority_queue_size(h), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(h)), a);

  CU_ASSERT_EQUAL(priority_queue_push(h, CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(priority_queue_size(h), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(h)), c);

  CU_ASSERT_EQUAL(priority_queue_push(h, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(priority_queue_size(h), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(h)), c);
  priority_queue_dtor(h);
}

void test_priority_queue_pop()
{
  priority_queue_t *h = NULL;
  int a = 0, b = 3, c = 2, d = 1;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = gt;
  CU_ASSERT_EQUAL(priority_queue_ctorl(cdc_pq_heap, &h, &info, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                       CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(h)), b);
  priority_queue_pop(h);
  CU_ASSERT_EQUAL(priority_queue_size(h), 3);

  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(h)), c);
  priority_queue_pop(h);
  CU_ASSERT_EQUAL(priority_queue_size(h), 2);

  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(h)), d);
  priority_queue_pop(h);
  CU_ASSERT_EQUAL(priority_queue_size(h), 1);

  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(h)), a);
  priority_queue_pop(h);
  CU_ASSERT(priority_queue_empty(h));
  priority_queue_dtor(h);
}

void test_priority_queue_top()
{
  priority_queue_t *h = NULL;
  int a = 1, b = 10, c = 2;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = gt;

  CU_ASSERT_EQUAL(priority_queue_ctorl(cdc_pq_heap, &h, &info, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                       CDC_FROM_INT(c), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(h)), b);
  priority_queue_dtor(h);
}

void test_priority_queue_swap()
{
  priority_queue_t *v = NULL, *w = NULL;
  int a = 2, b = 3, c = 4;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = gt;

  CU_ASSERT_EQUAL(priority_queue_ctorl(cdc_pq_heap, &v, &info, CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(
      priority_queue_ctorl(cdc_pq_heap, &w, &info, CDC_FROM_INT(a), CDC_FROM_INT(c), CDC_END),
      CDC_STATUS_OK);

  priority_queue_swap(v, w);
  CU_ASSERT_EQUAL(priority_queue_size(v), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(v)), c);
  CU_ASSERT_EQUAL(priority_queue_size(w), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(priority_queue_top(w)), b);
  priority_queue_dtor(v);
  priority_queue_dtor(w);
}
