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
#include "test-common.h"

#include "cdcontainers/casts.h"
#include "cdcontainers/queue.h"

#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

void test_queued_ctor()
{
  struct cdc_queue *q = NULL;

  CU_ASSERT_EQUAL(cdc_queue_ctor(cdc_seq_deque, &q, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_queue_empty(q));
  cdc_queue_dtor(q);
}

void test_queued_ctorl()
{
  struct cdc_queue *q = NULL;
  int a = 2, b = 3;

  CU_ASSERT_EQUAL(cdc_queue_ctorl(cdc_seq_deque, &q, NULL, CDC_FROM_INT(a),
                                  CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_queue_size(q), 2);

  void *elem = cdc_queue_front(q);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  CU_ASSERT_EQUAL(cdc_queue_pop(q), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_queue_size(q), 1);

  elem = cdc_queue_front(q);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);
  CU_ASSERT_EQUAL(cdc_queue_pop(q), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_queue_size(q), 0);
  cdc_queue_dtor(q);
}

void test_queued_push()
{
  struct cdc_queue *q = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_queue_ctor(cdc_seq_deque, &q, NULL), CDC_STATUS_OK);

  cdc_queue_push(q, CDC_FROM_INT(a));
  CU_ASSERT_EQUAL(cdc_queue_size(q), 1);
  void *elem = cdc_queue_back(q);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

  cdc_queue_push(q, CDC_FROM_INT(b));
  CU_ASSERT_EQUAL(cdc_queue_size(q), 2);
  elem = cdc_queue_back(q);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  cdc_queue_push(q, CDC_FROM_INT(c));
  CU_ASSERT_EQUAL(cdc_queue_size(q), 3);
  elem = cdc_queue_back(q);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  cdc_queue_dtor(q);
}

void test_queued_pop()
{
  struct cdc_queue *q = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_queue_ctorl(cdc_seq_deque, &q, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                      CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
      CDC_STATUS_OK);

  void *elem = cdc_queue_front(q);
  CU_ASSERT_EQUAL(cdc_queue_pop(q), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_queue_size(q), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

  elem = cdc_queue_front(q);
  CU_ASSERT_EQUAL(cdc_queue_pop(q), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_queue_size(q), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = cdc_queue_front(q);
  CU_ASSERT_EQUAL(cdc_queue_pop(q), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_queue_size(q), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = cdc_queue_front(q);
  CU_ASSERT_EQUAL(cdc_queue_pop(q), CDC_STATUS_OK);
  CU_ASSERT(cdc_queue_empty(q));
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);
  cdc_queue_dtor(q);
}

void test_queued_front()
{
  struct cdc_queue *q = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(cdc_queue_ctorl(cdc_seq_deque, &q, NULL, CDC_FROM_INT(a),
                                  CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_queue_front(q)), a);
  cdc_queue_dtor(q);
}

void test_queued_back()
{
  struct cdc_queue *q = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(cdc_queue_ctorl(cdc_seq_deque, &q, NULL, CDC_FROM_INT(a),
                                  CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_queue_back(q)), b);
  cdc_queue_dtor(q);
}

void test_queued_swap()
{
  struct cdc_queue *v = NULL;
  struct cdc_queue *w = NULL;
  int a = 2, b = 3, c = 4;

  CU_ASSERT_EQUAL(
      cdc_queue_ctorl(cdc_seq_deque, &v, NULL, CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_queue_ctorl(cdc_seq_deque, &w, NULL, CDC_FROM_INT(a),
                                  CDC_FROM_INT(c), CDC_END),
                  CDC_STATUS_OK);
  cdc_queue_swap(v, w);

  CU_ASSERT_EQUAL(cdc_queue_size(v), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_queue_back(v)), c);
  CU_ASSERT_EQUAL(cdc_queue_size(w), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_queue_back(w)), b);
  cdc_queue_dtor(v);
  cdc_queue_dtor(w);
}
