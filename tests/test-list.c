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

#include "cdcontainers/casts.h"
#include "cdcontainers/common.h"
#include "cdcontainers/list.h"

#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

static bool list_range_int_eq(list_t *l, size_t count, ...)
{
  va_list args;
  stat_t ret;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    void *val = NULL;
    if ((ret = list_at(l, i, &val)) != CDC_STATUS_OK) {
      return false;
    }
    int elem = va_arg(args, int);
    if (elem != CDC_TO_INT(val)) {
      return false;
    }
  }

  return list_size(l) == count;
}

static inline void list_range_int_print(list_t *l)
{
  printf("\n");
  for (size_t i = 0; i < list_size(l); ++i) {
    void *val = NULL;
    if (list_at(l, i, &val) == CDC_STATUS_OK) {
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

static int is_eq_2(const void *v)
{
  return CDC_TO_INT(v) == 2;
}

void test_list_ctor()
{
  list_t *l = NULL;

  CU_ASSERT_EQUAL(list_ctor(&l, NULL), CDC_STATUS_OK);
  CU_ASSERT(list_empty(l));
  list_dtor(l);
}

void test_list_ctorl()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 4);
  CU_ASSERT(list_range_int_eq(l, 4, a, b, c, d));
  list_dtor(l);
}

void test_list_push_back()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(list_ctor(&l, NULL), CDC_STATUS_OK);
  CU_ASSERT(list_empty(l));

  CU_ASSERT_EQUAL(list_push_back(l, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, a));

  CU_ASSERT_EQUAL(list_push_back(l, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, a, b));

  CU_ASSERT_EQUAL(list_push_back(l, CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, a, b, c));
  list_dtor(l);
}

void test_list_push_front()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(list_ctor(&l, NULL), CDC_STATUS_OK);
  CU_ASSERT(list_empty(l));

  CU_ASSERT_EQUAL(list_push_front(l, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, a));

  CU_ASSERT_EQUAL(list_push_front(l, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, b, a));

  CU_ASSERT_EQUAL(list_push_front(l, CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, c, b, a));
  list_dtor(l);
}

void test_list_at()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2;
  void *elem = NULL;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c), CDC_END),
                  CDC_STATUS_OK);

  CU_ASSERT_EQUAL(list_at(l, 0, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  CU_ASSERT_EQUAL(list_at(l, 1, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);
  CU_ASSERT_EQUAL(list_at(l, 2, &elem), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  size_t index = list_size(l) + 1;
  CU_ASSERT_EQUAL(list_at(l, index, &elem), CDC_STATUS_OUT_OF_RANGE);
  list_dtor(l);
}

void test_list_front()
{
  list_t *l = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_front(l)), a);
  list_dtor(l);
}

void test_list_back()
{
  list_t *l = NULL;
  int a = 1, b = 2;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_back(l)), b);
  list_dtor(l);
}

void test_list_pop_back()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  void *elem = list_back(l);
  list_pop_back(l);
  CU_ASSERT_EQUAL(list_size(l), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);

  elem = list_back(l);
  list_pop_back(l);
  CU_ASSERT_EQUAL(list_size(l), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = list_back(l);
  list_pop_back(l);
  CU_ASSERT_EQUAL(list_size(l), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = list_back(l);
  list_pop_back(l);
  CU_ASSERT(list_empty(l));
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  list_dtor(l);
}

void test_list_pop_front()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  void *elem = list_front(l);
  list_pop_front(l);
  CU_ASSERT_EQUAL(list_size(l), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

  elem = list_front(l);
  list_pop_front(l);
  CU_ASSERT_EQUAL(list_size(l), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = list_front(l);
  list_pop_front(l);
  CU_ASSERT_EQUAL(list_size(l), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = list_front(l);
  list_pop_front(l);
  CU_ASSERT(list_empty(l));
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);
  list_dtor(l);
}

void test_list_swap()
{
  list_t *v = NULL;
  list_t *w = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&v, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_ctorl(&w, NULL, CDC_FROM_INT(a), CDC_FROM_INT(d), CDC_END), CDC_STATUS_OK);

  list_swap(v, w);

  CU_ASSERT_EQUAL(list_size(v), 2);
  CU_ASSERT(list_range_int_eq(v, 2, a, d));
  CU_ASSERT_EQUAL(list_size(w), 4);
  CU_ASSERT(list_range_int_eq(w, 4, a, b, c, d));

  list_dtor(v);
  list_dtor(w);
}

void test_list_insert()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(list_ctor(&l, NULL), CDC_STATUS_OK);

  CU_ASSERT_EQUAL(list_insert(l, 0, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, a));

  CU_ASSERT_EQUAL(list_insert(l, list_size(l), CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, a, c));

  CU_ASSERT_EQUAL(list_insert(l, 1, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, a, b, c));

  CU_ASSERT_EQUAL(list_insert(l, 1, CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_size(l), 4);
  CU_ASSERT(list_range_int_eq(l, 4, a, c, b, c));
  list_dtor(l);
}

void test_list_erase()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  list_erase(l, 0);
  CU_ASSERT_EQUAL(list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, b, c, d));

  list_erase(l, 1);
  CU_ASSERT_EQUAL(list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, b, d));

  list_erase(l, list_size(l) - 1);
  CU_ASSERT_EQUAL(list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, b));
  list_dtor(l);
}

