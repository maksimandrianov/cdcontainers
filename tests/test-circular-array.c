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
#include "cdcontainers/circular-array.h"

#include <assert.h>
#include <float.h>
#include <stdarg.h>
#include <stdio.h>

#include <CUnit/Basic.h>

static bool circular_array_range_int_eq(struct cdc_circular_array *d,
                                        size_t count, ...)
{
  assert(cdc_circular_array_size(d) >= count);

  va_list args;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    int elem = va_arg(args, int);
    enum cdc_stat ret;
    void *val = NULL;
    if ((ret = cdc_circular_array_at(d, i, &val)) != CDC_STATUS_OK ||
        elem != CDC_TO_INT(val)) {
      return false;
    }
  }

  return true;
}

static inline void circular_array_range_int_print(struct cdc_circular_array *d)
{
  void *val = NULL;
  for (size_t i = 0; i < cdc_circular_array_size(d); ++i) {
    if (cdc_circular_array_at(d, i, &val) == CDC_STATUS_OK) {
      printf("%d ", CDC_TO_INT(val));
    }
  }
  printf("\n");
}

void test_circular_array_ctor()
{
  struct cdc_circular_array *d = NULL;

  CU_ASSERT_EQUAL(cdc_circular_array_ctor(&d, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_circular_array_empty(d));
  cdc_circular_array_dtor(d);
}

void test_circular_array_ctorl()
{
  struct cdc_circular_array *arr = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                               CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
      CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 4);
  CU_ASSERT(circular_array_range_int_eq(arr, 4, a, b, c, d));
  cdc_circular_array_dtor(arr);
}

void test_circular_array_push_back()
{
  struct cdc_circular_array *arr = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_circular_array_ctor(&arr, NULL), CDC_STATUS_OK);

  cdc_circular_array_push_back(arr, CDC_FROM_INT(a));
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 1);
  CU_ASSERT(circular_array_range_int_eq(arr, 1, a));

  cdc_circular_array_push_back(arr, CDC_FROM_INT(b));
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 2);
  CU_ASSERT(circular_array_range_int_eq(arr, 1, a, b));

  cdc_circular_array_push_back(arr, CDC_FROM_INT(c));
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);
  CU_ASSERT(circular_array_range_int_eq(arr, 3, a, b, c));

  cdc_circular_array_push_back(arr, CDC_FROM_INT(a));
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 4);
  CU_ASSERT(circular_array_range_int_eq(arr, 4, a, b, c, a));

  cdc_circular_array_push_back(arr, CDC_FROM_INT(b));
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 5);
  CU_ASSERT(circular_array_range_int_eq(arr, 5, a, b, c, a, b));
  cdc_circular_array_dtor(arr);
}

void test_circular_array_push_front()
{
  struct cdc_circular_array *arr = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_circular_array_ctor(&arr, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_circular_array_empty(arr));

  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(a)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 1);
  CU_ASSERT(circular_array_range_int_eq(arr, 1, a));

  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(b)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 2);
  CU_ASSERT(circular_array_range_int_eq(arr, 2, b, a));

  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(c)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);
  CU_ASSERT(circular_array_range_int_eq(arr, 3, c, b, a));

  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(a)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 4);
  CU_ASSERT(circular_array_range_int_eq(arr, 4, a, c, b, a));

  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(b)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 5);
  CU_ASSERT(circular_array_range_int_eq(arr, 4, b, a, c, b, a));
  cdc_circular_array_dtor(arr);
}

void test_circular_array_get()
{
  struct cdc_circular_array *arr = NULL;
  int a = 0, b = 1;

  CU_ASSERT_EQUAL(cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(a),
                                           CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_circular_array_get(arr, 0)), a);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_circular_array_get(arr, 1)), b);
  cdc_circular_array_dtor(arr);
}

