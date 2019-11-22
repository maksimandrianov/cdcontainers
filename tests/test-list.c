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
#include "cdcontainers/common.h"
#include "cdcontainers/list.h"

#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

static bool list_range_int_eq(struct cdc_list *l, size_t count, ...)
{
  va_list args;
  enum cdc_stat ret;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    void *val = NULL;
    if ((ret = cdc_list_at(l, i, &val)) != CDC_STATUS_OK) {
      return false;
    }
    int elem = va_arg(args, int);
    if (elem != CDC_TO_INT(val)) {
      return false;
    }
  }

  return cdc_list_size(l) == count;
}

static inline void list_range_int_print(struct cdc_list *l)
{
  printf("\n");
  for (size_t i = 0; i < cdc_list_size(l); ++i) {
    void *val = NULL;
    if (cdc_list_at(l, i, &val) == CDC_STATUS_OK) {
      printf("%d ", CDC_TO_INT(val));
    }
  }
  printf("\n");
}

static int lt(const void *l, const void *r)
{
  return CDC_TO_INT(l) < CDC_TO_INT(r);
}

static int eq(const void *l, const void *r)
{
  return CDC_TO_INT(l) == CDC_TO_INT(r);
}

static int is_eq_2(const void *v) { return CDC_TO_INT(v) == 2; }

void test_list_ctor()
{
  struct cdc_list *l = NULL;

  CU_ASSERT_EQUAL(cdc_list_ctor(&l, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_list_empty(l));
  cdc_list_dtor(l);
}

void test_list_ctorl()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 4);
  CU_ASSERT(list_range_int_eq(l, 4, a, b, c, d));
  cdc_list_dtor(l);
}

void test_list_push_back()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_list_ctor(&l, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_list_empty(l));

  CU_ASSERT_EQUAL(cdc_list_push_back(l, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, a));

  CU_ASSERT_EQUAL(cdc_list_push_back(l, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, a, b));

  CU_ASSERT_EQUAL(cdc_list_push_back(l, CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, a, b, c));
  cdc_list_dtor(l);
}

void test_list_push_front()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_list_ctor(&l, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_list_empty(l));

  CU_ASSERT_EQUAL(cdc_list_push_front(l, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, a));

  CU_ASSERT_EQUAL(cdc_list_push_front(l, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, b, a));

  CU_ASSERT_EQUAL(cdc_list_push_front(l, CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, c, b, a));
  cdc_list_dtor(l);
}

void test_list_at()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2;
  void *elem = NULL;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_END),
                  CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_list_at(l, 0, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  CU_ASSERT_EQUAL(cdc_list_at(l, 1, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);
  CU_ASSERT_EQUAL(cdc_list_at(l, 2, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  size_t index = cdc_list_size(l) + 1;
  CU_ASSERT_EQUAL(cdc_list_at(l, index, &elem), CDC_STATUS_OUT_OF_RANGE);
  cdc_list_dtor(l);
}

void test_list_front()
{
  struct cdc_list *l = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(
      cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_front(l)), a);
  cdc_list_dtor(l);
}

void test_list_back()
{
  struct cdc_list *l = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(
      cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_back(l)), b);
  cdc_list_dtor(l);
}

void test_list_pop_back()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  void *elem = cdc_list_back(l);
  CU_ASSERT_EQUAL(cdc_list_pop_back(l), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);

  elem = cdc_list_back(l);
  CU_ASSERT_EQUAL(cdc_list_pop_back(l), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = cdc_list_back(l);
  CU_ASSERT_EQUAL(cdc_list_pop_back(l), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = cdc_list_back(l);
  CU_ASSERT_EQUAL(cdc_list_pop_back(l), CDC_STATUS_OK);
  CU_ASSERT(cdc_list_empty(l));
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  cdc_list_dtor(l);
}

void test_list_pop_front()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  void *elem = cdc_list_front(l);
  CU_ASSERT_EQUAL(cdc_list_pop_front(l), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

  elem = cdc_list_front(l);
  CU_ASSERT_EQUAL(cdc_list_pop_front(l), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = cdc_list_front(l);
  CU_ASSERT_EQUAL(cdc_list_pop_front(l), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = cdc_list_front(l);
  CU_ASSERT_EQUAL(cdc_list_pop_front(l), CDC_STATUS_OK);
  CU_ASSERT(cdc_list_empty(l));
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);
  cdc_list_dtor(l);
}

void test_list_swap()
{
  struct cdc_list *v = NULL;
  struct cdc_list *w = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(
      cdc_list_ctorl(&w, NULL, CDC_FROM_INT(a), CDC_FROM_INT(d), CDC_END),
      CDC_STATUS_OK);

  cdc_list_swap(v, w);

  CU_ASSERT_EQUAL(cdc_list_size(v), 2);
  CU_ASSERT(list_range_int_eq(v, 2, a, d));
  CU_ASSERT_EQUAL(cdc_list_size(w), 4);
  CU_ASSERT(list_range_int_eq(w, 4, a, b, c, d));

  cdc_list_dtor(v);
  cdc_list_dtor(w);
}

void test_list_insert()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_list_ctor(&l, NULL), CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_list_insert(l, 0, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, a));

  CU_ASSERT_EQUAL(cdc_list_insert(l, cdc_list_size(l), CDC_FROM_INT(c)),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, a, c));

  CU_ASSERT_EQUAL(cdc_list_insert(l, 1, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, a, b, c));
  cdc_list_dtor(l);
}

