#include "test_common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

static bool vector_range_int_eq(cdc_vector_t *v, size_t count, ...)
{
        va_list args;
        int elem;
        int i;

        va_start(args, count);
        for (i = 0; i < count; ++i) {
                elem = va_arg(args, int);
                if (elem != *((int *)cdc_vector_get(v, i)))
                        return false;
        }

        return true;
}

void test_vector_ctor()
{
        cdc_vector_t *v;

        CU_ASSERT(cdc_vector_ctor(&v, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == 0);
        CU_ASSERT(cdc_vector_capacity(v) == CDC_VECTOR_MIN_CAPACITY);
        CU_ASSERT_DOUBLE_EQUAL(cdc_vector_cap_exp(v), CDC_VECTOR_COPACITY_EXP,
                               FLT_EPSILON);

        cdc_vector_dtor(v);
}

void test_vector_ctor_list()
{
        cdc_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;
        size_t count_elements = 4;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == count_elements);
        CU_ASSERT(cdc_vector_capacity(v) == CDC_VECTOR_MIN_CAPACITY);
        CU_ASSERT_DOUBLE_EQUAL(cdc_vector_cap_exp(v), CDC_VECTOR_COPACITY_EXP,
                               FLT_EPSILON);
        CU_ASSERT(vector_range_int_eq(v, count_elements, a, b, c, d));

        cdc_vector_dtor(v);
}

void test_vector_reserve()
{
        cdc_vector_t *v;
        size_t count_reserved= 10;

        CU_ASSERT(cdc_vector_ctor(&v, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_reserve(v, count_reserved) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_capacity(v) == count_reserved);

        CU_ASSERT(cdc_vector_reserve(v, count_reserved - 1) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_capacity(v) == count_reserved);

        cdc_vector_dtor(v);
}

void test_vector_data()
{
        cdc_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)(cdc_vector_data(v)[0])) == a);
        CU_ASSERT(*((int *)(cdc_vector_data(v)[1])) == b);
        CU_ASSERT(*((int *)(cdc_vector_data(v)[2])) == c);
        CU_ASSERT(*((int *)(cdc_vector_data(v)[3])) == d);

        cdc_vector_dtor(v);
}

void test_vector_get()
{
        cdc_vector_t *v;
        int a = 0, b = 1;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_vector_get(v, 0)) == a);
        CU_ASSERT(*((int *)cdc_vector_get(v, 1)) == b);

        cdc_vector_dtor(v);
}

void test_vector_at()
{
        cdc_vector_t *v;
        int a = 0, b = 1, c = 2;
        void *elem = NULL;
        size_t index;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, &c, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_vector_at(v, 0, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == a);
        CU_ASSERT(cdc_vector_at(v, 1, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == b);
        CU_ASSERT(cdc_vector_at(v, 2, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*(int *)elem == c);

        index = cdc_vector_size(v) + 1;
        CU_ASSERT(cdc_vector_at(v, index, &elem) == CDC_STATUS_OUT_OF_RANGE);

        cdc_vector_dtor(v);
}

void test_vector_front()
{
        cdc_vector_t *v;
        int a = 1, b = 2;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_vector_front(v)) == a);

        cdc_vector_dtor(v);
}

void test_vector_back()
{
        cdc_vector_t *v;
        int a = 1, b = 2;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_vector_back(v)) == b);

        cdc_vector_dtor(v);
}

void test_vector_insert()
{
        cdc_vector_t *v;
        int a = 1, b = 2;
        int i1 = 3, i2 = 4, i3 = 5;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_vector_insert(v, 0, &i1) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == 3);
        CU_ASSERT(vector_range_int_eq(v, 3, i1, a, b));

        CU_ASSERT(cdc_vector_insert(v, cdc_vector_size(v), &i2) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == 4);
        CU_ASSERT(vector_range_int_eq(v, 4, i1, a, b, i2));

        CU_ASSERT(cdc_vector_insert(v, 2, &i3) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == 5);
        CU_ASSERT(vector_range_int_eq(v, 5, i1, a, i3, b, i2));

        cdc_vector_dtor(v);
}

void test_vector_erase()
{
        cdc_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem = NULL;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        CU_ASSERT(cdc_vector_erase(v, 2, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == c);
        CU_ASSERT(cdc_vector_size(v) == 3);
        CU_ASSERT(vector_range_int_eq(v, 3, a, b, d));

        CU_ASSERT(cdc_vector_erase(v, 0, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cdc_vector_size(v) == 2);
        CU_ASSERT(vector_range_int_eq(v, 2, b, d));

        CU_ASSERT(cdc_vector_erase(v, cdc_vector_size(v) - 1, &elem) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)elem) == d);
        CU_ASSERT(cdc_vector_size(v) == 1);
        CU_ASSERT(vector_range_int_eq(v, 1, b));

        cdc_vector_dtor(v);
}

void test_vector_clear()
{
        cdc_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        cdc_vector_clear(v);

        CU_ASSERT(cdc_vector_size(v) == 0);

        cdc_vector_dtor(v);
}

void test_vector_push_back()
{
        cdc_vector_t *v;
        int a = 0, b = 1, c = 2;

        CU_ASSERT(cdc_vector_ctor(&v, NULL) == CDC_STATUS_OK);

        cdc_vector_push_back(v, &a);
        CU_ASSERT(cdc_vector_size(v) == 1);
        CU_ASSERT(vector_range_int_eq(v, 1, a));

        cdc_vector_push_back(v, &b);
        CU_ASSERT(cdc_vector_size(v) == 2);
        CU_ASSERT(vector_range_int_eq(v, 1, a, b));

        cdc_vector_push_back(v, &c);
        CU_ASSERT(cdc_vector_size(v) == 3);
        CU_ASSERT(vector_range_int_eq(v, 3, a, b, c));

        cdc_vector_dtor(v);
}

void test_vector_pop_back()
{
        cdc_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        elem = cdc_vector_back(v);
        CU_ASSERT(cdc_vector_pop_back(v) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == 3);
        CU_ASSERT(*((int *)elem) == d);

        elem = cdc_vector_back(v);
        CU_ASSERT(cdc_vector_pop_back(v) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == 2);
        CU_ASSERT(*((int *)elem) == c);

        elem = cdc_vector_back(v);
        CU_ASSERT(cdc_vector_pop_back(v) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == 1);
        CU_ASSERT(*((int *)elem) == b);

        elem = cdc_vector_back(v);
        CU_ASSERT(cdc_vector_pop_back(v) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_size(v) == 0);
        CU_ASSERT(*((int *)elem) == a);

        cdc_vector_dtor(v);
}

void test_vector_swap()
{
        cdc_vector_t *v, *w;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_vector_ctorl(&v, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_vector_ctorl(&w, NULL, &a, &d, NULL) == CDC_STATUS_OK);

        cdc_vector_swap(v, w);

        CU_ASSERT(cdc_vector_size(v) == 2);
        CU_ASSERT(vector_range_int_eq(v, 2, a, d));
        CU_ASSERT(cdc_vector_size(w) == 4);
        CU_ASSERT(vector_range_int_eq(w, 4, a, b, c, d));

        cdc_vector_dtor(v);
        cdc_vector_dtor(w);
}

