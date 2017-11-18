#include "test-common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

void test_stackl_ctor()
{
        struct cdc_stack *s;

        CU_ASSERT(cdc_stack_ctor(cdc_stackl_table, &s, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stack_size(s) == 0);

        cdc_stack_dtor(s);
}

void test_stackl_ctorl()
{
        struct cdc_stack *s = NULL;
        int a = 2, b = 3;
        void *elem;

        CU_ASSERT(cdc_stack_ctorl(cdc_stackl_table, &s, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stack_size(s) == 2);

        elem = cdc_stack_top(s);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_stack_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stack_size(s) == 1);

        elem = cdc_stack_top(s);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_stack_pop(s) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stack_size(s) == 0);

}

void test_stackl_push()
{
        struct cdc_stack *s;
        int a = 0, b = 1, c = 2;
        void *elem;

        CU_ASSERT(cdc_stack_ctor(cdc_stackl_table, &s, NULL) == CDC_STATUS_OK);

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

void test_stackl_pop()
{
        struct cdc_stack *s;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_stack_ctorl(cdc_stackl_table, &s, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

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

void test_stackl_swap()
{
        struct cdc_stack *v, *w;
        int a = 2, b = 3, c = 4;

        CU_ASSERT(cdc_stack_ctorl(cdc_stackl_table, &v, NULL, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_stack_ctorl(cdc_stackl_table, &w, NULL, &a, &c, NULL) == CDC_STATUS_OK);

        cdc_stack_swap(v, w);

        CU_ASSERT(cdc_stack_size(v) == 2);
        CU_ASSERT(*((int *)cdc_stack_top(v)) == c);
        CU_ASSERT(cdc_stack_size(w) == 1);
        CU_ASSERT(*((int *)cdc_stack_top(w)) == b);

        cdc_stack_dtor(v);
        cdc_stack_dtor(w);
}
