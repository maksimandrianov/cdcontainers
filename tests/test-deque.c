#include "test-common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>
#include <assert.h>

static void deque_int_print(cdc_deque_t *d)
{
        int i;

        printf("\n");
        for (i = 0; i < cdc_deque_size(d); ++i)
                printf("%u\n", *((int *)cdc_deque_get(d, i)));
}

static bool deque_range_int_eq(cdc_deque_t *d, size_t count, ...)
{
        assert(cdc_deque_size(d) >= count);

        va_list args;
        int elem, i;
        void *val;
        enum cdc_stat ret;

        va_start(args, count);
        for (i = 0; i < count; ++i) {
                elem = va_arg(args, int);
                if ((ret = cdc_deque_at(d, i, &val)) != CDC_STATUS_OK)
                        return false;

                if (elem != *((int *)val))
                        return false;
        }

        return true;
}

void test_deque_ctor()
{
        cdc_deque_t *d = NULL;

        CU_ASSERT(cdc_deque_ctor(&d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 0);

        cdc_deque_dtor(d);
}

void test_deque_ctorl()
{
        cdc_deque_t *deq = NULL;
        int a = 0, b = 1, c = 2, d =3;
        void *f;

        CU_ASSERT(cdc_deque_ctorl(&deq, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 4);
        CU_ASSERT(deque_range_int_eq(deq, 4, a, b, c, d));

        cdc_deque_dtor(deq);
}

void test_deque_push_back()
{
        cdc_deque_t *d;
        int a = 0, b = 1, c = 2;

        CU_ASSERT(cdc_deque_ctor(&d, NULL) == CDC_STATUS_OK);

        cdc_deque_push_back(d, &a);
        CU_ASSERT(cdc_deque_size(d) == 1);
        CU_ASSERT(deque_range_int_eq(d, 1, a));

        cdc_deque_push_back(d, &b);
        CU_ASSERT(cdc_deque_size(d) == 2);
        CU_ASSERT(deque_range_int_eq(d, 1, a, b));

        cdc_deque_push_back(d, &c);
        CU_ASSERT(cdc_deque_size(d) == 3);
        CU_ASSERT(deque_range_int_eq(d, 3, a, b, c));

        cdc_deque_push_back(d, &a);
        CU_ASSERT(cdc_deque_size(d) == 4);
        CU_ASSERT(deque_range_int_eq(d, 4, a, b, c, a));

        cdc_deque_push_back(d, &b);
        CU_ASSERT(cdc_deque_size(d) == 5);
        CU_ASSERT(deque_range_int_eq(d, 5, a, b, c, a, b));

        cdc_deque_dtor(d);
}

void test_deque_push_front()
{
        cdc_deque_t *d = NULL;
        int a = 0, b = 1, c = 2;

        CU_ASSERT(cdc_deque_ctor(&d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 0);

        CU_ASSERT(cdc_deque_push_front(d, &a) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 1);
        CU_ASSERT(deque_range_int_eq(d, 1, a));

        CU_ASSERT(cdc_deque_push_front(d, &b) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 2);
        CU_ASSERT(deque_range_int_eq(d, 2, b, a));

        CU_ASSERT(cdc_deque_push_front(d, &c) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 3);
        CU_ASSERT(deque_range_int_eq(d, 3, c, b, a));

        CU_ASSERT(cdc_deque_push_front(d, &a) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 4);
        CU_ASSERT(deque_range_int_eq(d, 4, a, c, b, a));

        CU_ASSERT(cdc_deque_push_front(d, &b) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 5);
        CU_ASSERT(deque_range_int_eq(d, 4, b, a, c, b, a));

        cdc_deque_dtor(d);
}

void test_deque_get()
{
        cdc_deque_t *d;
        int a = 0, b = 1;

        CU_ASSERT(cdc_deque_ctorl(&d, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_deque_get(d, 0)) == a);
        CU_ASSERT(*((int *)cdc_deque_get(d, 1)) == b);

        cdc_deque_dtor(d);
}

void test_deque_at()
{
        cdc_deque_t *d;
        int a = 0, b = 1, c = 2;
        void *elem = NULL;
        size_t index;

        CU_ASSERT(cdc_deque_ctorl(&d, NULL, &a, &b, &c, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_deque_at(d, 0, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == a);
        CU_ASSERT(cdc_deque_at(d, 1, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == b);
        CU_ASSERT(cdc_deque_at(d, 2, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == c);

        index = cdc_deque_size(d) + 1;
        CU_ASSERT(cdc_deque_at(d, index, &elem) == CDC_STATUS_OUT_OF_RANGE);

        cdc_deque_dtor(d);
}

void test_deque_set()
{
        cdc_deque_t *d;
        int a = 1, b = 2;

        CU_ASSERT(cdc_deque_ctorl(&d, NULL, &a, &b, NULL) == CDC_STATUS_OK);

        cdc_deque_set(d, 0, &b);
        CU_ASSERT(deque_range_int_eq(d, 2, b, b));

        cdc_deque_set(d, 1, &a);
        CU_ASSERT(deque_range_int_eq(d, 2, b, a));

        cdc_deque_dtor(d);
}

void test_deque_front()
{
        cdc_deque_t *d;
        int a = 1, b = 2;

        CU_ASSERT(cdc_deque_ctorl(&d, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_deque_front(d)) == a);

        CU_ASSERT(cdc_deque_push_front(d, &b) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_deque_front(d)) == b);

        cdc_deque_dtor(d);
}

void test_deque_back()
{
        cdc_deque_t *d;
        int a = 1, b = 2;

        CU_ASSERT(cdc_deque_ctorl(&d, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_deque_back(d)) == b);

        CU_ASSERT(cdc_deque_push_back(d, &a) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_deque_back(d)) == a);

        cdc_deque_dtor(d);
}

void test_deque_pop_back()
{
        cdc_deque_t *deq;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_deque_ctorl(&deq, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_deque_back(deq);
        CU_ASSERT(cdc_deque_pop_back(deq) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 3);
        CU_ASSERT(*((int *)elem) == d);

        elem = cdc_deque_back(deq);
        CU_ASSERT(cdc_deque_pop_back(deq) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 2);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_deque_back(deq);
        CU_ASSERT(cdc_deque_pop_back(deq) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 1);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_deque_back(deq);
        CU_ASSERT(cdc_deque_pop_back(deq) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 0);
        CU_ASSERT(*((int *)elem) == a);

        cdc_deque_dtor(deq);
}

void test_deque_pop_front()
{
        cdc_deque_t *deq;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_deque_ctorl(&deq, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_deque_front(deq);
        CU_ASSERT(cdc_deque_pop_front(deq) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 3);
        CU_ASSERT(*((int *)elem) == a);

        elem = cdc_deque_front(deq);
        CU_ASSERT(cdc_deque_pop_front(deq) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 2);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_deque_front(deq);
        CU_ASSERT(cdc_deque_pop_front(deq) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 1);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_deque_front(deq);
        CU_ASSERT(cdc_deque_pop_front(deq) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(deq) == 0);
        CU_ASSERT(*((int *)elem) == d);

        cdc_deque_dtor(deq);
}

void test_deque_swap()
{
        cdc_deque_t *v, *w;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_deque_ctorl(&v, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_ctorl(&w, NULL, &a, &d, NULL) == CDC_STATUS_OK);

        cdc_deque_swap(v, w);

        CU_ASSERT(cdc_deque_size(v) == 2);
        CU_ASSERT(deque_range_int_eq(v, 2, a, d));
        CU_ASSERT(cdc_deque_size(w) == 4);
        CU_ASSERT(deque_range_int_eq(w, 4, a, b, c, d));

        cdc_deque_dtor(v);
        cdc_deque_dtor(w);
}

void test_deque_insert()
{
        cdc_deque_t *d;
        int a = 1, b = 2;
        int i1 = 3, i2 = 4, i3 = 5;

        CU_ASSERT(cdc_deque_ctorl(&d, NULL, &a, &b, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_deque_insert(d, 0, &i1) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 3);
        CU_ASSERT(deque_range_int_eq(d, 3, i1, a, b));

        CU_ASSERT(cdc_deque_insert(d, cdc_deque_size(d), &i2) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 4);
        CU_ASSERT(deque_range_int_eq(d, 4, i1, a, b, i2));

        CU_ASSERT(cdc_deque_insert(d, 2, &i3) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 5);
        CU_ASSERT(deque_range_int_eq(d, 5, i1, a, i3, b, i2));

        cdc_deque_dtor(d);
}

void test_deque_erase()
{
        cdc_deque_t *deq;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem = NULL;

        CU_ASSERT(cdc_deque_ctorl(&deq, NULL, &a, &b, &c, &d, &a, &b, &c, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_deque_erase(deq, 2, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == c);
        CU_ASSERT(cdc_deque_size(deq) == 6);
        CU_ASSERT(deque_range_int_eq(deq, 3, a, b, d, a, b, c));

        CU_ASSERT(cdc_deque_erase(deq, 4, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == b);
        CU_ASSERT(cdc_deque_size(deq) == 5);
        CU_ASSERT(deque_range_int_eq(deq, 5, a, b, d, a, c));


        CU_ASSERT(cdc_deque_erase(deq, 0, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_deque_size(deq) == 4);
        CU_ASSERT(deque_range_int_eq(deq, 4, b, d, a, c));

        CU_ASSERT(cdc_deque_erase(deq, cdc_deque_size(deq) - 1, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == c);
        CU_ASSERT(cdc_deque_size(deq) == 3);
        CU_ASSERT(deque_range_int_eq(deq, 3, b, d, a));

        cdc_deque_dtor(deq);
}

void test_deque_clear()
{
        cdc_deque_t *deq;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_deque_ctorl(&deq, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        cdc_deque_clear(deq);

        CU_ASSERT(cdc_deque_size(deq) == 0);

        cdc_deque_dtor(deq);
}

void test_deque_pop_push()
{
        cdc_deque_t *d;
        int i;
        int a = 1, b = 2, c = 3;
        int i1 = 3, i2 = 4, i3 = 5;

        CU_ASSERT(cdc_deque_ctorl(&d, NULL, &a, &b, &c, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 3);

        for (i = 0; i < 5; ++i) {
                CU_ASSERT(cdc_deque_push_front(d, &a) == CDC_STATUS_OK);
                CU_ASSERT(cdc_deque_pop_back(d) == CDC_STATUS_OK);
                CU_ASSERT(cdc_deque_push_front(d, &b) == CDC_STATUS_OK);
                CU_ASSERT(cdc_deque_pop_back(d) == CDC_STATUS_OK)
        }

        CU_ASSERT(deque_range_int_eq(d, 3, b, a, b));
        CU_ASSERT(cdc_deque_size(d) == 3);

        for (i = 0; i < 5; ++i) {
                CU_ASSERT(cdc_deque_push_back(d, &a) == CDC_STATUS_OK);
                CU_ASSERT(cdc_deque_pop_front(d) == CDC_STATUS_OK);
                CU_ASSERT(cdc_deque_push_back(d, &b) == CDC_STATUS_OK);
                CU_ASSERT(cdc_deque_pop_front(d) == CDC_STATUS_OK);
        }
        CU_ASSERT(deque_range_int_eq(d, 3, b, a, b));
        CU_ASSERT(cdc_deque_size(d) == 3);

        CU_ASSERT(cdc_deque_insert(d, 0, &i1) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 4);
        CU_ASSERT(deque_range_int_eq(d, 4, i1, b, a, b));

        CU_ASSERT(cdc_deque_insert(d, cdc_deque_size(d), &i2) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 5);
        CU_ASSERT(deque_range_int_eq(d, 5, i1, b, a, b, i2));

        CU_ASSERT(cdc_deque_insert(d, 2, &i3) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 6);
        CU_ASSERT(deque_range_int_eq(d, 6, i1, b, i3, a, b, i2));

        CU_ASSERT(cdc_deque_insert(d, 4, &i1) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 7);
        CU_ASSERT(deque_range_int_eq(d, 7, i1, b, i3, a, i1, b, i2));

        CU_ASSERT(cdc_deque_insert(d, 1, &i2) == CDC_STATUS_OK);
        CU_ASSERT(cdc_deque_size(d) == 8);
        CU_ASSERT(deque_range_int_eq(d, 8, i1, i2, b, i3, a, i1, b, i2));
}
