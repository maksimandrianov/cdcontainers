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

#include "cdcontainers/array.h"
#include "cdcontainers/casts.h"
#include "cdcontainers/common.h"

#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

static bool array_range_int_eq(struct cdc_array *v, size_t count, ...)
{
  va_list args;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    int elem = va_arg(args, int);
    if (elem != CDC_TO_INT(cdc_array_get(v, i))) {
      return false;
    }
  }

  return true;
}

void test_array_ctor()
{
  struct cdc_array *v = NULL;

  CU_ASSERT_EQUAL(cdc_array_ctor(&v, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_array_empty(v));
  cdc_array_dtor(v);
}

void test_array_ctorl()
{
  struct cdc_array *v = NULL;
  int a = 0, b = 1, c = 2, d = 3;
  size_t count_elements = 4;

  CU_ASSERT_EQUAL(cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                  CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_array_size(v), count_elements);
  CU_ASSERT(array_range_int_eq(v, count_elements, a, b, c, d));
  cdc_array_dtor(v);
}

void test_array_reserve()
{
  struct cdc_array *v = NULL;
  size_t count_reserved = 10;

  CU_ASSERT_EQUAL(cdc_array_ctor(&v, NULL), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_array_reserve(v, count_reserved), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_array_capacity(v), count_reserved);

  CU_ASSERT_EQUAL(cdc_array_reserve(v, count_reserved - 1), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_array_capacity(v), count_reserved);
  cdc_array_dtor(v);
}

void test_array_data()
{
  struct cdc_array *v = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                  CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT((cdc_array_data(v)[0])), a);
  CU_ASSERT_EQUAL(CDC_TO_INT((cdc_array_data(v)[1])), b);
  CU_ASSERT_EQUAL(CDC_TO_INT((cdc_array_data(v)[2])), c);
  CU_ASSERT_EQUAL(CDC_TO_INT((cdc_array_data(v)[3])), d);
  cdc_array_dtor(v);
}

void test_array_get()
{
  struct cdc_array *v = NULL;
  int a = 0, b = 1;

  CU_ASSERT_EQUAL(
      cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_array_get(v, 0)), a);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_array_get(v, 1)), b);
  cdc_array_dtor(v);
}

