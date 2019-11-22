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
#include "cdcontainers/deque.h"

#include <assert.h>
#include <float.h>
#include <stdarg.h>
#include <stdio.h>

#include <CUnit/Basic.h>

static bool deque_range_int_eq(struct cdc_deque *d, size_t count, ...)
{
  assert(cdc_deque_size(d) >= count);

  va_list args;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    int elem = va_arg(args, int);
    enum cdc_stat ret;
    void *val = NULL;
    if ((ret = cdc_deque_at(d, i, &val)) != CDC_STATUS_OK ||
        elem != CDC_PTR_TO_INT(val)) {
      return false;
    }
  }

  return true;
}

static inline void deque_range_int_print(struct cdc_deque *d)
{
  void *val = NULL;
  for (size_t i = 0; i < cdc_deque_size(d); ++i) {
    if (cdc_deque_at(d, i, &val) == CDC_STATUS_OK) {
      printf("%d ", CDC_PTR_TO_INT(val));
    }
  }
  printf("\n");
}

void test_deque_ctor()
{
  struct cdc_deque *d = NULL;

  CU_ASSERT_EQUAL(cdc_deque_ctor(&d, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_deque_empty(d));
  cdc_deque_dtor(d);
}

void test_deque_ctorl()
{
  struct cdc_deque *deq = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&deq, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b),
                      CDC_INT_TO_PTR(c), CDC_INT_TO_PTR(d), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 4);
  CU_ASSERT(deque_range_int_eq(deq, 4, a, b, c, d));
  cdc_deque_dtor(deq);
}

void test_deque_push_back()
{
  struct cdc_deque *d = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_deque_ctor(&d, NULL), CDC_STATUS_OK);

  cdc_deque_push_back(d, CDC_INT_TO_PTR(a));
  CU_ASSERT_EQUAL(cdc_deque_size(d), 1);
  CU_ASSERT(deque_range_int_eq(d, 1, a));

  cdc_deque_push_back(d, CDC_INT_TO_PTR(b));
  CU_ASSERT_EQUAL(cdc_deque_size(d), 2);
  CU_ASSERT(deque_range_int_eq(d, 1, a, b));

  cdc_deque_push_back(d, CDC_INT_TO_PTR(c));
  CU_ASSERT_EQUAL(cdc_deque_size(d), 3);
  CU_ASSERT(deque_range_int_eq(d, 3, a, b, c));

  cdc_deque_push_back(d, CDC_INT_TO_PTR(a));
  CU_ASSERT_EQUAL(cdc_deque_size(d), 4);
  CU_ASSERT(deque_range_int_eq(d, 4, a, b, c, a));

  cdc_deque_push_back(d, CDC_INT_TO_PTR(b));
  CU_ASSERT_EQUAL(cdc_deque_size(d), 5);
  CU_ASSERT(deque_range_int_eq(d, 5, a, b, c, a, b));
  cdc_deque_dtor(d);
}

void test_deque_push_front()
{
  struct cdc_deque *d = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_deque_ctor(&d, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_deque_empty(d));

  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 1);
  CU_ASSERT(deque_range_int_eq(d, 1, a));

  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 2);
  CU_ASSERT(deque_range_int_eq(d, 2, b, a));

  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 3);
  CU_ASSERT(deque_range_int_eq(d, 3, c, b, a));

  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 4);
  CU_ASSERT(deque_range_int_eq(d, 4, a, c, b, a));

  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 5);
  CU_ASSERT(deque_range_int_eq(d, 4, b, a, c, b, a));
  cdc_deque_dtor(d);
}

void test_deque_get()
{
  struct cdc_deque *d = NULL;
  int a = 0, b = 1;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&d, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(cdc_deque_get(d, 0)), a);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(cdc_deque_get(d, 1)), b);
  cdc_deque_dtor(d);
}