void test_circular_array_at()
{
  struct cdc_circular_array *arr = NULL;
  int a = 0, b = 1, c = 2;
  void *elem = NULL;

  CU_ASSERT_EQUAL(
      cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                               CDC_FROM_INT(c), CDC_END),
      CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_circular_array_at(arr, 0, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  CU_ASSERT_EQUAL(cdc_circular_array_at(arr, 1, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);
  CU_ASSERT_EQUAL(cdc_circular_array_at(arr, 2, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  size_t index = cdc_circular_array_size(arr) + 1;
  CU_ASSERT_EQUAL(cdc_circular_array_at(arr, index, &elem),
                  CDC_STATUS_OUT_OF_RANGE);
  cdc_circular_array_dtor(arr);
}

void test_circular_array_set()
{
  struct cdc_circular_array *d = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(a),
                                           CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);

  cdc_circular_array_set(d, 0, CDC_FROM_INT(b));
  CU_ASSERT(circular_array_range_int_eq(d, 2, b, b));

  cdc_circular_array_set(d, 1, CDC_FROM_INT(a));
  CU_ASSERT(circular_array_range_int_eq(d, 2, b, a));
  cdc_circular_array_dtor(d);
}

void test_circular_array_front()
{
  struct cdc_circular_array *d = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(a),
                                           CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_circular_array_front(d)), a);

  CU_ASSERT_EQUAL(cdc_circular_array_push_front(d, CDC_FROM_INT(b)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_circular_array_front(d)), b);
  cdc_circular_array_dtor(d);
}

void test_circular_array_back()
{
  struct cdc_circular_array *d = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(a),
                                           CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_circular_array_back(d)), b);

  CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(a)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_circular_array_back(d)), a);
  cdc_circular_array_dtor(d);
}

void test_circular_array_pop_back()
{
  struct cdc_circular_array *arr = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                               CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
      CDC_STATUS_OK);

  void *elem = cdc_circular_array_back(arr);
  cdc_circular_array_pop_back(arr);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);

  elem = cdc_circular_array_back(arr);
  cdc_circular_array_pop_back(arr);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = cdc_circular_array_back(arr);
  cdc_circular_array_pop_back(arr);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = cdc_circular_array_back(arr);
  cdc_circular_array_pop_back(arr);
  CU_ASSERT(cdc_circular_array_empty(arr));
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  cdc_circular_array_dtor(arr);
}

void test_circular_array_pop_front()
{
  struct cdc_circular_array *arr = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                               CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
      CDC_STATUS_OK);

  void *elem = cdc_circular_array_front(arr);
  cdc_circular_array_pop_front(arr);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

  elem = cdc_circular_array_front(arr);
  cdc_circular_array_pop_front(arr);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = cdc_circular_array_front(arr);
  cdc_circular_array_pop_front(arr);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = cdc_circular_array_front(arr);
  cdc_circular_array_pop_front(arr);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 0);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);
  cdc_circular_array_dtor(arr);
}