void test_list_ierase()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;
  list_iter_t it = CDC_INIT_STRUCT;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  list_begin(l, &it);
  list_ierase(&it);
  CU_ASSERT_EQUAL(list_size(l), 3);
  CU_ASSERT(list_range_int_eq(l, 3, b, c, d));

  list_begin(l, &it);
  list_iter_next(&it);
  list_ierase(&it);
  CU_ASSERT_EQUAL(list_size(l), 2);
  CU_ASSERT(list_range_int_eq(l, 2, b, d));

  list_end(l, &it);
  list_iter_prev(&it);
  list_ierase(&it);
  CU_ASSERT_EQUAL(list_size(l), 1);
  CU_ASSERT(list_range_int_eq(l, 1, b));
  list_dtor(l);
}

void test_list_clear()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  list_clear(l);
  CU_ASSERT(list_empty(l));
  list_dtor(l);
}

void test_list_iterators()
{
  list_t *l = NULL;
  list_iter_t it = CDC_INIT_STRUCT;
  list_iter_t ittmp = CDC_INIT_STRUCT;
  list_riter_t rit = CDC_INIT_STRUCT;
  list_riter_t rittmp = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  list_begin(l, &it);
  CU_ASSERT_EQUAL(list_iter_has_next(&it), true);
  CU_ASSERT_EQUAL(list_iter_has_prev(&it), false);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_iter_data(&it)), a);
  list_riter_from(&it, &rit);
  list_rend(l, &rittmp);
  CU_ASSERT(list_riter_is_eq(&rittmp, &rit));

  list_iter_next(&it);
  CU_ASSERT(list_iter_has_next(&it));
  CU_ASSERT(list_iter_has_prev(&it));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_iter_data(&it)), b);
  list_riter_from(&it, &rit);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_riter_data(&rit)), a);

  list_iter_next(&it);
  CU_ASSERT(list_iter_has_next(&it));
  CU_ASSERT(list_iter_has_prev(&it));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_iter_data(&it)), c);
  list_riter_from(&it, &rit);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_riter_data(&rit)), b);

  list_iter_next(&it);
  CU_ASSERT(!list_iter_has_next(&it));
  CU_ASSERT(list_iter_has_prev(&it));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_iter_data(&it)), d);
  list_riter_from(&it, &rit);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_riter_data(&rit)), c);

  list_iter_next(&it);
  list_end(l, &ittmp);
  CU_ASSERT(list_iter_is_eq(&ittmp, &it));
  list_riter_from(&it, &rit);
  list_rbegin(l, &rittmp);
  CU_ASSERT(list_riter_is_eq(&rittmp, &rit));

  list_iter_prev(&it);
  CU_ASSERT(!list_iter_has_next(&it));
  CU_ASSERT(list_iter_has_prev(&it));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_iter_data(&it)), d);
  list_dtor(l);
}

void test_list_reverse_iterators()
{
  list_t *l = NULL;
  list_iter_t it = CDC_INIT_STRUCT;
  list_iter_t ittmp = CDC_INIT_STRUCT;
  list_riter_t rit = CDC_INIT_STRUCT;
  list_riter_t rittmp = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  list_rbegin(l, &rit);
  CU_ASSERT(list_riter_has_next(&rit));
  CU_ASSERT(!list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_riter_data(&rit)), d);
  list_iter_from(&rit, &it);
  list_end(l, &ittmp);
  CU_ASSERT(list_iter_is_eq(&ittmp, &it));

  list_riter_next(&rit);
  CU_ASSERT(list_riter_has_next(&rit));
  CU_ASSERT(list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_riter_data(&rit)), c);
  list_iter_from(&rit, &it);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_iter_data(&it)), d);

  list_riter_next(&rit);
  CU_ASSERT(list_riter_has_next(&rit));
  CU_ASSERT(list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_riter_data(&rit)), b);
  list_iter_from(&rit, &it);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_iter_data(&it)), c);

  list_riter_next(&rit);
  CU_ASSERT(!list_riter_has_next(&rit));
  CU_ASSERT(list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_riter_data(&rit)), a);
  list_iter_from(&rit, &it);
  CU_ASSERT_EQUAL(CDC_TO_INT(list_iter_data(&it)), b);

  list_riter_next(&rit);
  list_rend(l, &rittmp);
  CU_ASSERT(list_riter_is_eq(&rittmp, &rit));
  list_iter_from(&rit, &it);
  list_begin(l, &ittmp);
  CU_ASSERT(list_iter_is_eq(&ittmp, &it));

  list_riter_prev(&rit);
  CU_ASSERT(!list_riter_has_next(&rit));
  CU_ASSERT(list_riter_has_prev(&rit));
  CU_ASSERT_EQUAL(CDC_TO_INT(list_riter_data(&rit)), a);
  list_dtor(l);
}

