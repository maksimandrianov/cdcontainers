#include "test_common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

void test_queued_ctor()
{
        cdc_queued_t *q;

        CU_ASSERT(cdc_queued_ctor(&q, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_size(q) == 0);

        cdc_queued_dtor(q);
}

void test_queued_ctorl()
{
        cdc_queued_t *q;
        int a = 2, b = 3;
        void *elem;

        CU_ASSERT(cdc_queued_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_size(q) == 2);

        elem = cdc_queued_front(q);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_queued_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_size(q) == 1);

        elem = cdc_queued_front(q);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_queued_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_size(q) == 0);
}

void test_queued_push()
{
        cdc_queued_t *q;
        int a = 0, b = 1, c = 2;
        void *elem;

        CU_ASSERT(cdc_queued_ctor(&q, NULL) == CDC_STATUS_OK);

        cdc_queued_push(q, &a);
        CU_ASSERT(cdc_queued_size(q) == 1);
        elem = cdc_queued_back(q);
        CU_ASSERT(*((int *)elem) == a);

        cdc_queued_push(q, &b);
        CU_ASSERT(cdc_queued_size(q) == 2);
        elem = cdc_queued_back(q);
        CU_ASSERT(*((int *)elem) == b);

        cdc_queued_push(q, &c);
        CU_ASSERT(cdc_queued_size(q) == 3);
        elem = cdc_queued_back(q);
        CU_ASSERT(*((int *)elem) == c);

        cdc_queued_dtor(q);
}

void test_queued_pop()
{
        cdc_queued_t *q;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_queued_ctorl(&q, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_queued_front(q);
        CU_ASSERT(cdc_queued_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_size(q) == 3);
        CU_ASSERT(*((int *)elem) == a);

        elem = cdc_queued_front(q);
        CU_ASSERT(cdc_queued_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_size(q) == 2);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_queued_front(q);
        CU_ASSERT(cdc_queued_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_size(q) == 1);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_queued_front(q);
        CU_ASSERT(cdc_queued_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_size(q) == 0);
        CU_ASSERT(*((int *)elem) == d);

        cdc_queued_dtor(q);
}

void test_queued_front()
{
        cdc_queued_t *q;
        int a = 1, b = 2;

        CU_ASSERT(cdc_queued_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_queued_front(q)) == a);

        cdc_queued_dtor(q);
}

void test_queued_back()
{
        cdc_queued_t *q;
        int a = 1, b = 2;

        CU_ASSERT(cdc_queued_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_queued_back(q)) == b);

        cdc_queued_dtor(q);
}

void test_queued_swap()
{
        cdc_queued_t *v, *w;
        int a = 2, b = 3, c = 4;

        CU_ASSERT(cdc_queued_ctorl(&v, NULL, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queued_ctorl(&w, NULL, &a, &c, NULL) == CDC_STATUS_OK);

        cdc_queued_swap(v, w);

        CU_ASSERT(cdc_queued_size(v) == 2);
        CU_ASSERT(*((int *)cdc_queued_back(v)) == c);
        CU_ASSERT(cdc_queued_size(w) == 1);
        CU_ASSERT(*((int *)cdc_queued_back(w)) == b);

        cdc_queued_dtor(v);
        cdc_queued_dtor(w);
}
