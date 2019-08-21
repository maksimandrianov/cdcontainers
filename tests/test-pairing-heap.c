// The MIT License (MIT)
// Copyright (c) 2018 Maksim Andrianov
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
#include <assert.h>
#include <float.h>
#include <stdarg.h>
#include "cdcontainers/global.h"
#include "cdcontainers/pairing-heap.h"

static size_t count_free = 0;

static int gt_int(const void *a, const void *b)
{
  return *((int *)a) > *((int *)b);
}

static int gt_ptr(const void *a, const void *b) { return a > b; }

static void test_free(void *ptr)
{
  CDC_UNUSED(ptr);

  ++count_free;
}

static inline void heap_int_print(struct cdc_pairing_heap_node *node)
{
  while (node) {
    printf("%d ", *((int *)node->key));
    heap_int_print(node->child);
    node = node->sibling;
  }
}

void test_pairing_heap_ctor()
{
  struct cdc_pairing_heap *h;

  CU_ASSERT(cdc_pairing_heap_ctor1(&h, NULL, gt_int) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 0);

  cdc_pairing_heap_dtor(h);
}

void test_pairing_heap_ctorl()
{
  struct cdc_pairing_heap *h;
  int a = 2;
  void *elem;

  CU_ASSERT(cdc_pairing_heap_ctorl1(&h, NULL, gt_int, &a, &a, NULL) ==
            CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_is_heap(h) == true);
  CU_ASSERT(cdc_pairing_heap_size(h) == 2);

  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(*((int *)elem) == a);
  CU_ASSERT(cdc_pairing_heap_extract_top(h) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 1);

  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(*((int *)elem) == a);
  CU_ASSERT(cdc_pairing_heap_extract_top(h) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 0);

  cdc_pairing_heap_dtor(h);
}

void test_pairing_heap_dtor()
{
  struct cdc_pairing_heap *h;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  size_t i;
  const size_t count = 9;

  info.dfree = test_free;
  CU_ASSERT(cdc_pairing_heap_ctor1(&h, &info, gt_ptr) == CDC_STATUS_OK);

  for (i = count; i > 0; --i)
    CU_ASSERT(cdc_pairing_heap_insert(h, (size_t *)i) == CDC_STATUS_OK);

  for (i = 0; i < count / 2; ++i)
    CU_ASSERT(cdc_pairing_heap_insert(h, (size_t *)i) == CDC_STATUS_OK);

  CU_ASSERT(cdc_pairing_heap_is_heap(h) == true);
  CU_ASSERT(cdc_pairing_heap_size(h) == count + count / 2);
  CU_ASSERT(cdc_pairing_heap_top(h) == (void *)((size_t *)count));

  cdc_pairing_heap_dtor(h);

  CU_ASSERT(count_free == count + count / 2);
  count_free = 0;
}

void test_pairing_heap_top()
{
  struct cdc_pairing_heap *h;
  int a = 1, b = 10, c = 2;

  CU_ASSERT(cdc_pairing_heap_ctorl1(&h, NULL, gt_int, &a, &b, &c, NULL) ==
            CDC_STATUS_OK);
  CU_ASSERT(*((int *)cdc_pairing_heap_top(h)) == b);

  cdc_pairing_heap_dtor(h);
}

void test_pairing_heap_extract_top()
{
  struct cdc_pairing_heap *h;
  int a = 0, b = 3, c = 2, d = 1;
  void *elem;

  CU_ASSERT(cdc_pairing_heap_ctorl1(&h, NULL, gt_int, &a, &b, &c, &d, NULL) ==
            CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_is_heap(h) == true);

  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(cdc_pairing_heap_extract_top(h) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 3);
  CU_ASSERT(*((int *)elem) == b);

  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(cdc_pairing_heap_extract_top(h) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 2);
  CU_ASSERT(*((int *)elem) == c);

  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(cdc_pairing_heap_extract_top(h) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 1);
  CU_ASSERT(*((int *)elem) == d);

  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(cdc_pairing_heap_extract_top(h) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 0);
  CU_ASSERT(*((int *)elem) == a);

  cdc_pairing_heap_dtor(h);
}

