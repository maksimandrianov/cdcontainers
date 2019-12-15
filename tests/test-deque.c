// The MIT License (MIT)
// Copyright (c) 2019 Maksim Andrianov
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

#include "cdcontainers/adapters/deque.h"
#include "cdcontainers/casts.h"

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
    if (CDC_TO_INT(cdc_deque_get(d, i)) != elem) {
      return false;
    }
  }

  return true;
}

static inline void deque_range_int_print(struct cdc_deque *d)
{
  for (size_t i = 0; i < cdc_deque_size(d); ++i) {
    printf("%d ", CDC_TO_INT(cdc_deque_get(d, i)));
  }
  printf("\n");
}

void test_deque_ctor()
{
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *d = NULL;
    CU_ASSERT_EQUAL(cdc_deque_ctor(tables[t], &d, NULL), CDC_STATUS_OK);
    CU_ASSERT(cdc_deque_empty(d));
    cdc_deque_dtor(d);
  }
}

void test_deque_ctorl()
{
  int a = 0, b = 1, c = 2, d = 3;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *deq = NULL;
    CU_ASSERT_EQUAL(
        cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                        CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
        CDC_STATUS_OK);

    CU_ASSERT_EQUAL(cdc_deque_size(deq), 4);
    CU_ASSERT(deque_range_int_eq(deq, 4, a, b, c, d));
    cdc_deque_dtor(deq);
  }
}

void test_deque_push_back()
{
  int a = 0, b = 1, c = 2;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *deq = NULL;
    CU_ASSERT_EQUAL(cdc_deque_ctor(tables[t], &deq, NULL), CDC_STATUS_OK);

    cdc_deque_push_back(deq, CDC_FROM_INT(a));
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 1);
    CU_ASSERT(deque_range_int_eq(deq, 1, a));

    cdc_deque_push_back(deq, CDC_FROM_INT(b));
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 2);
    CU_ASSERT(deque_range_int_eq(deq, 1, a, b));

    cdc_deque_push_back(deq, CDC_FROM_INT(c));
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
    CU_ASSERT(deque_range_int_eq(deq, 3, a, b, c));

    cdc_deque_push_back(deq, CDC_FROM_INT(a));
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 4);
    CU_ASSERT(deque_range_int_eq(deq, 4, a, b, c, a));

    cdc_deque_push_back(deq, CDC_FROM_INT(b));
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 5);
    CU_ASSERT(deque_range_int_eq(deq, 5, a, b, c, a, b));
    cdc_deque_dtor(deq);
  }
}

void test_deque_push_front()
{
  int a = 0, b = 1, c = 2;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *deq = NULL;
    CU_ASSERT_EQUAL(cdc_deque_ctor(tables[t], &deq, NULL), CDC_STATUS_OK);
    CU_ASSERT(cdc_deque_empty(deq));

    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(a)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 1);
    CU_ASSERT(deque_range_int_eq(deq, 1, a));

    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(b)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 2);
    CU_ASSERT(deque_range_int_eq(deq, 2, b, a));

    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(c)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
    CU_ASSERT(deque_range_int_eq(deq, 3, c, b, a));

    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(a)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 4);
    CU_ASSERT(deque_range_int_eq(deq, 4, a, c, b, a));

    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(b)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 5);
    CU_ASSERT(deque_range_int_eq(deq, 4, b, a, c, b, a));
    cdc_deque_dtor(deq);
  }
}

void test_deque_get()
{
  int a = 0, b = 1;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *deq = NULL;
    CU_ASSERT_EQUAL(cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(a),
                                    CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(cdc_deque_get(deq, 0)), a);
    CU_ASSERT_EQUAL(CDC_TO_INT(cdc_deque_get(deq, 1)), b);
    cdc_deque_dtor(deq);
  }
}

void test_deque_set()
{
  int a = 1, b = 2;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *d = NULL;
    CU_ASSERT_EQUAL(cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(a),
                                    CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);

    cdc_deque_set(d, 0, CDC_FROM_INT(b));
    CU_ASSERT(deque_range_int_eq(d, 2, b, b));

    cdc_deque_set(d, 1, CDC_FROM_INT(a));
    CU_ASSERT(deque_range_int_eq(d, 2, b, a));
    cdc_deque_dtor(d);
  }
}

void test_deque_front()
{
  int a = 1, b = 2;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *d = NULL;
    CU_ASSERT_EQUAL(cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(a),
                                    CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(cdc_deque_front(d)), a);

    CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_FROM_INT(b)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(cdc_deque_front(d)), b);
    cdc_deque_dtor(d);
  }
}