void test_circular_array_swap()
{
  struct cdc_circular_array *v = NULL;
  struct cdc_circular_array *w = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_circular_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                               CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_ctorl(&w, NULL, CDC_FROM_INT(a),
                                           CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_circular_array_swap(v, w);
  CU_ASSERT_EQUAL(cdc_circular_array_size(v), 2);
  CU_ASSERT(circular_array_range_int_eq(v, 2, a, d));
  CU_ASSERT_EQUAL(cdc_circular_array_size(w), 4);
  CU_ASSERT(circular_array_range_int_eq(w, 4, a, b, c, d));
  cdc_circular_array_dtor(v);
  cdc_circular_array_dtor(w);
}

void test_circular_array_insert()
{
  struct cdc_circular_array *arr = NULL;
  int a = 1, b = 2, insert_count = 100;
  int i1 = 3, i2 = 4, i3 = 5;
  int v0 = -1, v1 = 0, v2 = 1, v3 = 2, v4 = 3, v5 = 4, v6 = 5;

  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 2, CDC_FROM_INT(v0)),
                    CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 7, v1, v2, v0, v3, v4, v5, v6));
    cdc_circular_array_dtor(arr);
  }
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(arr);
    CU_ASSERT(circular_array_range_int_eq(arr, 5, v2, v3, v4, v5, v6));
    CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 1, CDC_FROM_INT(v0)),
                    CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 6, v2, v0, v3, v4, v5, v6));
    cdc_circular_array_dtor(arr);
  }
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v1)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v2)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v3)),
                    CDC_STATUS_OK);

    CU_ASSERT(circular_array_range_int_eq(arr, 6, v4, v5, v6, v1, v2, v3));
    CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 1, CDC_FROM_INT(v0)),
                    CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 7, v4, v0, v5, v6, v1, v2, v3));
    cdc_circular_array_dtor(arr);
  }
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v1)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v2)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v3)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v4)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v5)),
                    CDC_STATUS_OK);

    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v4)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v5)),
                    CDC_STATUS_OK);

    CU_ASSERT(circular_array_range_int_eq(arr, 6, v2, v3, v4, v5, v4, v5));
    CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 1, CDC_FROM_INT(v0)),
                    CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 7, v2, v0, v3, v4, v5, v4, v5));
    cdc_circular_array_dtor(arr);
  }
  {
    CU_ASSERT_EQUAL(cdc_circular_array_ctorl(
                        &arr, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v1),
                        CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                        CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
                    CDC_STATUS_OK);

    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);

    CU_ASSERT(circular_array_range_int_eq(arr, 5, v2, v3, v4, v5, v6));
    CU_ASSERT_EQUAL(
        cdc_circular_array_insert(arr, cdc_circular_array_size(arr) - 2,
                                  CDC_FROM_INT(v0)),
        CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 6, v2, v3, v4, v0, v5, v6));
    cdc_circular_array_dtor(arr);
  }

  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(arr);

    CU_ASSERT(circular_array_range_int_eq(arr, 5, v2, v3, v4, v5, v6));
    CU_ASSERT_EQUAL(
        cdc_circular_array_insert(arr, cdc_circular_array_size(arr) - 2,
                                  CDC_FROM_INT(v0)),
        CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 6, v2, v3, v4, v0, v5, v6));
    cdc_circular_array_dtor(arr);
  }
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v1)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v2)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v3)),
                    CDC_STATUS_OK);

    CU_ASSERT(circular_array_range_int_eq(arr, 6, v4, v5, v6, v1, v2, v3));
    CU_ASSERT_EQUAL(
        cdc_circular_array_insert(arr, cdc_circular_array_size(arr) - 1,
                                  CDC_FROM_INT(v0)),
        CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 7, v4, v5, v6, v1, v2, v0, v3));
    cdc_circular_array_dtor(arr);
  }
  {
    CU_ASSERT_EQUAL(cdc_circular_array_ctorl(
                        &arr, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v1),
                        CDC_FROM_INT(v1), CDC_FROM_INT(v2), CDC_FROM_INT(v3),
                        CDC_FROM_INT(v4), CDC_FROM_INT(v5), CDC_END),
                    CDC_STATUS_OK);

    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);
    cdc_circular_array_pop_front(arr);

    CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(v1)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v6)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(v5)),
                    CDC_STATUS_OK);

    CU_ASSERT(circular_array_range_int_eq(arr, 6, v1, v3, v4, v5, v6, v5));
    CU_ASSERT_EQUAL(
        cdc_circular_array_insert(arr, cdc_circular_array_size(arr) - 3,
                                  CDC_FROM_INT(v0)),
        CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 7, v1, v3, v4, v0, v5, v6, v5));
    cdc_circular_array_dtor(arr);
  }

  CU_ASSERT_EQUAL(cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(a),
                                           CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 0, CDC_FROM_INT(i1)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);
  CU_ASSERT(circular_array_range_int_eq(arr, 3, i1, a, b));

  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, cdc_circular_array_size(arr),
                                            CDC_FROM_INT(i2)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 4);
  CU_ASSERT(circular_array_range_int_eq(arr, 4, i1, a, b, i2));

  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 2, CDC_FROM_INT(i3)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 5);
  CU_ASSERT(circular_array_range_int_eq(arr, 5, i1, a, i3, b, i2));

  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(a)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(b)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(i1)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(i2)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(i3)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(a)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(b)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(i1)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(i3)),
                  CDC_STATUS_OK);
  CU_ASSERT(circular_array_range_int_eq(arr, 14, i3, i1, b, a, i3, i2, i1, b, a,
                                        i1, a, i3, b, i2));

  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 1, CDC_FROM_INT(a)),
                  CDC_STATUS_OK);
  CU_ASSERT(circular_array_range_int_eq(arr, 15, i3, a, i1, b, a, i3, i2, i1, b,
                                        a, i1, a, i3, b, i2));

  CU_ASSERT_EQUAL(cdc_circular_array_insert(
                      arr, cdc_circular_array_size(arr) - 2, CDC_FROM_INT(a)),
                  CDC_STATUS_OK);
  CU_ASSERT(circular_array_range_int_eq(arr, 16, i3, a, i1, b, a, i3, i2, i1, b,
                                        a, i1, a, i3, a, b, i2));

  cdc_circular_array_clear(arr);
  CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(0)),
                  CDC_STATUS_OK);
  for (int i = 0; i <= insert_count; ++i) {
    CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 1, CDC_FROM_INT(i)),
                    CDC_STATUS_OK);
  }

  CU_ASSERT_EQUAL(cdc_circular_array_get(arr, 0), CDC_FROM_INT(0));
  for (int i = insert_count; i >= 0; --i) {
    CU_ASSERT_EQUAL(cdc_circular_array_get(arr, (size_t)(insert_count - i + 1)),
                    CDC_FROM_INT(i));
  }

  cdc_circular_array_dtor(arr);
}

