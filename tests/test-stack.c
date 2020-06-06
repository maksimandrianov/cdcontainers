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

#include "cdcontainers/adapters/stack.h"
#include "cdcontainers/casts.h"

#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

void test_stackv_ctor()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    cstack_t *s = NULL;

    CU_ASSERT_EQUAL(stack_ctor(tables[t], &s, NULL), CDC_STATUS_OK);
    CU_ASSERT(stack_empty(s));
    stack_dtor(s);
  }
}

void test_stackv_ctorl()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 2, b = 3;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    cstack_t *s = NULL;

    CU_ASSERT_EQUAL(stack_ctorl(tables[t], &s, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(stack_size(s), 2);

    void *elem = stack_top(s);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);
    stack_pop(s);
    CU_ASSERT_EQUAL(stack_size(s), 1);

    elem = stack_top(s);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
    stack_pop(s);
    CU_ASSERT(stack_empty(s));
    stack_dtor(s);
  }
}

void test_stackv_push()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 0, b = 1, c = 2;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    cstack_t *s = NULL;

    CU_ASSERT_EQUAL(stack_ctor(tables[t], &s, NULL), CDC_STATUS_OK);

    stack_push(s, CDC_FROM_INT(a));
    CU_ASSERT_EQUAL(stack_size(s), 1);
    void *elem = stack_top(s);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

    stack_push(s, CDC_FROM_INT(b));
    CU_ASSERT_EQUAL(stack_size(s), 2);
    elem = stack_top(s);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

    stack_push(s, CDC_FROM_INT(c));
    CU_ASSERT_EQUAL(stack_size(s), 3);
    elem = stack_top(s);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);
    stack_dtor(s);
  }
}

void test_stackv_pop()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 0, b = 1, c = 2, d = 3;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    cstack_t *s = NULL;

    CU_ASSERT_EQUAL(stack_ctorl(tables[t], &s, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                    CDC_STATUS_OK);

    void *elem = stack_top(s);
    stack_pop(s);
    CU_ASSERT_EQUAL(stack_size(s), 3);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);

    elem = stack_top(s);
    stack_pop(s);
    CU_ASSERT_EQUAL(stack_size(s), 2);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

    elem = stack_top(s);
    stack_pop(s);
    CU_ASSERT_EQUAL(stack_size(s), 1);
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

    elem = stack_top(s);
    stack_pop(s);
    CU_ASSERT(stack_empty(s));
    CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
    stack_dtor(s);
  }
}

void test_stackv_swap()
{
  const sequence_table_t *tables[] = {cdc_seq_carray, cdc_seq_array, cdc_seq_list};
  int a = 2, b = 3, c = 4;

  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    cstack_t *v = NULL;
    cstack_t *w = NULL;

    CU_ASSERT_EQUAL(stack_ctorl(tables[t], &v, NULL, CDC_FROM_INT(b), CDC_END), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(stack_ctorl(tables[t], &w, NULL, CDC_FROM_INT(a), CDC_FROM_INT(c), CDC_END),
                    CDC_STATUS_OK);

    stack_swap(v, w);

    CU_ASSERT_EQUAL(stack_size(v), 2);
    CU_ASSERT_EQUAL(CDC_TO_INT(stack_top(v)), c);
    CU_ASSERT_EQUAL(stack_size(w), 1);
    CU_ASSERT_EQUAL(CDC_TO_INT(stack_top(w)), b);
    stack_dtor(v);
    stack_dtor(w);
  }
}