void test_array_at()
{
  struct cdc_array *v = NULL;
  int a = 0, b = 1, c = 2;
  void *elem = NULL;

  CU_ASSERT_EQUAL(cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                  CDC_FROM_INT(c), CDC_END),
                  CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_array_at(v, 0, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  CU_ASSERT_EQUAL(cdc_array_at(v, 1, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);
  CU_ASSERT_EQUAL(cdc_array_at(v, 2, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  size_t index = cdc_array_size(v) + 1;
  CU_ASSERT_EQUAL(cdc_array_at(v, index, &elem), CDC_STATUS_OUT_OF_RANGE);
  cdc_array_dtor(v);
}

void test_array_set()
{
  struct cdc_array *v = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(
      cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);

  cdc_array_set(v, 0, CDC_FROM_INT(b));
  CU_ASSERT(array_range_int_eq(v, 2, b, b));

  cdc_array_set(v, 1, CDC_FROM_INT(a));
  CU_ASSERT(array_range_int_eq(v, 2, b, a));
  cdc_array_dtor(v);
}

void test_array_front()
{
  struct cdc_array *v = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(
      cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_array_front(v)), a);
  cdc_array_dtor(v);
}

void test_array_back()
{
  struct cdc_array *v = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(
      cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_array_back(v)), b);
  cdc_array_dtor(v);
}

void test_array_insert()
{
  struct cdc_array *v = NULL;
  int a = 1, b = 2;
  int i1 = 3, i2 = 4, i3 = 5;

  CU_ASSERT_EQUAL(
      cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_array_insert(v, 0, CDC_FROM_INT(i1)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_array_size(v), 3);
  CU_ASSERT(array_range_int_eq(v, 3, i1, a, b));

  CU_ASSERT_EQUAL(cdc_array_insert(v, cdc_array_size(v), CDC_FROM_INT(i2)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_array_size(v), 4);
  CU_ASSERT(array_range_int_eq(v, 4, i1, a, b, i2));

  CU_ASSERT_EQUAL(cdc_array_insert(v, 2, CDC_FROM_INT(i3)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_array_size(v), 5);
  CU_ASSERT(array_range_int_eq(v, 5, i1, a, i3, b, i2));
  cdc_array_dtor(v);
}

void test_array_erase()
{
  struct cdc_array *v = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                  CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_array_erase(v, 2);
  CU_ASSERT_EQUAL(cdc_array_size(v), 3);
  CU_ASSERT(array_range_int_eq(v, 3, a, b, d));

  cdc_array_erase(v, 0);
  CU_ASSERT_EQUAL(cdc_array_size(v), 2);
  CU_ASSERT(array_range_int_eq(v, 2, b, d));

  cdc_array_erase(v, cdc_array_size(v) - 1);
  CU_ASSERT_EQUAL(cdc_array_size(v), 1);
  CU_ASSERT(array_range_int_eq(v, 1, b));
  cdc_array_dtor(v);
}

void test_array_clear()
{
  struct cdc_array *v = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                  CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  cdc_array_clear(v);
  CU_ASSERT(cdc_array_empty(v));
  cdc_array_dtor(v);
}

void test_array_push_back()
{
  struct cdc_array *v = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_array_ctor(&v, NULL), CDC_STATUS_OK);

  cdc_array_push_back(v, CDC_FROM_INT(a));
  CU_ASSERT_EQUAL(cdc_array_size(v), 1);
  CU_ASSERT(array_range_int_eq(v, 1, a));

  cdc_array_push_back(v, CDC_FROM_INT(b));
  CU_ASSERT_EQUAL(cdc_array_size(v), 2);
  CU_ASSERT(array_range_int_eq(v, 1, a, b));

  cdc_array_push_back(v, CDC_FROM_INT(c));
  CU_ASSERT_EQUAL(cdc_array_size(v), 3);
  CU_ASSERT(array_range_int_eq(v, 3, a, b, c));

  cdc_array_dtor(v);
}

void test_array_pop_back()
{
  struct cdc_array *v = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                  CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  void *elem = cdc_array_back(v);
  cdc_array_pop_back(v);
  CU_ASSERT_EQUAL(cdc_array_size(v), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);

  elem = cdc_array_back(v);
  cdc_array_pop_back(v);
  CU_ASSERT_EQUAL(cdc_array_size(v), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = cdc_array_back(v);
  cdc_array_pop_back(v);
  CU_ASSERT_EQUAL(cdc_array_size(v), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = cdc_array_back(v);
  cdc_array_pop_back(v);
  CU_ASSERT(cdc_array_empty(v));
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  cdc_array_dtor(v);
}

void test_array_swap()
{
  struct cdc_array *v = NULL;
  struct cdc_array *w = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_array_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                  CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(
      cdc_array_ctorl(&w, NULL, CDC_FROM_INT(a), CDC_FROM_INT(d), CDC_END),
      CDC_STATUS_OK);
  cdc_array_swap(v, w);

  CU_ASSERT_EQUAL(cdc_array_size(v), 2);
  CU_ASSERT(array_range_int_eq(v, 2, a, d));
  CU_ASSERT_EQUAL(cdc_array_size(w), 4);
  CU_ASSERT(array_range_int_eq(w, 4, a, b, c, d));
  cdc_array_dtor(v);
  cdc_array_dtor(w);
}

void test_array_shrink_to_fit()
{
  struct cdc_array *v = NULL;
  const int count = 100;

  CU_ASSERT_EQUAL(cdc_array_ctor(&v, NULL), CDC_STATUS_OK);
  for (int i = 0; i < count; ++i) {
    CU_ASSERT_EQUAL(cdc_array_push_back(v, CDC_FROM_INT(i)), CDC_STATUS_OK);
  }
  CU_ASSERT(cdc_array_capacity(v) > count);
  cdc_array_shrink_to_fit(v);
  CU_ASSERT_EQUAL(cdc_array_capacity(v), count);
}