void test_list_remove()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;
  void *elem = NULL;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_list_remove(l, 0, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, b, c, d));

  CU_ASSERT_EQUAL(cdc_list_remove(l, 1, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, b, d));

  CU_ASSERT_EQUAL(cdc_list_remove(l, cdc_list_size(l) - 1, &elem),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, b));
  cdc_list_dtor(l);
}

void test_list_iremove()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;
  struct cdc_list_iter it = CDC_INIT_STRUCT;
  void *elem = NULL;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_begin(l, &it);
  CU_ASSERT_EQUAL(cdc_list_iremove(&it, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, b, c, d));

  cdc_list_begin(l, &it);
  cdc_list_iter_next(&it);
  CU_ASSERT_EQUAL(cdc_list_iremove(&it, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, b, d));

  cdc_list_end(l, &it);
  cdc_list_iter_prev(&it);
  CU_ASSERT_EQUAL(cdc_list_iremove(&it, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, b));
  cdc_list_dtor(l);
}

void test_list_erase()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_list_erase(l, 0), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, b, c, d));

  CU_ASSERT_EQUAL(cdc_list_erase(l, 1), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, b, d));

  CU_ASSERT_EQUAL(cdc_list_erase(l, cdc_list_size(l) - 1), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, b));
  cdc_list_dtor(l);
}

void test_list_ierase()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;
  struct cdc_list_iter it = CDC_INIT_STRUCT;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_begin(l, &it);
  CU_ASSERT_EQUAL(cdc_list_ierase(&it), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, b, c, d));

  cdc_list_begin(l, &it);
  cdc_list_iter_next(&it);
  CU_ASSERT_EQUAL(cdc_list_ierase(&it), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, b, d));

  cdc_list_end(l, &it);
  cdc_list_iter_prev(&it);
  CU_ASSERT_EQUAL(cdc_list_ierase(&it), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, b));
  cdc_list_dtor(l);
}

void test_list_clear()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  cdc_list_clear(l);
  CU_ASSERT(cdc_list_empty(l));
  cdc_list_dtor(l);
}

