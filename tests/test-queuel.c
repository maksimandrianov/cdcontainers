#include "test-common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

void test_queuel_ctor()
{
        struct cdc_queue *q;

        CU_ASSERT(cdc_queue_ctor(cdc_queuel_table, &q, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 0);

        cdc_queue_dtor(q);
}

void test_queuel_ctorl()
{
        struct cdc_queue *q;
        int a = 2, b = 3;
        void *elem;

        CU_ASSERT(cdc_queue_ctorl(cdc_queuel_table, &q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 2);

        elem = cdc_queue_front(q);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_queue_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 1);

        elem = cdc_queue_front(q);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_queue_pop(q) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_size(q) == 0);
}

void test_queuel_push()
{
        struct cdc_queue *q;
        int a = 0, b = 1, c = 2;
        void *elem;

        CU_ASSERT(cdc_queue_ctor(cdc_queuel_table, &q, NULL) == CDC_STATUS_OK);

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

void test_queuel_pop()
{
        struct cdc_queue *q;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_queue_ctorl(cdc_queuel_table, &q, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

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

void test_queuel_front()
{
        struct cdc_queue *q;
        int a = 1, b = 2;

        CU_ASSERT(cdc_queue_ctorl(cdc_queuel_table, &q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_queue_front(q)) == a);

        cdc_queue_dtor(q);
}

void test_queuel_back()
{
        struct cdc_queue *q;
        int a = 1, b = 2;

        CU_ASSERT(cdc_queue_ctorl(cdc_queuel_table, &q, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_queue_back(q)) == b);

        cdc_queue_dtor(q);
}

void test_queuel_swap()
{
        struct cdc_queue *v, *w;
        int a = 2, b = 3, c = 4;

        CU_ASSERT(cdc_queue_ctorl(cdc_queuel_table, &v, NULL, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_queue_ctorl(cdc_queuel_table, &w, NULL, &a, &c, NULL) == CDC_STATUS_OK);

        cdc_queue_swap(v, w);

        CU_ASSERT(cdc_queue_size(v) == 2);
        CU_ASSERT(*((int *)cdc_queue_back(v)) == c);
        CU_ASSERT(cdc_queue_size(w) == 1);
        CU_ASSERT(*((int *)cdc_queue_back(w)) == b);

        cdc_queue_dtor(v);
        cdc_queue_dtor(w);
}