void test_pairing_heap_insert()
{
  struct cdc_pairing_heap *h;
  int a = 0, b = 1, c = 2;
  void *elem;

  CU_ASSERT(cdc_pairing_heap_ctor1(&h, NULL, gt_int) == CDC_STATUS_OK);

  CU_ASSERT(cdc_pairing_heap_insert(h, &a) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 1);
  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(*((int *)elem) == a);

  CU_ASSERT(cdc_pairing_heap_insert(h, &c) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 2);
  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(*((int *)elem) == c);

  CU_ASSERT(cdc_pairing_heap_insert(h, &b) == CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_size(h) == 3);
  elem = cdc_pairing_heap_top(h);
  CU_ASSERT(*((int *)elem) == c);

  cdc_pairing_heap_dtor(h);
}

void test_pairing_heap_change_key()
{
  //        struct cdc_pairing_heap *h;
  //        struct cdc_pairing_heap_iter iter1;
  //        int a = 0, b = 4, c = 3, d = 1, n = 2, max_key = 10, min_key = -1;
  //        void *elem;

  //        CU_ASSERT(cdc_pairing_heap_ctorl(&h, NULL, gt_int, &a, &b, &c, &d,
  //        NULL) == CDC_STATUS_OK); CU_ASSERT(cdc_pairing_heap_is_heap(h) ==
  //        true); CU_ASSERT(cdc_pairing_heap_riinsert(h, &n, &iter1) ==
  //        CDC_STATUS_OK);

  //        CU_ASSERT(cdc_pairing_heap_insert(h, &a) == CDC_STATUS_OK);
  //        CU_ASSERT(cdc_pairing_heap_insert(h, &c) == CDC_STATUS_OK);
  //        CU_ASSERT(cdc_pairing_heap_insert(h, &b) == CDC_STATUS_OK);

  //        cdc_pairing_heap_change_key(h, &iter1, &max_key);
  //        CU_ASSERT(cdc_pairing_heap_is_heap(h) == true);
  //        elem = cdc_pairing_heap_top(h);
  //        CU_ASSERT(*((int *)elem) == max_key);
  //        CU_ASSERT(*((int *)cdc_pairing_heap_iter_data(iter1)) == max_key);

  //        cdc_pairing_heap_change_key(h, &iter1, &max_key);
  //        CU_ASSERT(cdc_pairing_heap_is_heap(h) == true);
  //        elem = cdc_pairing_heap_top(h);
  //        CU_ASSERT(*((int *)elem) == max_key);
  //        CU_ASSERT(*((int *)cdc_pairing_heap_iter_data(iter1)) == max_key);

  //        cdc_pairing_heap_change_key(h, &iter1, &min_key);
  //        CU_ASSERT(cdc_pairing_heap_is_heap(h) == true);
  //        elem = cdc_pairing_heap_top(h);
  //        CU_ASSERT(*((int *)elem) == b);
  //        CU_ASSERT(*((int *)cdc_pairing_heap_iter_data(iter1)) == min_key);

  //        cdc_pairing_heap_dtor(h);
}

void test_pairing_heap_merge()
{
  struct cdc_pairing_heap *v, *w;
  int a = 2, b = 3, c = 4;

  CU_ASSERT(cdc_pairing_heap_ctorl1(&v, NULL, gt_int, &b, NULL) ==
            CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_ctorl1(&w, NULL, gt_int, &a, &c, NULL) ==
            CDC_STATUS_OK);

  cdc_pairing_heap_merge(v, w);

  CU_ASSERT(cdc_pairing_heap_size(v) == 3);
  CU_ASSERT(*((int *)cdc_pairing_heap_top(v)) == 4);
  CU_ASSERT(cdc_pairing_heap_size(w) == 0);

  cdc_pairing_heap_dtor(v);
  cdc_pairing_heap_dtor(w);
}

void test_pairing_heap_swap()
{
  struct cdc_pairing_heap *v, *w;
  int a = 2, b = 3, c = 4;

  CU_ASSERT(cdc_pairing_heap_ctorl1(&v, NULL, gt_int, &b, NULL) ==
            CDC_STATUS_OK);
  CU_ASSERT(cdc_pairing_heap_ctorl1(&w, NULL, gt_int, &a, &c, NULL) ==
            CDC_STATUS_OK);

  cdc_pairing_heap_swap(v, w);

  CU_ASSERT(cdc_pairing_heap_size(v) == 2);
  CU_ASSERT(*((int *)cdc_pairing_heap_top(v)) == c);
  CU_ASSERT(cdc_pairing_heap_size(w) == 1);
  CU_ASSERT(*((int *)cdc_pairing_heap_top(w)) == b);

  cdc_pairing_heap_dtor(v);
  cdc_pairing_heap_dtor(w);
}