void test_list_splice()
{
  list_t *l1 = NULL;
  list_t *l2 = NULL;
  list_iter_t current = CDC_INIT_STRUCT;
  list_iter_t first = CDC_INIT_STRUCT;
  list_iter_t last = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_ctorl(&l2, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  list_begin(l1, &current);
  list_iter_next(&current);

  list_begin(l2, &first);
  list_iter_next(&first);

  list_end(l2, &last);
  list_iter_prev(&last);

  list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(list_size(l1), 6);
  CU_ASSERT_EQUAL(list_size(l2), 2);
  CU_ASSERT(list_range_int_eq(l1, 6, a, b, c, b, c, d));
  CU_ASSERT(list_range_int_eq(l2, 2, a, d));

  list_begin(l2, &current);
  list_iter_next(&current);
  list_begin(l1, &first);
  list_end(l1, &last);

  list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(list_size(l1), 0);
  CU_ASSERT_EQUAL(list_size(l2), 8);
  CU_ASSERT(list_range_int_eq(l2, 8, a, a, b, c, b, c, d, d));

  list_end(l2, &last);
  list_iter_prev(&last);
  list_iter_prev(&last);
  list_iter_prev(&last);
  list_iter_prev(&last);
  list_begin(l1, &current);
  list_begin(l2, &first);

  list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(list_size(l1), 4);
  CU_ASSERT_EQUAL(list_size(l2), 4);
  CU_ASSERT(list_range_int_eq(l1, 4, a, a, b, c));
  CU_ASSERT(list_range_int_eq(l2, 4, b, c, d, d));

  list_begin(l1, &current);
  list_begin(l2, &first);
  list_end(l2, &last);

  list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(list_size(l1), 8);
  CU_ASSERT_EQUAL(list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, b, c, d, d, a, a, b, c));

  list_end(l1, &last);
  list_iter_prev(&last);
  list_iter_prev(&last);
  list_iter_prev(&last);
  list_iter_prev(&last);
  list_begin(l2, &current);
  list_begin(l1, &first);

  list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(list_size(l1), 4);
  CU_ASSERT_EQUAL(list_size(l2), 4);
  CU_ASSERT(list_range_int_eq(l1, 4, a, a, b, c));
  CU_ASSERT(list_range_int_eq(l2, 4, b, c, d, d));

  list_end(l1, &current);
  list_begin(l2, &first);
  list_end(l2, &last);

  list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(list_size(l1), 8);
  CU_ASSERT_EQUAL(list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, a, a, b, c, b, c, d, d));

  list_begin(l1, &first);
  list_iter_next(&first);
  list_iter_next(&first);
  list_iter_next(&first);
  list_iter_next(&first);
  list_begin(l2, &current);
  list_end(l1, &last);

  list_splice(&current, &first, &last);

  CU_ASSERT_EQUAL(list_size(l1), 4);
  CU_ASSERT_EQUAL(list_size(l2), 4);
  CU_ASSERT(list_range_int_eq(l1, 4, a, a, b, c));
  CU_ASSERT(list_range_int_eq(l2, 4, b, c, d, d));

  list_dtor(l1);
  list_dtor(l2);
}

void test_list_ssplice()
{
  list_t *l1 = NULL;
  list_t *l2 = NULL;
  list_iter_t current = CDC_INIT_STRUCT;
  list_iter_t first = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_ctorl(&l2, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  list_begin(l1, &current);
  list_iter_next(&current);

  list_begin(l2, &first);
  list_iter_next(&first);

  list_ssplice(&current, &first);

  CU_ASSERT_EQUAL(list_size(l1), 7);
  CU_ASSERT_EQUAL(list_size(l2), 1);
  CU_ASSERT(list_range_int_eq(l1, 7, a, b, c, d, b, c, d));
  CU_ASSERT(list_range_int_eq(l2, 1, a));

  list_begin(l1, &current);
  list_begin(l2, &first);
  list_ssplice(&current, &first);
  CU_ASSERT_EQUAL(list_size(l1), 8);
  CU_ASSERT_EQUAL(list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, a, a, b, c, d, b, c, d));
  list_dtor(l1);
  list_dtor(l2);
}