void test_list_iterators()
{
  struct cdc_list *l = NULL;
  struct cdc_list_iter it = CDC_INIT_STRUCT;
  struct cdc_list_iter ittmp = CDC_INIT_STRUCT;
  struct cdc_list_riter rit = CDC_INIT_STRUCT;
  struct cdc_list_riter rittmp = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_begin(l, &it);
  CU_ASSERT_EQUAL(cdc_list_iter_has_next(&it), true);
  CU_ASSERT_EQUAL(cdc_list_iter_has_prev(&it), false);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_iter_data(&it)), a);
  cdc_list_riter_from(&it, &rit);
  cdc_list_rend(l, &rittmp);
  CU_ASSERT(cdc_list_riter_is_eq(&rittmp, &rit));

  cdc_list_iter_next(&it);
  CU_ASSERT(cdc_list_iter_has_next(&it));
  CU_ASSERT(cdc_list_iter_has_prev(&it));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_iter_data(&it)), b);
  cdc_list_riter_from(&it, &rit);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_riter_data(&rit)), a);

  cdc_list_iter_next(&it);
  CU_ASSERT(cdc_list_iter_has_next(&it));
  CU_ASSERT(cdc_list_iter_has_prev(&it));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_iter_data(&it)), c);
  cdc_list_riter_from(&it, &rit);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_riter_data(&rit)), b);

  cdc_list_iter_next(&it);
  CU_ASSERT(!cdc_list_iter_has_next(&it));
  CU_ASSERT(cdc_list_iter_has_prev(&it));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_iter_data(&it)), d);
  cdc_list_riter_from(&it, &rit);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_riter_data(&rit)), c);

  cdc_list_iter_next(&it);
  cdc_list_end(l, &ittmp);
  CU_ASSERT(cdc_list_iter_is_eq(&ittmp, &it));
  cdc_list_riter_from(&it, &rit);
  cdc_list_rbegin(l, &rittmp);
  CU_ASSERT(cdc_list_riter_is_eq(&rittmp, &rit));

  cdc_list_iter_prev(&it);
  CU_ASSERT(!cdc_list_iter_has_next(&it));
  CU_ASSERT(cdc_list_iter_has_prev(&it));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_iter_data(&it)), d);
  cdc_list_dtor(l);
}

void test_list_reverse_iterators()
{
  struct cdc_list *l = NULL;
  struct cdc_list_iter it = CDC_INIT_STRUCT;
  struct cdc_list_iter ittmp = CDC_INIT_STRUCT;
  struct cdc_list_riter rit = CDC_INIT_STRUCT;
  struct cdc_list_riter rittmp = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_rbegin(l, &rit);
  CU_ASSERT(cdc_list_riter_has_next(&rit));
  CU_ASSERT(!cdc_list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_riter_data(&rit)), d);
  cdc_list_iter_from(&rit, &it);
  cdc_list_end(l, &ittmp);
  CU_ASSERT(cdc_list_iter_is_eq(&ittmp, &it));

  cdc_list_riter_next(&rit);
  CU_ASSERT(cdc_list_riter_has_next(&rit));
  CU_ASSERT(cdc_list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_riter_data(&rit)), c);
  cdc_list_iter_from(&rit, &it);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_iter_data(&it)), d);

  cdc_list_riter_next(&rit);
  CU_ASSERT(cdc_list_riter_has_next(&rit));
  CU_ASSERT(cdc_list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_riter_data(&rit)), b);
  cdc_list_iter_from(&rit, &it);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_iter_data(&it)), c);

  cdc_list_riter_next(&rit);
  CU_ASSERT(!cdc_list_riter_has_next(&rit));
  CU_ASSERT(cdc_list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_riter_data(&rit)), a);
  cdc_list_iter_from(&rit, &it);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_iter_data(&it)), b);

  cdc_list_riter_next(&rit);
  cdc_list_rend(l, &rittmp);
  CU_ASSERT(cdc_list_riter_is_eq(&rittmp, &rit));
  cdc_list_iter_from(&rit, &it);
  cdc_list_begin(l, &ittmp);
  CU_ASSERT(cdc_list_iter_is_eq(&ittmp, &it));

  cdc_list_riter_prev(&rit);
  CU_ASSERT(!cdc_list_riter_has_next(&rit));
  CU_ASSERT(cdc_list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_list_riter_data(&rit)), a);
  cdc_list_dtor(l);
}