void test_deque_back()
{
  int a = 1, b = 2;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *d = NULL;
    CU_ASSERT_EQUAL(cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(a),
                                    CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(cdc_deque_back(d)), b);

    CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(a)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(cdc_deque_back(d)), a);
    cdc_deque_dtor(d);
  }
}

void test_deque_pop_back()
{
  int a = 0, b = 1, c = 2, d = 3;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *deq = NULL;
    CU_ASSERT_EQUAL(
        cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                        CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
        CDC_STATUS_OK);

    void *elem = cdc_deque_back(deq);
    cdc_deque_pop_back(deq);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);

    elem = cdc_deque_back(deq);
    cdc_deque_pop_back(deq);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 2);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

    elem = cdc_deque_back(deq);
    cdc_deque_pop_back(deq);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 1);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

    elem = cdc_deque_back(deq);
    cdc_deque_pop_back(deq);
    CU_ASSERT(cdc_deque_empty(deq));
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
    cdc_deque_dtor(deq);
  }
}

void test_deque_pop_front()
{
  int a = 0, b = 1, c = 2, d = 3;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *deq = NULL;
    CU_ASSERT_EQUAL(
        cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                        CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
        CDC_STATUS_OK);

    void *elem = cdc_deque_front(deq);
    cdc_deque_pop_front(deq);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

    elem = cdc_deque_front(deq);
    cdc_deque_pop_front(deq);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 2);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

    elem = cdc_deque_front(deq);
    cdc_deque_pop_front(deq);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 1);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

    elem = cdc_deque_front(deq);
    cdc_deque_pop_front(deq);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 0);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);
    cdc_deque_dtor(deq);
  }
}

void test_deque_swap()
{
  int a = 0, b = 1, c = 2, d = 3;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *v = NULL;
    struct cdc_deque *w = NULL;
    CU_ASSERT_EQUAL(
        cdc_deque_ctorl(tables[t], &v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                        CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
        CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_ctorl(tables[t], &w, NULL, CDC_FROM_INT(a),
                                    CDC_FROM_INT(d), CDC_END),
                    CDC_STATUS_OK);

    cdc_deque_swap(v, w);
    CU_ASSERT_EQUAL(cdc_deque_size(v), 2);
    CU_ASSERT(deque_range_int_eq(v, 2, a, d));
    CU_ASSERT_EQUAL(cdc_deque_size(w), 4);
    CU_ASSERT(deque_range_int_eq(w, 4, a, b, c, d));
    cdc_deque_dtor(v);
    cdc_deque_dtor(w);
  }
}

