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

#include "cdcontainers/adapters/queue.h"
#include "cdcontainers/casts.h"

#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

void test_queued_ctor()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    queue_t *q = NULL;

    CU_ASSERT_EQUAL(queue_ctor(tables[t], &q, NULL), CDC_STATUS_OK);
    CU_ASSERT(queue_empty(q));
    queue_dtor(q);
  }
}

void test_queued_ctorl()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 2, b = 3;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    queue_t *q = NULL;

    CU_ASSERT_EQUAL(queue_ctorl(tables[t], &q, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(queue_size(q), 2);

    void *elem = queue_front(q);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
    queue_pop(q);
    CU_ASSERT_EQUAL(queue_size(q), 1);

    elem = queue_front(q);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);
    queue_pop(q);
    CU_ASSERT_EQUAL(queue_size(q), 0);
    queue_dtor(q);
  }
}

void test_queued_push()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 0, b = 1, c = 2;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    queue_t *q = NULL;

    CU_ASSERT_EQUAL(queue_ctor(tables[t], &q, NULL), CDC_STATUS_OK);

    queue_push(q, CDC_FROM_INT(a));
    CU_ASSERT_EQUAL(queue_size(q), 1);
    void *elem = queue_back(q);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

    queue_push(q, CDC_FROM_INT(b));
    CU_ASSERT_EQUAL(queue_size(q), 2);
    elem = queue_back(q);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

    queue_push(q, CDC_FROM_INT(c));
    CU_ASSERT_EQUAL(queue_size(q), 3);
    elem = queue_back(q);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

    queue_dtor(q);
  }
}

void test_queued_pop()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 0, b = 1, c = 2, d = 3;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    queue_t *q = NULL;

    CU_ASSERT_EQUAL(queue_ctorl(tables[t], &q, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                    CDC_STATUS_OK);

    void *elem = queue_front(q);
    queue_pop(q);
    CU_ASSERT_EQUAL(queue_size(q), 3);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

    elem = queue_front(q);
    queue_pop(q);
    CU_ASSERT_EQUAL(queue_size(q), 2);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

    elem = queue_front(q);
    queue_pop(q);
    CU_ASSERT_EQUAL(queue_size(q), 1);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

    elem = queue_front(q);
    queue_pop(q);
    CU_ASSERT(queue_empty(q));
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);
    queue_dtor(q);
  }
}

void test_queued_front()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 1, b = 2;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    queue_t *q = NULL;

    CU_ASSERT_EQUAL(queue_ctorl(tables[t], &q, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(queue_front(q)), a);
    queue_dtor(q);
  }
}

void test_queued_back()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 1, b = 2;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    queue_t *q = NULL;

    CU_ASSERT_EQUAL(queue_ctorl(tables[t], &q, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(queue_back(q)), b);
    queue_dtor(q);
  }
}

void test_queued_swap()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 2, b = 3, c = 4;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    queue_t *v = NULL;
    queue_t *w = NULL;

    CU_ASSERT_EQUAL(queue_ctorl(tables[t], &v, NULL, CDC_FROM_INT(b), CDC_END), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(queue_ctorl(tables[t], &w, NULL, CDC_FROM_INT(a), CDC_FROM_INT(c), CDC_END),
                    CDC_STATUS_OK);
    queue_swap(v, w);

    CU_ASSERT_EQUAL(queue_size(v), 2);
    CU_ASSERT_EQUAL(CDC_TO_INT(queue_back(v)), c);
    CU_ASSERT_EQUAL(queue_size(w), 1);
    CU_ASSERT_EQUAL(CDC_TO_INT(queue_back(w)), b);
    queue_dtor(v);
    queue_dtor(w);
  }
}
