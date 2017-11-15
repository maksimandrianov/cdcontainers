#include "test_common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

void test_stackv_ctor()
{
        cdc_stackv_t *s;

        CU_ASSERT(cdc_stackv_ctor(&s, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_size(s) == 0);

        cdc_stackv_dtor(s);
}

void test_stackv_ctorl()
{
        cdc_stackv_t *s;
        int a = 2, b = 3;
        void *elem;

        CU_ASSERT(cdc_stackv_ctorl(&s, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_size(s) == 2);

        elem = cdc_stackv_top(s);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_stackv_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_size(s) == 1);

        elem = cdc_stackv_top(s);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_stackv_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_size(s) == 0);

}

void test_stackv_push()
{
        cdc_stackv_t *s;
        int a = 0, b = 1, c = 2;
        void *elem;

        CU_ASSERT(cdc_stackv_ctor(&s, NULL) == CDC_STATUS_OK);

        cdc_stackv_push(s, &a);
        CU_ASSERT(cdc_stackv_size(s) == 1);
        elem = cdc_stackv_top(s);
        CU_ASSERT(*((int *)elem) == a);

        cdc_stackv_push(s, &b);
        CU_ASSERT(cdc_stackv_size(s) == 2);
        elem = cdc_stackv_top(s);
        CU_ASSERT(*((int *)elem) == b);

        cdc_stackv_push(s, &c);
        CU_ASSERT(cdc_stackv_size(s) == 3);
        elem = cdc_stackv_top(s);
        CU_ASSERT(*((int *)elem) == c);

        cdc_stackv_dtor(s);
}

void test_stackv_pop()
{
        cdc_stackv_t *s;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_stackv_ctorl(&s, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_stackv_top(s);
        CU_ASSERT(cdc_stackv_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_size(s) == 3);
        CU_ASSERT(*((int *)elem) == d);

        elem = cdc_stackv_top(s);
        CU_ASSERT(cdc_stackv_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_size(s) == 2);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_stackv_top(s);
        CU_ASSERT(cdc_stackv_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_size(s) == 1);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_stackv_top(s);
        CU_ASSERT(cdc_stackv_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_size(s) == 0);
        CU_ASSERT(*((int *)elem) == a);

        cdc_stackv_dtor(s);
}

void test_stackv_swap()
{
        cdc_stackv_t *v, *w;
        int a = 2, b = 3, c = 4;

        CU_ASSERT(cdc_stackv_ctorl(&v, NULL, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackv_ctorl(&w, NULL, &a, &c, NULL) == CDC_STATUS_OK);

        cdc_stackv_swap(v, w);

        CU_ASSERT(cdc_stackv_size(v) == 2);
        CU_ASSERT(*((int *)cdc_stackv_top(v)) == c);
        CU_ASSERT(cdc_stackv_size(w) == 1);
        CU_ASSERT(*((int *)cdc_stackv_top(w)) == b);

        cdc_stackv_dtor(v);
        cdc_stackv_dtor(w);
}