void test_deque_insert()
{
  struct cdc_deque *deq = NULL;
  int a = 1, b = 2;
  int i1 = 3, i2 = 4, i3 = 5;
  int v0 = -1, v1 = 0, v2 = 1, v3 = 2, v4 = 3, v5 = 4, v6 = 5;

  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      CU_ASSERT_EQUAL(cdc_deque_insert(deq, 2, CDC_FROM_INT(v0)),
                      CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 7, v1, v2, v0, v3, v4, v5, v6));
      cdc_deque_dtor(deq);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(deq);
      CU_ASSERT(deque_range_int_eq(deq, 5, v2, v3, v4, v5, v6));
      CU_ASSERT_EQUAL(cdc_deque_insert(deq, 1, CDC_FROM_INT(v0)),
                      CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 6, v2, v0, v3, v4, v5, v6));
      cdc_deque_dtor(deq);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);

      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v1)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v2)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v3)),
                      CDC_STATUS_OK);

      CU_ASSERT(deque_range_int_eq(deq, 6, v4, v5, v6, v1, v2, v3));
      CU_ASSERT_EQUAL(cdc_deque_insert(deq, 1, CDC_FROM_INT(v0)),
                      CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 7, v4, v0, v5, v6, v1, v2, v3));
      cdc_deque_dtor(deq);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);

      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v1)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v2)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v3)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v4)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v5)),
                      CDC_STATUS_OK);

      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);

      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v4)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v5)),
                      CDC_STATUS_OK);

      CU_ASSERT(deque_range_int_eq(deq, 6, v2, v3, v4, v5, v4, v5));
      CU_ASSERT_EQUAL(cdc_deque_insert(deq, 1, CDC_FROM_INT(v0)),
                      CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 7, v2, v0, v3, v4, v5, v4, v5));
      cdc_deque_dtor(deq);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v1), CDC_FROM_INT(v2), CDC_FROM_INT(v3),
                          CDC_FROM_INT(v4), CDC_FROM_INT(v5), CDC_FROM_INT(v6),
                          CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);

      CU_ASSERT(deque_range_int_eq(deq, 5, v2, v3, v4, v5, v6));
      CU_ASSERT_EQUAL(
          cdc_deque_insert(deq, cdc_deque_size(deq) - 2, CDC_FROM_INT(v0)),
          CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 6, v2, v3, v4, v0, v5, v6));
      cdc_deque_dtor(deq);
    }
  }

  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(deq);

      CU_ASSERT(deque_range_int_eq(deq, 5, v2, v3, v4, v5, v6));
      CU_ASSERT_EQUAL(
          cdc_deque_insert(deq, cdc_deque_size(deq) - 2, CDC_FROM_INT(v0)),
          CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 6, v2, v3, v4, v0, v5, v6));
      cdc_deque_dtor(deq);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);

      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v1)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v2)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v3)),
                      CDC_STATUS_OK);

      CU_ASSERT(deque_range_int_eq(deq, 6, v4, v5, v6, v1, v2, v3));
      CU_ASSERT_EQUAL(
          cdc_deque_insert(deq, cdc_deque_size(deq) - 1, CDC_FROM_INT(v0)),
          CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 7, v4, v5, v6, v1, v2, v0, v3));
      cdc_deque_dtor(deq);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v1), CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                          CDC_FROM_INT(v3), CDC_FROM_INT(v4), CDC_FROM_INT(v5),
                          CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);
      cdc_deque_pop_front(deq);

      CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(v1)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v6)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(v5)),
                      CDC_STATUS_OK);

      CU_ASSERT(deque_range_int_eq(deq, 6, v1, v3, v4, v5, v6, v5));
      CU_ASSERT_EQUAL(
          cdc_deque_insert(deq, cdc_deque_size(deq) - 3, CDC_FROM_INT(v0)),
          CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 7, v1, v3, v4, v0, v5, v6, v5));
      cdc_deque_dtor(deq);
    }
  }

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    int insert_count = 100;
    CU_ASSERT_EQUAL(cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(a),
                                    CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_insert(deq, 0, CDC_FROM_INT(i1)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
    CU_ASSERT(deque_range_int_eq(deq, 3, i1, a, b));

    CU_ASSERT_EQUAL(
        cdc_deque_insert(deq, cdc_deque_size(deq), CDC_FROM_INT(i2)),
        CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 4);
    CU_ASSERT(deque_range_int_eq(deq, 4, i1, a, b, i2));

    CU_ASSERT_EQUAL(cdc_deque_insert(deq, 2, CDC_FROM_INT(i3)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 5);
    CU_ASSERT(deque_range_int_eq(deq, 5, i1, a, i3, b, i2));

    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(a)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(b)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(i1)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(i2)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(i3)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(a)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(b)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(i1)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(i3)), CDC_STATUS_OK);
    CU_ASSERT(deque_range_int_eq(deq, 14, i3, i1, b, a, i3, i2, i1, b, a, i1, a,
                                 i3, b, i2));

    CU_ASSERT_EQUAL(cdc_deque_insert(deq, 1, CDC_FROM_INT(a)), CDC_STATUS_OK);
    CU_ASSERT(deque_range_int_eq(deq, 15, i3, a, i1, b, a, i3, i2, i1, b, a, i1,
                                 a, i3, b, i2));

    CU_ASSERT_EQUAL(
        cdc_deque_insert(deq, cdc_deque_size(deq) - 2, CDC_FROM_INT(a)),
        CDC_STATUS_OK);
    CU_ASSERT(deque_range_int_eq(deq, 16, i3, a, i1, b, a, i3, i2, i1, b, a, i1,
                                 a, i3, a, b, i2));

    cdc_deque_clear(deq);
    CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(0)), CDC_STATUS_OK);
    for (int i = 0; i <= insert_count; ++i) {
      CU_ASSERT_EQUAL(cdc_deque_insert(deq, 1, CDC_FROM_INT(i)), CDC_STATUS_OK);
    }

    CU_ASSERT_EQUAL(cdc_deque_get(deq, 0), CDC_FROM_INT(0));
    for (int i = insert_count; i >= 0; --i) {
      CU_ASSERT_EQUAL(cdc_deque_get(deq, (size_t)(insert_count - i + 1)),
                      CDC_FROM_INT(i));
    }

    cdc_deque_dtor(deq);
  }
}

