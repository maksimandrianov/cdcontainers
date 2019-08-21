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

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>
#include "cdcontainers/stack.h"

void test_stackv_ctor()
{
  struct cdc_stack *s;

  CU_ASSERT(cdc_stackv_ctor(&s, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_size(s) == 0);

  cdc_stack_dtor(s);
}

void test_stackv_ctorl()
{
  struct cdc_stack *s;
  int a = 2, b = 3;
  void *elem;

  CU_ASSERT(cdc_stackv_ctorl(&s, NULL, &a, &b, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_size(s) == 2);

  elem = cdc_stack_top(s);
  CU_ASSERT(*((int *)elem) == b);
  CU_ASSERT(cdc_stack_pop(s) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_size(s) == 1);

  elem = cdc_stack_top(s);
  CU_ASSERT(*((int *)elem) == a);
  CU_ASSERT(cdc_stack_pop(s) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_size(s) == 0);

  cdc_stack_dtor(s);
}

void test_stackv_push()
{
  struct cdc_stack *s;
  int a = 0, b = 1, c = 2;
  void *elem;

  CU_ASSERT(cdc_stackv_ctor(&s, NULL) == CDC_STATUS_OK);

  cdc_stack_push(s, &a);
  CU_ASSERT(cdc_stack_size(s) == 1);
  elem = cdc_stack_top(s);
  CU_ASSERT(*((int *)elem) == a);

  cdc_stack_push(s, &b);
  CU_ASSERT(cdc_stack_size(s) == 2);
  elem = cdc_stack_top(s);
  CU_ASSERT(*((int *)elem) == b);

  cdc_stack_push(s, &c);
  CU_ASSERT(cdc_stack_size(s) == 3);
  elem = cdc_stack_top(s);
  CU_ASSERT(*((int *)elem) == c);

  cdc_stack_dtor(s);
}

void test_stackv_pop()
{
  struct cdc_stack *s;
  int a = 0, b = 1, c = 2, d = 3;
  void *elem;

  CU_ASSERT(cdc_stackv_ctorl(&s, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

  elem = cdc_stack_top(s);
  CU_ASSERT(cdc_stack_pop(s) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_size(s) == 3);
  CU_ASSERT(*((int *)elem) == d);

  elem = cdc_stack_top(s);
  CU_ASSERT(cdc_stack_pop(s) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_size(s) == 2);
  CU_ASSERT(*((int *)elem) == c);

  elem = cdc_stack_top(s);
  CU_ASSERT(cdc_stack_pop(s) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_size(s) == 1);
  CU_ASSERT(*((int *)elem) == b);

  elem = cdc_stack_top(s);
  CU_ASSERT(cdc_stack_pop(s) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stack_size(s) == 0);
  CU_ASSERT(*((int *)elem) == a);

  cdc_stack_dtor(s);
}

void test_stackv_swap()
{
  struct cdc_stack *v, *w;
  int a = 2, b = 3, c = 4;

  CU_ASSERT(cdc_stackv_ctorl(&v, NULL, &b, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_stackv_ctorl(&w, NULL, &a, &c, NULL) == CDC_STATUS_OK);

  cdc_stack_swap(v, w);

  CU_ASSERT(cdc_stack_size(v) == 2);
  CU_ASSERT(*((int *)cdc_stack_top(v)) == c);
  CU_ASSERT(cdc_stack_size(w) == 1);
  CU_ASSERT(*((int *)cdc_stack_top(w)) == b);

  cdc_stack_dtor(v);
  cdc_stack_dtor(w);
}