void test_list_splice()
{
  struct cdc_list *l1 = NULL;
  struct cdc_list *l2 = NULL;
  struct cdc_list_iter current = CDC_INIT_STRUCT;
  struct cdc_list_iter first = CDC_INIT_STRUCT;
  struct cdc_list_iter last = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_ctorl(&l2, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_begin(l1, &current);
  cdc_list_iter_next(&current);

  cdc_list_begin(l2, &first);
  cdc_list_iter_next(&first);

  cdc_list_end(l2, &last);
  cdc_list_iter_prev(&last);

  cdc_list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 6);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 2);
  CU_ASSERT(list_range_int_eq(l1, 6, a, b, c, b, c, d));
  CU_ASSERT(list_range_int_eq(l2, 2, a, d));

  cdc_list_begin(l2, &current);
  cdc_list_iter_next(&current);
  cdc_list_begin(l1, &first);
  cdc_list_end(l1, &last);

  cdc_list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 0);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 8);
  CU_ASSERT(list_range_int_eq(l2, 8, a, a, b, c, b, c, d, d));

  cdc_list_end(l2, &last);
  cdc_list_iter_prev(&last);
  cdc_list_iter_prev(&last);
  cdc_list_iter_prev(&last);
  cdc_list_iter_prev(&last);
  cdc_list_begin(l1, &current);
  cdc_list_begin(l2, &first);

  cdc_list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 4);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 4);
  CU_ASSERT(list_range_int_eq(l1, 4, a, a, b, c));
  CU_ASSERT(list_range_int_eq(l2, 4, b, c, d, d));

  cdc_list_begin(l1, &current);
  cdc_list_begin(l2, &first);
  cdc_list_end(l2, &last);

  cdc_list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 8);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, b, c, d, d, a, a, b, c));

  cdc_list_end(l1, &last);
  cdc_list_iter_prev(&last);
  cdc_list_iter_prev(&last);
  cdc_list_iter_prev(&last);
  cdc_list_iter_prev(&last);
  cdc_list_begin(l2, &current);
  cdc_list_begin(l1, &first);

  cdc_list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 4);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 4);
  CU_ASSERT(list_range_int_eq(l1, 4, a, a, b, c));
  CU_ASSERT(list_range_int_eq(l2, 4, b, c, d, d));

  cdc_list_end(l1, &current);
  cdc_list_begin(l2, &first);
  cdc_list_end(l2, &last);

  cdc_list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 8);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, a, a, b, c, b, c, d, d));

  cdc_list_begin(l1, &first);
  cdc_list_iter_next(&first);
  cdc_list_iter_next(&first);
  cdc_list_iter_next(&first);
  cdc_list_iter_next(&first);
  cdc_list_begin(l2, &current);
  cdc_list_end(l1, &last);

  cdc_list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 4);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 4);
  CU_ASSERT(list_range_int_eq(l1, 4, a, a, b, c));
  CU_ASSERT(list_range_int_eq(l2, 4, b, c, d, d));

  cdc_list_dtor(l1);
  cdc_list_dtor(l2);
}

void test_list_ssplice()
{
  struct cdc_list *l1 = NULL;
  struct cdc_list *l2 = NULL;
  struct cdc_list_iter current = CDC_INIT_STRUCT;
  struct cdc_list_iter first = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_ctorl(&l2, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_begin(l1, &current);
  cdc_list_iter_next(&current);

  cdc_list_begin(l2, &first);
  cdc_list_iter_next(&first);

  cdc_list_ssplice(&current, &first);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 7);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 1);
  CU_ASSERT(list_range_int_eq(l1, 7, a, b, c, d, b, c, d));
  CU_ASSERT(list_range_int_eq(l2, 1, a));

  cdc_list_begin(l1, &current);
  cdc_list_begin(l2, &first);
  cdc_list_ssplice(&current, &first);
  CU_ASSERT_EQUAL(cdc_list_size(l1), 8);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, a, a, b, c, d, b, c, d));
  cdc_list_dtor(l1);
  cdc_list_dtor(l2);
}

void test_list_lsplice()
{
  struct cdc_list *l1 = NULL;
  struct cdc_list *l2 = NULL;
  struct cdc_list_iter current = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_ctorl(&l2, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_begin(l1, &current);
  cdc_list_iter_next(&current);

  cdc_list_lsplice(&current, l2);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 8);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, a, a, b, c, d, b, c, d));
  cdc_list_dtor(l1);
  cdc_list_dtor(l2);
}