void test_deque_at()
{
  struct cdc_deque *d = NULL;
  int a = 0, b = 1, c = 2;
  void *elem = NULL;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&d, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b),
                      CDC_INT_TO_PTR(c), CDC_END),
      CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_deque_at(d, 0, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), a);
  CU_ASSERT_EQUAL(cdc_deque_at(d, 1, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), b);
  CU_ASSERT_EQUAL(cdc_deque_at(d, 2, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), c);

  size_t index = cdc_deque_size(d) + 1;
  CU_ASSERT_EQUAL(cdc_deque_at(d, index, &elem), CDC_STATUS_OUT_OF_RANGE);
  cdc_deque_dtor(d);
}

void test_deque_set()
{
  struct cdc_deque *d = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&d, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b), CDC_END),
      CDC_STATUS_OK);

  cdc_deque_set(d, 0, CDC_INT_TO_PTR(b));
  CU_ASSERT(deque_range_int_eq(d, 2, b, b));

  cdc_deque_set(d, 1, CDC_INT_TO_PTR(a));
  CU_ASSERT(deque_range_int_eq(d, 2, b, a));
  cdc_deque_dtor(d);
}

void test_deque_front()
{
  struct cdc_deque *d = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&d, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(cdc_deque_front(d)), a);

  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(cdc_deque_front(d)), b);
  cdc_deque_dtor(d);
}

void test_deque_back()
{
  struct cdc_deque *d = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&d, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(cdc_deque_back(d)), b);

  CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(cdc_deque_back(d)), a);
  cdc_deque_dtor(d);
}

void test_deque_pop_back()
{
  struct cdc_deque *deq = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&deq, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b),
                      CDC_INT_TO_PTR(c), CDC_INT_TO_PTR(d), CDC_END),
      CDC_STATUS_OK);

  void *elem = cdc_deque_back(deq);
  CU_ASSERT_EQUAL(cdc_deque_pop_back(deq), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), d);

  elem = cdc_deque_back(deq);
  CU_ASSERT_EQUAL(cdc_deque_pop_back(deq), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 2);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), c);

  elem = cdc_deque_back(deq);
  CU_ASSERT_EQUAL(cdc_deque_pop_back(deq), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 1);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), b);

  elem = cdc_deque_back(deq);
  CU_ASSERT_EQUAL(cdc_deque_pop_back(deq), CDC_STATUS_OK);
  CU_ASSERT(cdc_deque_empty(deq));
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), a);
  cdc_deque_dtor(deq);
}

void test_deque_pop_front()
{
  struct cdc_deque *deq = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&deq, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b),
                      CDC_INT_TO_PTR(c), CDC_INT_TO_PTR(d), CDC_END),
      CDC_STATUS_OK);

  void *elem = cdc_deque_front(deq);
  CU_ASSERT_EQUAL(cdc_deque_pop_front(deq), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), a);

  elem = cdc_deque_front(deq);
  CU_ASSERT_EQUAL(cdc_deque_pop_front(deq), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 2);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), b);

  elem = cdc_deque_front(deq);
  CU_ASSERT_EQUAL(cdc_deque_pop_front(deq), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 1);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), c);

  elem = cdc_deque_front(deq);
  CU_ASSERT_EQUAL(cdc_deque_pop_front(deq), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 0);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), d);
  cdc_deque_dtor(deq);
}

void test_deque_swap()
{
  struct cdc_deque *v = NULL;
  struct cdc_deque *w = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&v, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b),
                      CDC_INT_TO_PTR(c), CDC_INT_TO_PTR(d), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&w, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(d), CDC_END),
      CDC_STATUS_OK);

  cdc_deque_swap(v, w);
  CU_ASSERT_EQUAL(cdc_deque_size(v), 2);
  CU_ASSERT(deque_range_int_eq(v, 2, a, d));
  CU_ASSERT_EQUAL(cdc_deque_size(w), 4);
  CU_ASSERT(deque_range_int_eq(w, 4, a, b, c, d));
  cdc_deque_dtor(v);
  cdc_deque_dtor(w);
}