void test_circular_array_erase()
{
  struct cdc_circular_array *d = NULL;
  int v1 = 0, v2 = 1, v3 = 2, v4 = 3, v5 = 4, v6 = 5;
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_erase(d, 2);
    CU_ASSERT(circular_array_range_int_eq(d, 5, v1, v2, v4, v5, v6));
    cdc_circular_array_dtor(d);
  }
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(d);
    CU_ASSERT(circular_array_range_int_eq(d, 5, v2, v3, v4, v5, v6));
    cdc_circular_array_erase(d, 1);
    CU_ASSERT(circular_array_range_int_eq(d, 4, v2, v4, v5, v6));
    cdc_circular_array_dtor(d);
  }
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v1)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v2)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v3)),
                    CDC_STATUS_OK);

    CU_ASSERT(circular_array_range_int_eq(d, 6, v4, v5, v6, v1, v2, v3));
    cdc_circular_array_erase(d, 1);
    CU_ASSERT(circular_array_range_int_eq(d, 5, v4, v6, v1, v2, v3));
    cdc_circular_array_dtor(d);
  }
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v1)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v2)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v3)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v4)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v5)),
                    CDC_STATUS_OK);

    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v4)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v5)),
                    CDC_STATUS_OK);

    CU_ASSERT(circular_array_range_int_eq(d, 6, v2, v3, v4, v5, v4, v5));
    cdc_circular_array_erase(d, 1);
    CU_ASSERT(circular_array_range_int_eq(d, 5, v2, v4, v5, v4, v5));
    cdc_circular_array_dtor(d);
  }
  {
    CU_ASSERT_EQUAL(cdc_circular_array_ctorl(
                        &d, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v1),
                        CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                        CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
                    CDC_STATUS_OK);

    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);

    CU_ASSERT(circular_array_range_int_eq(d, 5, v2, v3, v4, v5, v6));
    cdc_circular_array_erase(d, cdc_circular_array_size(d) - 2);
    CU_ASSERT(circular_array_range_int_eq(d, 4, v2, v3, v4, v6));
    cdc_circular_array_dtor(d);
  }

  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(d);

    CU_ASSERT(circular_array_range_int_eq(d, 5, v2, v3, v4, v5, v6));
    cdc_circular_array_erase(d, cdc_circular_array_size(d) - 2);
    CU_ASSERT(circular_array_range_int_eq(d, 4, v2, v3, v4, v6));
    cdc_circular_array_dtor(d);
  }
  {
    CU_ASSERT_EQUAL(
        cdc_circular_array_ctorl(&d, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                                 CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                                 CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
        CDC_STATUS_OK);

    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v1)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v2)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v3)),
                    CDC_STATUS_OK);

    CU_ASSERT(circular_array_range_int_eq(d, 6, v4, v5, v6, v1, v2, v3));
    cdc_circular_array_erase(d, cdc_circular_array_size(d) - 2);
    CU_ASSERT(circular_array_range_int_eq(d, 5, v4, v5, v6, v1, v3));
    cdc_circular_array_dtor(d);
  }
  {
    CU_ASSERT_EQUAL(cdc_circular_array_ctorl(
                        &d, NULL, CDC_FROM_INT(v1), CDC_FROM_INT(v1),
                        CDC_FROM_INT(v1), CDC_FROM_INT(v2), CDC_FROM_INT(v3),
                        CDC_FROM_INT(v4), CDC_FROM_INT(v5), CDC_END),
                    CDC_STATUS_OK);

    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);
    cdc_circular_array_pop_front(d);

    CU_ASSERT_EQUAL(cdc_circular_array_push_front(d, CDC_FROM_INT(v1)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v6)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(d, CDC_FROM_INT(v5)),
                    CDC_STATUS_OK);

    CU_ASSERT(circular_array_range_int_eq(d, 6, v1, v3, v4, v5, v6, v5));
    cdc_circular_array_erase(d, cdc_circular_array_size(d) - 3);
    CU_ASSERT(circular_array_range_int_eq(d, 5, v1, v3, v4, v6, v5));
    cdc_circular_array_dtor(d);
  }
  {
    struct cdc_circular_array *arr = NULL;
    int a = 0, b = 1, c = 2, d = 3, erase_count = 100;

    CU_ASSERT_EQUAL(cdc_circular_array_ctorl(
                        &arr, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                        CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_FROM_INT(a),
                        CDC_FROM_INT(b), CDC_FROM_INT(c), CDC_END),
                    CDC_STATUS_OK);

    cdc_circular_array_erase(arr, 2);
    CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 6);
    CU_ASSERT(circular_array_range_int_eq(arr, 3, a, b, d, a, b, c));

    cdc_circular_array_erase(arr, 4);
    CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 5);
    CU_ASSERT(circular_array_range_int_eq(arr, 5, a, b, d, a, c));

    cdc_circular_array_erase(arr, 0);
    CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 4);
    CU_ASSERT(circular_array_range_int_eq(arr, 4, b, d, a, c));

    cdc_circular_array_erase(arr, cdc_circular_array_size(arr) - 1);
    CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);
    CU_ASSERT(circular_array_range_int_eq(arr, 3, b, d, a));

    cdc_circular_array_clear(arr);

    CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(a)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(b)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(c)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(d)),
                    CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 4, d, c, b, a));
    cdc_circular_array_erase(arr, 1);
    CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);
    CU_ASSERT(circular_array_range_int_eq(arr, 3, d, b, a));

    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(d)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(c)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(a)),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(d)),
                    CDC_STATUS_OK);
    CU_ASSERT(circular_array_range_int_eq(arr, 7, d, d, b, a, d, c, a));
    cdc_circular_array_erase(arr, cdc_circular_array_size(arr) - 2);
    CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 6);
    CU_ASSERT(circular_array_range_int_eq(arr, 6, d, d, b, a, d, a));

    cdc_circular_array_clear(arr);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(0)),
                    CDC_STATUS_OK);
    for (int i = 0; i <= erase_count; ++i) {
      CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 1, CDC_FROM_INT(i)),
                      CDC_STATUS_OK);
    }

    for (int i = erase_count; i >= 0; --i) {
      cdc_circular_array_erase(arr, 1);
    }

    CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 1);
    CU_ASSERT_EQUAL(cdc_circular_array_get(arr, 0), CDC_FROM_INT(0));
    cdc_circular_array_dtor(arr);
  }
}

