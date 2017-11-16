#include "test-common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

void test_stackl_ctor()
{
        cdc_stackl_t *s;

        CU_ASSERT(cdc_stackl_ctor(&s, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_size(s) == 0);

        cdc_stackl_dtor(s);
}

void test_stackl_ctorl()
{
        cdc_stackl_t *s;
        int a = 2, b = 3;
        void *elem;

        CU_ASSERT(cdc_stackl_ctorl(&s, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_size(s) == 2);

        elem = cdc_stackl_top(s);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_stackl_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_size(s) == 1);

        elem = cdc_stackl_top(s);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_stackl_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_size(s) == 0);

}

void test_stackl_push()
{
        cdc_stackl_t *s;
        int a = 0, b = 1, c = 2;
        void *elem;

        CU_ASSERT(cdc_stackl_ctor(&s, NULL) == CDC_STATUS_OK);

        cdc_stackl_push(s, &a);
        CU_ASSERT(cdc_stackl_size(s) == 1);
        elem = cdc_stackl_top(s);
        CU_ASSERT(*((int *)elem) == a);

        cdc_stackl_push(s, &b);
        CU_ASSERT(cdc_stackl_size(s) == 2);
        elem = cdc_stackl_top(s);
        CU_ASSERT(*((int *)elem) == b);

        cdc_stackl_push(s, &c);
        CU_ASSERT(cdc_stackl_size(s) == 3);
        elem = cdc_stackl_top(s);
        CU_ASSERT(*((int *)elem) == c);

        cdc_stackl_dtor(s);
}

void test_stackl_pop()
{
        cdc_stackl_t *s;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_stackl_ctorl(&s, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_stackl_top(s);
        CU_ASSERT(cdc_stackl_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_size(s) == 3);
        CU_ASSERT(*((int *)elem) == d);

        elem = cdc_stackl_top(s);
        CU_ASSERT(cdc_stackl_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_size(s) == 2);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_stackl_top(s);
        CU_ASSERT(cdc_stackl_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_size(s) == 1);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_stackl_top(s);
        CU_ASSERT(cdc_stackl_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_size(s) == 0);
        CU_ASSERT(*((int *)elem) == a);

        cdc_stackl_dtor(s);
}

void test_stackl_swap()
{
        cdc_stackl_t *v, *w;
        int a = 2, b = 3, c = 4;

        CU_ASSERT(cdc_stackl_ctorl(&v, NULL, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stackl_ctorl(&w, NULL, &a, &c, NULL) == CDC_STATUS_OK);

        cdc_stackl_swap(v, w);

        CU_ASSERT(cdc_stackl_size(v) == 2);
        CU_ASSERT(*((int *)cdc_stackl_top(v)) == c);
        CU_ASSERT(cdc_stackl_size(w) == 1);
        CU_ASSERT(*((int *)cdc_stackl_top(w)) == b);

        cdc_stackl_dtor(v);
        cdc_stackl_dtor(w);
}