void test_deque_insert()
{
  struct cdc_deque *d = NULL;
  int a = 1, b = 2, insert_count = 100;
  int i1 = 3, i2 = 4, i3 = 5;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&d, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_insert(d, 0, CDC_INT_TO_PTR(i1)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 3);
  CU_ASSERT(deque_range_int_eq(d, 3, i1, a, b));

  CU_ASSERT_EQUAL(cdc_deque_insert(d, cdc_deque_size(d), CDC_INT_TO_PTR(i2)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 4);
  CU_ASSERT(deque_range_int_eq(d, 4, i1, a, b, i2));

  CU_ASSERT_EQUAL(cdc_deque_insert(d, 2, CDC_INT_TO_PTR(i3)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 5);
  CU_ASSERT(deque_range_int_eq(d, 5, i1, a, i3, b, i2));

  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(i1)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(i2)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(i3)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(i1)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(i3)), CDC_STATUS_OK);
  CU_ASSERT(deque_range_int_eq(d, 14, i3, i1, b, a, i3, i2, i1, b, a, i1, a, i3,
                               b, i2));

  CU_ASSERT_EQUAL(cdc_deque_insert(d, 1, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
  CU_ASSERT(deque_range_int_eq(d, 15, i3, a, i1, b, a, i3, i2, i1, b, a, i1, a,
                               i3, b, i2));

  CU_ASSERT_EQUAL(cdc_deque_insert(d, cdc_deque_size(d) - 2, CDC_INT_TO_PTR(a)),
                  CDC_STATUS_OK);
  CU_ASSERT(deque_range_int_eq(d, 16, i3, a, i1, b, a, i3, i2, i1, b, a, i1, a,
                               i3, a, b, i2));

  cdc_deque_clear(d);
  CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_INT_TO_PTR(0)), CDC_STATUS_OK);
  for (int i = 0; i <= insert_count; ++i) {
    CU_ASSERT_EQUAL(cdc_deque_insert(d, 1, CDC_INT_TO_PTR(i)), CDC_STATUS_OK);
  }

  CU_ASSERT_EQUAL(cdc_deque_get(d, 0), CDC_INT_TO_PTR(0));
  for (int i = insert_count; i >= 0; --i) {
    CU_ASSERT_EQUAL(cdc_deque_get(d, (size_t)(insert_count - i + 1)),
                    CDC_INT_TO_PTR(i));
  }

  cdc_deque_dtor(d);
}

void test_deque_erase()
{
  struct cdc_deque *deq = NULL;
  int a = 0, b = 1, c = 2, d = 3, erase_count = 100;
  void *elem = NULL;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&deq, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b),
                      CDC_INT_TO_PTR(c), CDC_INT_TO_PTR(d), CDC_INT_TO_PTR(a),
                      CDC_INT_TO_PTR(b), CDC_INT_TO_PTR(c), CDC_END),
      CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_deque_remove(deq, 2, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), c);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 6);
  CU_ASSERT(deque_range_int_eq(deq, 3, a, b, d, a, b, c));

  CU_ASSERT_EQUAL(cdc_deque_remove(deq, 4, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), b);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 5);
  CU_ASSERT(deque_range_int_eq(deq, 5, a, b, d, a, c));

  CU_ASSERT_EQUAL(cdc_deque_remove(deq, 0, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), a);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 4);
  CU_ASSERT(deque_range_int_eq(deq, 4, b, d, a, c));

  CU_ASSERT_EQUAL(cdc_deque_remove(deq, cdc_deque_size(deq) - 1, &elem),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), c);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
  CU_ASSERT(deque_range_int_eq(deq, 3, b, d, a));

  cdc_deque_clear(deq);

  CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_INT_TO_PTR(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_INT_TO_PTR(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_INT_TO_PTR(d)), CDC_STATUS_OK);
  CU_ASSERT(deque_range_int_eq(deq, 4, d, c, b, a));
  CU_ASSERT_EQUAL(cdc_deque_remove(deq, 1, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), c);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
  CU_ASSERT(deque_range_int_eq(deq, 3, d, b, a));

  CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_INT_TO_PTR(d)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_INT_TO_PTR(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_INT_TO_PTR(d)), CDC_STATUS_OK);
  CU_ASSERT(deque_range_int_eq(deq, 7, d, d, b, a, d, c, a));
  CU_ASSERT_EQUAL(cdc_deque_remove(deq, cdc_deque_size(deq) - 2, &elem),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), c);
  CU_ASSERT_EQUAL(cdc_deque_size(deq), 6);
  CU_ASSERT(deque_range_int_eq(deq, 6, d, d, b, a, d, a));

  cdc_deque_clear(deq);
  CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_INT_TO_PTR(0)), CDC_STATUS_OK);
  for (int i = 0; i <= erase_count; ++i) {
    CU_ASSERT_EQUAL(cdc_deque_insert(deq, 1, CDC_INT_TO_PTR(i)), CDC_STATUS_OK);
  }

  for (int i = erase_count; i >= 0; --i) {
    CU_ASSERT_EQUAL(cdc_deque_remove(deq, 1, &elem), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_PTR_TO_INT(elem), i);
  }

  CU_ASSERT_EQUAL(cdc_deque_size(deq), 1);
  CU_ASSERT_EQUAL(cdc_deque_get(deq, 0), CDC_INT_TO_PTR(0));
  cdc_deque_dtor(deq);
}