void test_list_lsplice()
{
  list_t *l1 = NULL;
  list_t *l2 = NULL;
  list_iter_t current = CDC_INIT_STRUCT;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_ctorl(&l2, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  list_begin(l1, &current);
  list_iter_next(&current);

  list_lsplice(&current, l2);

  CU_ASSERT_EQUAL(list_size(l1), 8);
  CU_ASSERT_EQUAL(list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, a, a, b, c, d, b, c, d));
  list_dtor(l1);
  list_dtor(l2);
}

void test_list_merge()
{
  list_t *l1 = NULL;
  list_t *l2 = NULL;
  int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6, h = 7;

  CU_ASSERT_EQUAL(list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(c), CDC_FROM_INT(e),
                             CDC_FROM_INT(g), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_ctorl(&l2, NULL, CDC_FROM_INT(b), CDC_FROM_INT(d), CDC_FROM_INT(f),
                             CDC_FROM_INT(h), CDC_END),
                  CDC_STATUS_OK);

  list_cmerge(l1, l2, lt);

  CU_ASSERT_EQUAL(list_size(l1), 8);
  CU_ASSERT_EQUAL(list_size(l2), 0);

  CU_ASSERT(list_range_int_eq(l1, 8, a, b, c, d, e, f, g, h));

  list_dtor(l1);
  list_dtor(l2);

  CU_ASSERT_EQUAL(list_ctorl(&l1, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_ctorl(&l2, NULL, CDC_FROM_INT(e), CDC_FROM_INT(f), CDC_FROM_INT(g),
                             CDC_FROM_INT(h), CDC_END),
                  CDC_STATUS_OK);

  list_cmerge(l1, l2, lt);

  CU_ASSERT_EQUAL(list_size(l1), 8);
  CU_ASSERT_EQUAL(list_size(l2), 0);

  CU_ASSERT(list_range_int_eq(l1, 8, a, b, c, d, e, f, g, h));

  list_dtor(l1);
  list_dtor(l2);

  CU_ASSERT_EQUAL(list_ctorl(&l1, NULL, CDC_FROM_INT(e), CDC_FROM_INT(f), CDC_FROM_INT(g),
                             CDC_FROM_INT(h), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(list_ctorl(&l2, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  list_cmerge(l1, l2, lt);

  CU_ASSERT_EQUAL(list_size(l1), 8);
  CU_ASSERT_EQUAL(list_size(l2), 0);
  CU_ASSERT(list_range_int_eq(l1, 8, a, b, c, d, e, f, g, h));

  list_dtor(l1);
  list_dtor(l2);
}

void test_list_erase_if()
{
  list_t *l;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(
      list_ctorl(&l, NULL, CDC_FROM_INT(c), CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                 CDC_FROM_INT(c), CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_FROM_INT(c), CDC_END),
      CDC_STATUS_OK);

  list_erase_if(l, is_eq_2);
  CU_ASSERT(list_range_int_eq(l, 3, a, b, d));
  list_dtor(l);
}

void test_list_reverse()
{
  list_t *l = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  list_reverse(l);
  CU_ASSERT(list_range_int_eq(l, 4, d, c, b, a));
  list_dtor(l);
}

void test_list_unique()
{
  list_t *l = NULL;
  int a = 2, b = 1;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(a), CDC_FROM_INT(a),
                             CDC_FROM_INT(a), CDC_END),
                  CDC_STATUS_OK);
  list_punique(l, eq);
  CU_ASSERT(list_range_int_eq(l, 1, a));
  list_clear(l);
  list_punique(l, eq);
  list_dtor(l);

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(b),
                             CDC_FROM_INT(a), CDC_END),
                  CDC_STATUS_OK);
  list_punique(l, eq);
  CU_ASSERT(list_range_int_eq(l, 3, a, b, a));
  list_dtor(l);

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(a), CDC_FROM_INT(b),
                             CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  list_punique(l, eq);
  CU_ASSERT(list_range_int_eq(l, 2, a, b));
  list_dtor(l);
}

void test_list_sort()
{
  list_t *l = NULL;
  int a = 2, b = 1, c = 4, d = 3;

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_FROM_INT(c),
                             CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);
  list_csort(l, lt);
  CU_ASSERT(list_range_int_eq(l, 4, b, a, d, c));
  list_clear(l);
  list_csort(l, lt);
  list_dtor(l);

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_END), CDC_STATUS_OK);
  list_csort(l, lt);
  CU_ASSERT(list_range_int_eq(l, 1, a));
  list_dtor(l);

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END), CDC_STATUS_OK);
  list_csort(l, lt);
  CU_ASSERT(list_range_int_eq(l, 2, b, a));
  list_dtor(l);

  CU_ASSERT_EQUAL(list_ctorl(&l, NULL, CDC_FROM_INT(c), CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  list_csort(l, lt);
  CU_ASSERT(list_range_int_eq(l, 3, b, a, c));
  list_dtor(l);
}
