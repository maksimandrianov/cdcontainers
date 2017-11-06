#include "test_common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

static void list_int_print(void *val, size_t index)
{
        printf("%u\n", *((int *)val));
}

static bool list_range_int_eq(cdc_list_t *l, size_t count, ...)
{
        va_list args;
        int elem, i;
        void *val;
        enum cdc_stat ret;

        va_start(args, count);
        for (i = 0; i < count; ++i) {
                elem = va_arg(args, int);
                if ((ret = cdc_list_at(l, i, &val)) != CDC_STATUS_OK)
                        return false;

                if (elem != *((int *)val))
                        return false;
        }

        return true;
}

void test_list_ctor()
{
        cdc_list_t *l = NULL;

        CU_ASSERT(cdc_list_ctor(&l, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 0);

        cdc_list_dtor(l);
}

void test_list_ctorl()
{
        cdc_list_t *l = NULL;
        int a = 0, b = 1, c = 2, d =3;
        void *f;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 4);
        CU_ASSERT(list_range_int_eq(l, 4, a, b, c, d));

        cdc_list_dtor(l);
}

void test_list_push_back()
{
        cdc_list_t *l = NULL;
        int a = 0, b = 1, c = 2;

        CU_ASSERT(cdc_list_ctor(&l, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 0);

        CU_ASSERT(cdc_list_push_back(l, &a) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 1);
        CU_ASSERT(list_range_int_eq(l, 1, a));

        CU_ASSERT(cdc_list_push_back(l, &b) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 2);
        CU_ASSERT(list_range_int_eq(l, 2, a, b));

        CU_ASSERT(cdc_list_push_back(l, &c) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 3);
        CU_ASSERT(list_range_int_eq(l, 3, a, b, c));

        cdc_list_dtor(l);
}

void test_list_push_front()
{
        cdc_list_t *l = NULL;
        int a = 0, b = 1, c = 2;

        CU_ASSERT(cdc_list_ctor(&l, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 0);

        CU_ASSERT(cdc_list_push_front(l, &a) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 1);
        CU_ASSERT(list_range_int_eq(l, 1, a));

        CU_ASSERT(cdc_list_push_front(l, &b) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 2);
        CU_ASSERT(list_range_int_eq(l, 2, b, a));

        CU_ASSERT(cdc_list_push_front(l, &c) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 3);
        CU_ASSERT(list_range_int_eq(l, 3, c, b, a));

        cdc_list_dtor(l);
}

void test_list_at()
{
        cdc_list_t *l;
        int a = 0, b = 1, c = 2;
        void *elem = NULL;
        size_t index;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_list_at(l, 0, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == a);
        CU_ASSERT(cdc_list_at(l, 1, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == b);
        CU_ASSERT(cdc_list_at(l, 2, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == c);

        index = cdc_list_size(l) + 1;
        CU_ASSERT(cdc_list_at(l, index, &elem) == CDC_STATUS_OUT_OF_RANGE);

        cdc_list_dtor(l);
}

void test_list_front()
{
        cdc_list_t *l;
        int a = 1, b = 2;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_list_front(l)) == a);

        cdc_list_dtor(l);
}

void test_list_back()
{
        cdc_list_t *l;
        int a = 1, b = 2;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_list_back(l)) == b);

        cdc_list_dtor(l);
}

void test_list_pop_back()
{
        cdc_list_t *l;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_list_back(l);
        CU_ASSERT(cdc_list_pop_back(l) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 3);
        CU_ASSERT(*((int *)elem) == d);

        elem = cdc_list_back(l);
        CU_ASSERT(cdc_list_pop_back(l) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 2);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_list_back(l);
        CU_ASSERT(cdc_list_pop_back(l) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 1);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_list_back(l);
        CU_ASSERT(cdc_list_pop_back(l) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 0);
        CU_ASSERT(*((int *)elem) == a);

        cdc_list_dtor(l);
}

void test_list_pop_front()
{
        cdc_list_t *l;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_list_front(l);
        CU_ASSERT(cdc_list_pop_front(l) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 3);
        CU_ASSERT(*((int *)elem) == a);

        elem = cdc_list_front(l);
        CU_ASSERT(cdc_list_pop_front(l) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 2);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_list_front(l);
        CU_ASSERT(cdc_list_pop_front(l) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 1);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_list_front(l);
        CU_ASSERT(cdc_list_pop_front(l) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 0);
        CU_ASSERT(*((int *)elem) == d);

        cdc_list_dtor(l);
}

void test_list_swap()
{
        cdc_list_t *v, *w;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_list_ctorl(&v, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_ctorl(&w, NULL, &a, &d, NULL) == CDC_STATUS_OK);

        cdc_list_swap(v, w);

        CU_ASSERT(cdc_list_size(v) == 2);
        CU_ASSERT(list_range_int_eq(v, 2, a, d));
        CU_ASSERT(cdc_list_size(w) == 4);
        CU_ASSERT(list_range_int_eq(w, 4, a, b, c, d));

        cdc_list_dtor(v);
        cdc_list_dtor(w);
}

void test_list_insert()
{
        cdc_list_t *l;
        int a = 0, b = 1, c = 2;

        CU_ASSERT(cdc_list_ctor(&l, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_list_insert(l, 0, &a) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 1);
        CU_ASSERT(list_range_int_eq(l, 1, a));

        CU_ASSERT(cdc_list_insert(l, cdc_list_size(l), &c) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 2);
        CU_ASSERT(list_range_int_eq(l, 2, a, c));

        CU_ASSERT(cdc_list_insert(l, 1, &b) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 3);
        CU_ASSERT(list_range_int_eq(l, 3, a, b, c));

        cdc_list_dtor(l);
}

void test_list_erase()
{
        cdc_list_t *l;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_list_erase(l, 0, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_list_size(l) == 3);
        CU_ASSERT(list_range_int_eq(l, 3, b, c, d));

        CU_ASSERT(cdc_list_erase(l, 1, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == c);
        CU_ASSERT(cdc_list_size(l) == 2);
        CU_ASSERT(list_range_int_eq(l, 2, b, d));

        CU_ASSERT(cdc_list_erase(l, cdc_list_size(l) - 1, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == d);
        CU_ASSERT(cdc_list_size(l) == 1);
        CU_ASSERT(list_range_int_eq(l, 1, b));

        cdc_list_dtor(l);
}

void test_list_clear()
{
        cdc_list_t *l;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        cdc_list_clear(l);

        CU_ASSERT(cdc_list_size(l) == 0);

        cdc_list_dtor(l);
}