void test_deque_clear()
{
  struct cdc_deque *deq = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&deq, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b),
                      CDC_INT_TO_PTR(c), CDC_INT_TO_PTR(d), CDC_END),
      CDC_STATUS_OK);
  cdc_deque_clear(deq);
  CU_ASSERT(cdc_deque_empty(deq));
  cdc_deque_dtor(deq);
}

void test_deque_pop_push()
{
  struct cdc_deque *d;
  int a = 1, b = 2, c = 3;
  int i1 = 3, i2 = 4, i3 = 5;

  CU_ASSERT_EQUAL(
      cdc_deque_ctorl(&d, NULL, CDC_INT_TO_PTR(a), CDC_INT_TO_PTR(b),
                      CDC_INT_TO_PTR(c), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 3);

  for (int i = 0; i < 5; ++i) {
    CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_pop_back(d), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_INT_TO_PTR(b)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_pop_back(d), CDC_STATUS_OK)
  }

  CU_ASSERT(deque_range_int_eq(d, 3, b, a, b));
  CU_ASSERT_EQUAL(cdc_deque_size(d), 3);

  for (int i = 0; i < 5; ++i) {
    CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_INT_TO_PTR(a)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_pop_front(d), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_INT_TO_PTR(b)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_pop_front(d), CDC_STATUS_OK);
  }
  CU_ASSERT(deque_range_int_eq(d, 3, b, a, b));
  CU_ASSERT_EQUAL(cdc_deque_size(d), 3);

  CU_ASSERT_EQUAL(cdc_deque_insert(d, 0, CDC_INT_TO_PTR(i1)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 4);
  CU_ASSERT(deque_range_int_eq(d, 4, i1, b, a, b));

  CU_ASSERT_EQUAL(cdc_deque_insert(d, cdc_deque_size(d), CDC_INT_TO_PTR(i2)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 5);
  CU_ASSERT(deque_range_int_eq(d, 5, i1, b, a, b, i2));

  CU_ASSERT_EQUAL(cdc_deque_insert(d, 2, CDC_INT_TO_PTR(i3)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 6);
  CU_ASSERT(deque_range_int_eq(d, 6, i1, b, i3, a, b, i2));

  CU_ASSERT_EQUAL(cdc_deque_insert(d, 4, CDC_INT_TO_PTR(i1)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 7);
  CU_ASSERT(deque_range_int_eq(d, 7, i1, b, i3, a, i1, b, i2));

  CU_ASSERT_EQUAL(cdc_deque_insert(d, 1, CDC_INT_TO_PTR(i2)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_deque_size(d), 8);
  CU_ASSERT(deque_range_int_eq(d, 8, i1, i2, b, i3, a, i1, b, i2));
  cdc_deque_dtor(d);
}
