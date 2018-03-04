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
#include "cdcontainers/queue.h"

void test_queued_ctor()
{
        struct cdc_queue *q;

        CU_ASSERT(cdc_queued_ctor(&q, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 0);

        cdc_queue_dtor(q);
}

void test_queued_ctorl()
{
        struct cdc_queue *q;
        int a = 2, b = 3;
        void *elem;

        CU_ASSERT(cdc_queued_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 2);

        elem = cdc_queue_front(q);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_queue_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 1);

        elem = cdc_queue_front(q);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_queue_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 0);

        cdc_queue_dtor(q);
}

void test_queued_push()
{
        struct cdc_queue *q;
        int a = 0, b = 1, c = 2;
        void *elem;

        CU_ASSERT(cdc_queued_ctor(&q, NULL) == CDC_STATUS_OK);

        cdc_queue_push(q, &a);
        CU_ASSERT(cdc_queue_size(q) == 1);
        elem = cdc_queue_back(q);
        CU_ASSERT(*((int *)elem) == a);

        cdc_queue_push(q, &b);
        CU_ASSERT(cdc_queue_size(q) == 2);
        elem = cdc_queue_back(q);
        CU_ASSERT(*((int *)elem) == b);

        cdc_queue_push(q, &c);
        CU_ASSERT(cdc_queue_size(q) == 3);
        elem = cdc_queue_back(q);
        CU_ASSERT(*((int *)elem) == c);

        cdc_queue_dtor(q);
}

void test_queued_pop()
{
        struct cdc_queue *q;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_queued_ctorl(&q, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_queue_front(q);
        CU_ASSERT(cdc_queue_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 3);
        CU_ASSERT(*((int *)elem) == a);

        elem = cdc_queue_front(q);
        CU_ASSERT(cdc_queue_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 2);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_queue_front(q);
        CU_ASSERT(cdc_queue_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 1);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_queue_front(q);
        CU_ASSERT(cdc_queue_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 0);
        CU_ASSERT(*((int *)elem) == d);

        cdc_queue_dtor(q);
}

void test_queued_front()
{
        struct cdc_queue *q;
        int a = 1, b = 2;

        CU_ASSERT(cdc_queued_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_queue_front(q)) == a);

        cdc_queue_dtor(q);
}

void test_queued_back()
{
        struct cdc_queue *q;
        int a = 1, b = 2;

        CU_ASSERT(cdc_queued_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_queue_back(q)) == b);

        cdc_queue_dtor(q);
}

void test_queued_swap()
{
        struct cdc_queue *v, *w;
        int a = 2, b = 3, c = 4;

        CU_ASSERT(cdc_queued_ctorl(&v, NULL, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_ctorl(&w, NULL, &a, &c, NULL) == CDC_STATUS_OK);

        cdc_queue_swap(v, w);

        CU_ASSERT(cdc_queue_size(v) == 2);
        CU_ASSERT(*((int *)cdc_queue_back(v)) == c);
        CU_ASSERT(cdc_queue_size(w) == 1);
        CU_ASSERT(*((int *)cdc_queue_back(w)) == b);

        cdc_queue_dtor(v);
        cdc_queue_dtor(w);
}
