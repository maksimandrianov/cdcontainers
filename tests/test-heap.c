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
#include "cdcontainers/heap.h"

#include <assert.h>
#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

static int gt(const void *a, const void *b)
{
  return CDC_TO_INT(a) > CDC_TO_INT(b);
}

static inline void heap_int_print(struct cdc_heap *h)
{
  for (size_t i = 0; i < cdc_heap_size(h); ++i) {
    printf("%d ", CDC_TO_INT(cdc_vector_get(h->vector, i)));
  }
}

void test_heap_ctor()
{
  struct cdc_heap *h = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = gt;
  CU_ASSERT_EQUAL(cdc_heap_ctor(&h, &info), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 0);
  cdc_heap_dtor(h);
}

void test_heap_ctorl()
{
  struct cdc_heap *h = NULL;
  int a = 2, b = 3;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = gt;
  CU_ASSERT_EQUAL(
      cdc_heap_ctorl(&h, &info, CDC_FROM_INT(a), CDC_FROM_INT(b), CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 2);

  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), b);
  CU_ASSERT_EQUAL(cdc_heap_extract_top(h), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 1);

  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), a);
  CU_ASSERT_EQUAL(cdc_heap_extract_top(h), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 0);
  cdc_heap_dtor(h);
}

void test_heap_top()
{
  struct cdc_heap *h = NULL;
  int a = 1, b = 10, c = 2;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = gt;
  CU_ASSERT_EQUAL(cdc_heap_ctorl(&h, &info, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), b);
  cdc_heap_dtor(h);
}

void test_heap_extract_top()
{
  struct cdc_heap *h = NULL;
  int a = 0, b = 3, c = 2, d = 1;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = gt;
  CU_ASSERT_EQUAL(cdc_heap_ctorl(&h, &info, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), b);
  CU_ASSERT_EQUAL(cdc_heap_extract_top(h), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 3);

  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), c);
  CU_ASSERT_EQUAL(cdc_heap_extract_top(h), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 2);

  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), d);
  CU_ASSERT_EQUAL(cdc_heap_extract_top(h), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 1);

  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), a);
  CU_ASSERT_EQUAL(cdc_heap_extract_top(h), CDC_STATUS_OK);
  CU_ASSERT(cdc_heap_empty(h));
  cdc_heap_dtor(h);
}

void test_heap_insert()
{
  struct cdc_heap *h = NULL;
  int a = 0, b = 1, c = 2;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = gt;
  CU_ASSERT_EQUAL(cdc_heap_ctor(&h, &info), CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_heap_insert(h, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), a);

  CU_ASSERT_EQUAL(cdc_heap_insert(h, CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), c);

  CU_ASSERT_EQUAL(cdc_heap_insert(h, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_size(h), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), c);
  cdc_heap_dtor(h);
}

void test_heap_change_key()
{
  struct cdc_heap *h = NULL;
  struct cdc_heap_iter iter1 = CDC_INIT_STRUCT;
  int a = 0, b = 4, c = 3, d = 1;
  int n = 2, max_key = 10, min_key = -1;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = gt;
  CU_ASSERT_EQUAL(cdc_heap_ctorl(&h, &info, CDC_FROM_INT(a), CDC_FROM_INT(b),
                                 CDC_FROM_INT(c), CDC_FROM_INT(d), CDC_END),
                  CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_heap_riinsert(h, CDC_FROM_INT(n), &iter1), CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_heap_insert(h, CDC_FROM_INT(a)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_insert(h, CDC_FROM_INT(c)), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_heap_insert(h, CDC_FROM_INT(b)), CDC_STATUS_OK);
  CU_ASSERT(cdc_heap_is_heap(h));

  cdc_heap_change_key(h, &iter1, CDC_FROM_INT(max_key));
  CU_ASSERT(cdc_heap_is_heap(h));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), max_key);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_iter_data(&iter1)), max_key);

  cdc_heap_change_key(h, &iter1, CDC_FROM_INT(min_key));
  CU_ASSERT(cdc_heap_is_heap(h));
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(h)), b);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_iter_data(&iter1)), min_key);
  cdc_heap_dtor(h);
}

void test_heap_merge()
{
  struct cdc_heap *v = NULL, *w = NULL;
  int a = 2, b = 3, c = 4;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = gt;

  CU_ASSERT_EQUAL(cdc_heap_ctorl(&v, &info, CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(
      cdc_heap_ctorl(&w, &info, CDC_FROM_INT(a), CDC_FROM_INT(c), CDC_END),
      CDC_STATUS_OK);

  cdc_heap_merge(v, w);
  CU_ASSERT(cdc_heap_is_heap(v));
  CU_ASSERT_EQUAL(cdc_heap_size(v), 3);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(v)), 4);
  CU_ASSERT(cdc_heap_empty(w));
  cdc_heap_dtor(v);
  cdc_heap_dtor(w);
}

void test_heap_swap()
{
  struct cdc_heap *v = NULL, *w = NULL;
  int a = 2, b = 3, c = 4;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = gt;

  CU_ASSERT_EQUAL(cdc_heap_ctorl(&v, &info, CDC_FROM_INT(b), CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(
      cdc_heap_ctorl(&w, &info, CDC_FROM_INT(a), CDC_FROM_INT(c), CDC_END),
      CDC_STATUS_OK);

  cdc_heap_swap(v, w);
  CU_ASSERT_EQUAL(cdc_heap_size(v), 2);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(v)), c);
  CU_ASSERT_EQUAL(cdc_heap_size(w), 1);
  CU_ASSERT_EQUAL(CDC_TO_INT(cdc_heap_top(w)), b);
  cdc_heap_dtor(v);
  cdc_heap_dtor(w);
}
