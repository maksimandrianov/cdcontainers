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
#include "cdcontainers/stack.h"

#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

void test_stackv_ctor()
{
  struct cdc_stack *s = NULL;

  CU_ASSERT_EQUAL(cdc_stackv_ctor(&s, NULL), CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_empty(s));
  cdc_stack_dtor(s);
}

void test_stackv_ctorl()
{
  struct cdc_stack *s = NULL;
  int a = 2, b = 3;

  CU_ASSERT_EQUAL(
      cdc_stackv_ctorl(&s, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_stack_size(s), 2);

  void *elem = cdc_stack_top(s);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);
  cdc_stack_pop(s);
  CU_ASSERT_EQUAL(cdc_stack_size(s), 1);

  elem = cdc_stack_top(s);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  cdc_stack_pop(s);
  CU_ASSERT(cdc_stack_empty(s));
  cdc_stack_dtor(s);
}

void test_stackv_push()
{
  struct cdc_stack *s = NULL;
  int a = 0, b = 1, c = 2;

  CU_ASSERT_EQUAL(cdc_stackv_ctor(&s, NULL), CDC_STATUS_OK);

  cdc_stack_push(s, CDC_FROM_INT(a));
  CU_ASSERT_EQUAL(cdc_stack_size(s), 1);
  void *elem = cdc_stack_top(s);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);

  cdc_stack_push(s, CDC_FROM_INT(b));
  CU_ASSERT_EQUAL(cdc_stack_size(s), 2);
  elem = cdc_stack_top(s);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  cdc_stack_push(s, CDC_FROM_INT(c));
  CU_ASSERT_EQUAL(cdc_stack_size(s), 3);
  elem = cdc_stack_top(s);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);
  cdc_stack_dtor(s);
}

void test_stackv_pop()
{
  struct cdc_stack *s = NULL;
  int a = 0, b = 1, c = 2, d = 3;

  CU_ASSERT_EQUAL(cdc_stackv_ctorl(&s, NULL, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                   CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  void *elem = cdc_stack_top(s);
  cdc_stack_pop(s);
  CU_ASSERT_EQUAL(cdc_stack_size(s), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), d);

  elem = cdc_stack_top(s);
  cdc_stack_pop(s);
  CU_ASSERT_EQUAL(cdc_stack_size(s), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), c);

  elem = cdc_stack_top(s);
  cdc_stack_pop(s);
  CU_ASSERT_EQUAL(cdc_stack_size(s), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), b);

  elem = cdc_stack_top(s);
  cdc_stack_pop(s);
  CU_ASSERT(cdc_stack_empty(s));
  CU_ASSERT_EQUAL(CDC_TO_INT(elem), a);
  cdc_stack_dtor(s);
}

void test_stackv_swap()
{
  struct cdc_stack *v = NULL;
  struct cdc_stack *w = NULL;
  int a = 2, b = 3, c = 4;

  CU_ASSERT_EQUAL(cdc_stackv_ctorl(&v, NULL, CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(
      cdc_stackv_ctorl(&w, NULL, CDC_FROM_INT(a), CDC_FROM_INT(c), CDC_END),
      CDC_STATUS_OK);

  cdc_stack_swap(v, w);

  CU_ASSERT_EQUAL(cdc_stack_size(v), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_stack_top(v)), c);
  CU_ASSERT_EQUAL(cdc_stack_size(w), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_stack_top(w)), b);
  cdc_stack_dtor(v);
  cdc_stack_dtor(w);
}
