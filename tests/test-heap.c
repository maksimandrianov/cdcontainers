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
#include <assert.h>
#include "cdcontainers/heap.h"

static int gt_int(const void *a, const void *b)
{
        return *((int *)a) > *((int *)b);
}

void test_heap_ctor()
{
        struct cdc_heap *h;

        CU_ASSERT(cdc_heap_ctor(&h, NULL, gt_int) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 0);

        cdc_heap_dtor(h);
}

void test_heap_ctorl()
{
        struct cdc_heap *h;
        int a = 2, b = 3;
        void *elem;

        CU_ASSERT(cdc_heap_ctorl(&h, NULL, gt_int, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 2);

        elem = cdc_heap_top(h);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_heap_extract_top(h) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 1);

        elem = cdc_heap_top(h);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_heap_extract_top(h) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 0);

        cdc_heap_dtor(h);
}

void test_heap_top()
{
        struct cdc_heap *h;
        int a = 1, b = 10, c = 2;

        CU_ASSERT(cdc_heap_ctorl(&h, NULL, gt_int, &a, &b, &c, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_heap_top(h)) == b);

        cdc_heap_dtor(h);
}

void test_heap_extract_top()
{
        struct cdc_heap *h;
        int a = 0, b = 3, c = 2, d = 1;
        void *elem;

        CU_ASSERT(cdc_heap_ctorl(&h, NULL, gt_int, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_heap_top(h);
        CU_ASSERT(cdc_heap_extract_top(h) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 3);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_heap_top(h);
        CU_ASSERT(cdc_heap_extract_top(h) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 2);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_heap_top(h);
        CU_ASSERT(cdc_heap_extract_top(h) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 1);
        CU_ASSERT(*((int *)elem) == d);

        elem = cdc_heap_top(h);
        CU_ASSERT(cdc_heap_extract_top(h) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 0);
        CU_ASSERT(*((int *)elem) == a);

        cdc_heap_dtor(h);
}

void test_heap_insert()
{
        struct cdc_heap *h;
        int a = 0, b = 1, c = 2;
        void *elem;

        CU_ASSERT(cdc_heap_ctor(&h, NULL, gt_int) == CDC_STATUS_OK);

        CU_ASSERT(cdc_heap_insert(h, &a) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 1);
        elem = cdc_heap_top(h);
        CU_ASSERT(*((int *)elem) == a);

        CU_ASSERT(cdc_heap_insert(h, &c) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 2);
        elem = cdc_heap_top(h);
        CU_ASSERT(*((int *)elem) == c);

        CU_ASSERT(cdc_heap_insert(h, &b) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_size(h) == 3);
        elem = cdc_heap_top(h);
        CU_ASSERT(*((int *)elem) == c);

        cdc_heap_dtor(h);
}

void test_heap_increase_key()
{

}

void test_heap_swap()
{
        struct cdc_heap *v, *w;
        int a = 2, b = 3, c = 4;

        CU_ASSERT(cdc_heap_ctorl(&v, NULL, gt_int, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_heap_ctorl(&w, NULL, gt_int, &a, &c, NULL) == CDC_STATUS_OK);

        cdc_heap_swap(v, w);

        CU_ASSERT(cdc_heap_size(v) == 2);
        CU_ASSERT(*((int *)cdc_heap_top(v)) == c);
        CU_ASSERT(cdc_heap_size(w) == 1);
        CU_ASSERT(*((int *)cdc_heap_top(w)) == b);

        cdc_heap_dtor(v);
        cdc_heap_dtor(w);
}