void test_deque_erase()
{
  struct cdc_deque *d = NULL;
  int v1 = 0, v2 = 1, v3 = 2, v4 = 3, v5 = 4, v6 = 5;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_erase(d, 2);
      CU_ASSERT(deque_range_int_eq(d, 5, v1, v2, v4, v5, v6));
      cdc_deque_dtor(d);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(d);
      CU_ASSERT(deque_range_int_eq(d, 5, v2, v3, v4, v5, v6));
      cdc_deque_erase(d, 1);
      CU_ASSERT(deque_range_int_eq(d, 4, v2, v4, v5, v6));
      cdc_deque_dtor(d);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);

      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v1)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v2)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v3)), CDC_STATUS_OK);

      CU_ASSERT(deque_range_int_eq(d, 6, v4, v5, v6, v1, v2, v3));
      cdc_deque_erase(d, 1);
      CU_ASSERT(deque_range_int_eq(d, 5, v4, v6, v1, v2, v3));
      cdc_deque_dtor(d);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);

      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v1)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v2)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v3)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v4)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v5)), CDC_STATUS_OK);

      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);

      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v4)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v5)), CDC_STATUS_OK);

      CU_ASSERT(deque_range_int_eq(d, 6, v2, v3, v4, v5, v4, v5));
      cdc_deque_erase(d, 1);
      CU_ASSERT(deque_range_int_eq(d, 5, v2, v4, v5, v4, v5));
      cdc_deque_dtor(d);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v1), CDC_FROM_INT(v2), CDC_FROM_INT(v3),
                          CDC_FROM_INT(v4), CDC_FROM_INT(v5), CDC_FROM_INT(v6),
                          CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);

      CU_ASSERT(deque_range_int_eq(d, 5, v2, v3, v4, v5, v6));
      cdc_deque_erase(d, cdc_deque_size(d) - 2);
      CU_ASSERT(deque_range_int_eq(d, 4, v2, v3, v4, v6));
      cdc_deque_dtor(d);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(d);

      CU_ASSERT(deque_range_int_eq(d, 5, v2, v3, v4, v5, v6));
      cdc_deque_erase(d, cdc_deque_size(d) - 2);
      CU_ASSERT(deque_range_int_eq(d, 4, v2, v3, v4, v6));
      cdc_deque_dtor(d);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v2), CDC_FROM_INT(v3), CDC_FROM_INT(v4),
                          CDC_FROM_INT(v5), CDC_FROM_INT(v6), CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);

      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v1)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v2)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v3)), CDC_STATUS_OK);

      CU_ASSERT(deque_range_int_eq(d, 6, v4, v5, v6, v1, v2, v3));
      cdc_deque_erase(d, cdc_deque_size(d) - 2);
      CU_ASSERT(deque_range_int_eq(d, 5, v4, v5, v6, v1, v3));
      cdc_deque_dtor(d);
    }
  }
  {
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &d, NULL, CDC_FROM_INT(v1),
                          CDC_FROM_INT(v1), CDC_FROM_INT(v1), CDC_FROM_INT(v2),
                          CDC_FROM_INT(v3), CDC_FROM_INT(v4), CDC_FROM_INT(v5),
                          CDC_END),
          CDC_STATUS_OK);

      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);
      cdc_deque_pop_front(d);

      CU_ASSERT_EQUAL(cdc_deque_push_front(d, CDC_FROM_INT(v1)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v6)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(d, CDC_FROM_INT(v5)), CDC_STATUS_OK);

      CU_ASSERT(deque_range_int_eq(d, 6, v1, v3, v4, v5, v6, v5));
      cdc_deque_erase(d, cdc_deque_size(d) - 3);
      CU_ASSERT(deque_range_int_eq(d, 5, v1, v3, v4, v6, v5));
      cdc_deque_dtor(d);
    }
  }
  {
    int a = 0, b = 1, c = 2, d = 3, erase_count = 100;
    for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
      struct cdc_deque *deq = NULL;

      CU_ASSERT_EQUAL(
          cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(a),
                          CDC_FROM_INT(b), CDC_FROM_INT(c), CDC_FROM_INT(d),
                          CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                          CDC_END),
          CDC_STATUS_OK);

      cdc_deque_erase(deq, 2);
      CU_ASSERT_EQUAL(cdc_deque_size(deq), 6);
      CU_ASSERT(deque_range_int_eq(deq, 3, a, b, d, a, b, c));

      cdc_deque_erase(deq, 4);
      CU_ASSERT_EQUAL(cdc_deque_size(deq), 5);
      CU_ASSERT(deque_range_int_eq(deq, 5, a, b, d, a, c));

      cdc_deque_erase(deq, 0);
      CU_ASSERT_EQUAL(cdc_deque_size(deq), 4);
      CU_ASSERT(deque_range_int_eq(deq, 4, b, d, a, c));

      cdc_deque_erase(deq, cdc_deque_size(deq) - 1);
      CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
      CU_ASSERT(deque_range_int_eq(deq, 3, b, d, a));

      cdc_deque_clear(deq);

      CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(a)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(b)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(c)),
                      CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(d)),
                      CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 4, d, c, b, a));
      cdc_deque_erase(deq, 1);
      CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);
      CU_ASSERT(deque_range_int_eq(deq, 3, d, b, a));

      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(d)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(c)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(a)), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(d)),
                      CDC_STATUS_OK);
      CU_ASSERT(deque_range_int_eq(deq, 7, d, d, b, a, d, c, a));
      cdc_deque_erase(deq, cdc_deque_size(deq) - 2);
      CU_ASSERT_EQUAL(cdc_deque_size(deq), 6);
      CU_ASSERT(deque_range_int_eq(deq, 6, d, d, b, a, d, a));

      cdc_deque_clear(deq);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(0)), CDC_STATUS_OK);
      for (int i = 0; i <= erase_count; ++i) {
        CU_ASSERT_EQUAL(cdc_deque_insert(deq, 1, CDC_FROM_INT(i)),
                        CDC_STATUS_OK);
      }

      for (int i = erase_count; i >= 0; --i) {
        cdc_deque_erase(deq, 1);
      }

      CU_ASSERT_EQUAL(cdc_deque_size(deq), 1);
      CU_ASSERT_EQUAL(cdc_deque_get(deq, 0), CDC_FROM_INT(0));
      cdc_deque_dtor(deq);
    }
  }
}