void test_circular_array_clear()
{
  struct cdc_circular_array *arr = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                               CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
      CDC_STATUS_OK);
  cdc_circular_array_clear(arr);
  CU_ASSERT(cdc_circular_array_empty(arr));
  cdc_circular_array_dtor(arr);
}

void test_circular_array_pop_push()
{
  struct cdc_circular_array *arr;
  int a = 1, b = 2, c = 3;
  int i1 = 3, i2 = 4, i3 = 5;

  CU_ASSERT_EQUAL(
      cdc_circular_array_ctorl(&arr, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                               CDC_FROM_INT(c), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);

  for (int i = 0; i < 5; ++i) {
    CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(a)),
                    CDC_STATUS_OK);
    cdc_circular_array_pop_back(arr);
    CU_ASSERT_EQUAL(cdc_circular_array_push_front(arr, CDC_FROM_INT(b)),
                    CDC_STATUS_OK);
    cdc_circular_array_pop_back(arr);
  }

  CU_ASSERT(circular_array_range_int_eq(arr, 3, b, a, b));
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);

  for (int i = 0; i < 5; ++i) {
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(a)),
                    CDC_STATUS_OK);
    cdc_circular_array_pop_front(arr);
    CU_ASSERT_EQUAL(cdc_circular_array_push_back(arr, CDC_FROM_INT(b)),
                    CDC_STATUS_OK);
    cdc_circular_array_pop_front(arr);
  }
  CU_ASSERT(circular_array_range_int_eq(arr, 3, b, a, b));
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 3);

  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 0, CDC_FROM_INT(i1)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 4);
  CU_ASSERT(circular_array_range_int_eq(arr, 4, i1, b, a, b));

  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, cdc_circular_array_size(arr),
                                            CDC_FROM_INT(i2)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 5);
  CU_ASSERT(circular_array_range_int_eq(arr, 5, i1, b, a, b, i2));

  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 2, CDC_FROM_INT(i3)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 6);
  CU_ASSERT(circular_array_range_int_eq(arr, 6, i1, b, i3, a, b, i2));

  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 4, CDC_FROM_INT(i1)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 7);
  CU_ASSERT(circular_array_range_int_eq(arr, 7, i1, b, i3, a, i1, b, i2));

  CU_ASSERT_EQUAL(cdc_circular_array_insert(arr, 1, CDC_FROM_INT(i2)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_circular_array_size(arr), 8);
  CU_ASSERT(circular_array_range_int_eq(arr, 8, i1, i2, b, i3, a, i1, b, i2));
  cdc_circular_array_dtor(arr);
}