void test_list_merge()
{
  struct cdc_list *l1 = NULL;
  struct cdc_list *l2 = NULL;
  int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6, h = 7;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(c),
                                 CDC_FROM_INT(e), CDC_FROM_INT(g), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_ctorl(&l2, NULL, CDC_FROM_INT(b), CDC_FROM_INT(d),
                                 CDC_FROM_INT(f), CDC_FROM_INT(h), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_cmerge(l1, l2, lt);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 8);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 0);

  CU_ASSERT(list_range_int_eq(l1, 8, a, b, c, d, e, f, g, h));

  cdc_list_dtor(l1);
  cdc_list_dtor(l2);

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_ctorl(&l2, NULL, CDC_FROM_INT(e), CDC_FROM_INT(f),
                                 CDC_FROM_INT(g), CDC_FROM_INT(h), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_cmerge(l1, l2, lt);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 8);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 0);

  CU_ASSERT(list_range_int_eq(l1, 8, a, b, c, d, e, f, g, h));

  cdc_list_dtor(l1);
  cdc_list_dtor(l2);

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l1, NULL, CDC_FROM_INT(e), CDC_FROM_INT(f),
                                 CDC_FROM_INT(g), CDC_FROM_INT(h), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_list_ctorl(&l2, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_cmerge(l1, l2, lt);

  CU_ASSERT_EQUAL(cdc_list_size(l1), 8);
  CU_ASSERT_EQUAL(cdc_list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, a, b, c, d, e, f, g, h));

  cdc_list_dtor(l1);
  cdc_list_dtor(l2);
}

void test_list_erase_if()
{
  struct cdc_list *l;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(c), CDC_FROM_INT(a),
                                 CDC_FROM_INT(b), CDC_FROM_INT(c),
                                 CDC_FROM_INT(c), CDC_FROM_INT(c),
                                 CDC_FROM_INT(d), CDC_FROM_INT(c), CDC_END),
                  CDC_STATUS_OK);

  cdc_list_erase_if(l, is_eq_2);
  CU_ASSERT(list_range_int_eq(l, 3, a, b, d));
  cdc_list_dtor(l);
}

void test_list_reverse()
{
  struct cdc_list *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  cdc_list_reverse(l);
  CU_ASSERT(list_range_int_eq(l, 4, d, c, b, a));
  cdc_list_dtor(l);
}

void test_list_unique()
{
  struct cdc_list *l = NULL;
  int a = 2, b = 1;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(a),
                                 CDC_FROM_INT(a), CDC_FROM_INT(a), CDC_END),
                  CDC_STATUS_OK);
  cdc_list_punique(l, eq);
  CU_ASSERT(list_range_int_eq(l, 1, a));
  cdc_list_clear(l);
  cdc_list_punique(l, eq);
  cdc_list_dtor(l);

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(b), CDC_FROM_INT(a), CDC_END),
                  CDC_STATUS_OK);
  cdc_list_punique(l, eq);
  CU_ASSERT(list_range_int_eq(l, 3, a, b, a));
  cdc_list_dtor(l);

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(a),
                                 CDC_FROM_INT(b), CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  cdc_list_punique(l, eq);
  CU_ASSERT(list_range_int_eq(l, 2, a, b));
  cdc_list_dtor(l);
}

void test_list_sort()
{
  struct cdc_list *l = NULL;
  int a = 2, b = 1, c = 4, d = 3;

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  cdc_list_csort(l, lt);
  CU_ASSERT(list_range_int_eq(l, 4, b, a, d, c));
  cdc_list_clear(l);
  cdc_list_csort(l, lt);
  cdc_list_dtor(l);

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_END),
                  CDC_STATUS_OK);
  cdc_list_csort(l, lt);
  CU_ASSERT(list_range_int_eq(l, 1, a));
  cdc_list_dtor(l);

  CU_ASSERT_EQUAL(
      cdc_list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  cdc_list_csort(l, lt);
  CU_ASSERT(list_range_int_eq(l, 2, b, a));
  cdc_list_dtor(l);

  CU_ASSERT_EQUAL(cdc_list_ctorl(&l, NULL, CDC_FROM_INT(c), CDC_FROM_INT(a),
                                 CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  cdc_list_csort(l, lt);
  CU_ASSERT(list_range_int_eq(l, 3, b, a, c));
  cdc_list_dtor(l);
}