void test_deque_clear()
{
  int a = 0, b = 1, c = 2, d = 3;
  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *deq = NULL;
    CU_ASSERT_EQUAL(
        cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                        CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
        CDC_STATUS_OK);
    cdc_deque_clear(deq);
    CU_ASSERT(cdc_deque_empty(deq));
    cdc_deque_dtor(deq);
  }
}

void test_deque_pop_push()
{
  int a = 1, b = 2, c = 3;
  int i1 = 3, i2 = 4, i3 = 5;

  const struct cdc_sequence_table *tables[] = {cdc_seq_carray, cdc_seq_array,
                                               cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    struct cdc_deque *deq = NULL;
    CU_ASSERT_EQUAL(cdc_deque_ctorl(tables[t], &deq, NULL, CDC_FROM_INT(a),
                                    CDC_FROM_INT(b), CDC_FROM_INT(c), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);

    for (int i = 0; i < 5; ++i) {
      CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(a)),
                      CDC_STATUS_OK);
      cdc_deque_pop_back(deq);
      CU_ASSERT_EQUAL(cdc_deque_push_front(deq, CDC_FROM_INT(b)),
                      CDC_STATUS_OK);
      cdc_deque_pop_back(deq);
    }

    CU_ASSERT(deque_range_int_eq(deq, 3, b, a, b));
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);

    for (int i = 0; i < 5; ++i) {
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(a)), CDC_STATUS_OK);
      cdc_deque_pop_front(deq);
      CU_ASSERT_EQUAL(cdc_deque_push_back(deq, CDC_FROM_INT(b)), CDC_STATUS_OK);
      cdc_deque_pop_front(deq);
    }
    CU_ASSERT(deque_range_int_eq(deq, 3, b, a, b));
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 3);

    CU_ASSERT_EQUAL(cdc_deque_insert(deq, 0, CDC_FROM_INT(i1)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 4);
    CU_ASSERT(deque_range_int_eq(deq, 4, i1, b, a, b));

    CU_ASSERT_EQUAL(
        cdc_deque_insert(deq, cdc_deque_size(deq), CDC_FROM_INT(i2)),
        CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 5);
    CU_ASSERT(deque_range_int_eq(deq, 5, i1, b, a, b, i2));

    CU_ASSERT_EQUAL(cdc_deque_insert(deq, 2, CDC_FROM_INT(i3)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 6);
    CU_ASSERT(deque_range_int_eq(deq, 6, i1, b, i3, a, b, i2));

    CU_ASSERT_EQUAL(cdc_deque_insert(deq, 4, CDC_FROM_INT(i1)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 7);
    CU_ASSERT(deque_range_int_eq(deq, 7, i1, b, i3, a, i1, b, i2));

    CU_ASSERT_EQUAL(cdc_deque_insert(deq, 1, CDC_FROM_INT(i2)), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(cdc_deque_size(deq), 8);
    CU_ASSERT(deque_range_int_eq(deq, 8, i1, i2, b, i3, a, i1, b, i2));
    cdc_deque_dtor(deq);
  }
}
