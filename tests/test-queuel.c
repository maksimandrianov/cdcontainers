#include "test-common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

void test_queuel_ctor()
{
        cdc_queuel_t *q;

        CU_ASSERT(cdc_queuel_ctor(&q, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_size(q) == 0);

        cdc_queuel_dtor(q);
}

void test_queuel_ctorl()
{
        cdc_queuel_t *q;
        int a = 2, b = 3;
        void *elem;

        CU_ASSERT(cdc_queuel_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_size(q) == 2);

        elem = cdc_queuel_front(q);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_queuel_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_size(q) == 1);

        elem = cdc_queuel_front(q);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_queuel_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_size(q) == 0);
}

void test_queuel_push()
{
        cdc_queuel_t *q;
        int a = 0, b = 1, c = 2;
        void *elem;

        CU_ASSERT(cdc_queuel_ctor(&q, NULL) == CDC_STATUS_OK);

        cdc_queuel_push(q, &a);
        CU_ASSERT(cdc_queuel_size(q) == 1);
        elem = cdc_queuel_back(q);
        CU_ASSERT(*((int *)elem) == a);

        cdc_queuel_push(q, &b);
        CU_ASSERT(cdc_queuel_size(q) == 2);
        elem = cdc_queuel_back(q);
        CU_ASSERT(*((int *)elem) == b);

        cdc_queuel_push(q, &c);
        CU_ASSERT(cdc_queuel_size(q) == 3);
        elem = cdc_queuel_back(q);
        CU_ASSERT(*((int *)elem) == c);

        cdc_queuel_dtor(q);
}

void test_queuel_pop()
{
        cdc_queuel_t *q;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_queuel_ctorl(&q, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_queuel_front(q);
        CU_ASSERT(cdc_queuel_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_size(q) == 3);
        CU_ASSERT(*((int *)elem) == a);

        elem = cdc_queuel_front(q);
        CU_ASSERT(cdc_queuel_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_size(q) == 2);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_queuel_front(q);
        CU_ASSERT(cdc_queuel_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_size(q) == 1);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_queuel_front(q);
        CU_ASSERT(cdc_queuel_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_size(q) == 0);
        CU_ASSERT(*((int *)elem) == d);

        cdc_queuel_dtor(q);
}

void test_queuel_front()
{
        cdc_queuel_t *q;
        int a = 1, b = 2;

        CU_ASSERT(cdc_queuel_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_queuel_front(q)) == a);

        cdc_queuel_dtor(q);
}

void test_queuel_back()
{
        cdc_queuel_t *q;
        int a = 1, b = 2;

        CU_ASSERT(cdc_queuel_ctorl(&q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_queuel_back(q)) == b);

        cdc_queuel_dtor(q);
}

void test_queuel_swap()
{
        cdc_queuel_t *v, *w;
        int a = 2, b = 3, c = 4;

        CU_ASSERT(cdc_queuel_ctorl(&v, NULL, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queuel_ctorl(&w, NULL, &a, &c, NULL) == CDC_STATUS_OK);

        cdc_queuel_swap(v, w);

        CU_ASSERT(cdc_queuel_size(v) == 2);
        CU_ASSERT(*((int *)cdc_queuel_back(v)) == c);
        CU_ASSERT(cdc_queuel_size(w) == 1);
        CU_ASSERT(*((int *)cdc_queuel_back(w)) == b);

        cdc_queuel_dtor(v);
        cdc_queuel_dtor(w);
}
