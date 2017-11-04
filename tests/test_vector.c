#include "test_common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>
#include "cdstructures.h"

static bool vector_range_int_eq(cds_vector_t *v, size_t count, ...)
{
        va_list args;
        int elem;
        int i;

        va_start(args, count);
        for (i = 0; i < count; ++i) {
                elem = va_arg(args, int);
                if (elem != *((int *)cds_vector_get(v, i)))
                        return false;
        }

        return true;
}

void test_vector_ctor()
{
        cds_vector_t *v;

        CU_ASSERT(cds_vector_ctor(&v, NULL) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_size(v) == 0);
        CU_ASSERT(cds_vector_capacity(v) == CDS_VECTOR_MIN_CAPACITY);
        CU_ASSERT_DOUBLE_EQUAL(cds_vector_cap_exp(v), CDS_VECTOR_COPACITY_EXP,
                               FLT_EPSILON);

        cds_vector_dtor(v);
}

void test_vector_ctor_list()
{
        cds_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;
        size_t count_elements = 4;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, &c, &d, NULL) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_size(v) == count_elements);
        CU_ASSERT(cds_vector_capacity(v) == CDS_VECTOR_MIN_CAPACITY);
        CU_ASSERT_DOUBLE_EQUAL(cds_vector_cap_exp(v), CDS_VECTOR_COPACITY_EXP,
                               FLT_EPSILON);
        CU_ASSERT(vector_range_int_eq(v, count_elements, a, b, c, d));

        cds_vector_dtor(v);
}

void test_vector_reserve()
{
        cds_vector_t *v;
        size_t count_reserved= 10;

        CU_ASSERT(cds_vector_ctor(&v, NULL) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_reserve(v, count_reserved) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_capacity(v) == count_reserved);

        CU_ASSERT(cds_vector_reserve(v, count_reserved - 1) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_capacity(v) == count_reserved);

        cds_vector_dtor(v);
}

void test_vector_data()
{
        cds_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, &c, &d, NULL) == CDS_STATUS_OK);
        CU_ASSERT(*((int *)(cds_vector_data(v)[0])) == a);
        CU_ASSERT(*((int *)(cds_vector_data(v)[1])) == b);
        CU_ASSERT(*((int *)(cds_vector_data(v)[2])) == c);
        CU_ASSERT(*((int *)(cds_vector_data(v)[3])) == d);

        cds_vector_dtor(v);
}

void test_vector_get()
{
        cds_vector_t *v;
        int a = 0, b = 1;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, NULL) == CDS_STATUS_OK);
        CU_ASSERT(*((int *)cds_vector_get(v, 0)) == a);
        CU_ASSERT(*((int *)cds_vector_get(v, 1)) == b);

        cds_vector_dtor(v);
}

void test_vector_at()
{
        cds_vector_t *v;
        int a = 0, b = 1, c = 2;
        void *elem = NULL;
        size_t index;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, &c, NULL) == CDS_STATUS_OK);

        CU_ASSERT(cds_vector_at(v, 0, &elem) == CDS_STATUS_OK);
        CU_ASSERT(*(int *)elem == a);
        CU_ASSERT(cds_vector_at(v, 1, &elem) == CDS_STATUS_OK);
        CU_ASSERT(*(int *)elem == b);
        CU_ASSERT(cds_vector_at(v, 2, &elem) == CDS_STATUS_OK);
        CU_ASSERT(*(int *)elem == c);

        index = cds_vector_size(v) + 1;
        CU_ASSERT(cds_vector_at(v, index, &elem) == CDS_STATUS_OUT_OF_RANGE);

        cds_vector_dtor(v);
}

void test_vector_front()
{
        cds_vector_t *v;
        int a = 1, b = 2;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, NULL) == CDS_STATUS_OK);
        CU_ASSERT(*((int *)cds_vector_front(v)) == a);

        cds_vector_dtor(v);
}

void test_vector_back()
{
        cds_vector_t *v;
        int a = 1, b = 2;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, NULL) == CDS_STATUS_OK);
        CU_ASSERT(*((int *)cds_vector_back(v)) == b);

        cds_vector_dtor(v);
}

void test_vector_insert()
{
        cds_vector_t *v;
        int a = 1, b = 2;
        int i1 = 3, i2 = 4, i3 = 5;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, NULL) == CDS_STATUS_OK);

        CU_ASSERT(cds_vector_insert(v, 0, &i1) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_size(v) == 3);
        CU_ASSERT(vector_range_int_eq(v, 3, i1, a, b));

        CU_ASSERT(cds_vector_insert(v, cds_vector_size(v), &i2) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_size(v) == 4);
        CU_ASSERT(vector_range_int_eq(v, 4, i1, a, b, i2));

        CU_ASSERT(cds_vector_insert(v, 2, &i3) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_size(v) == 5);
        CU_ASSERT(vector_range_int_eq(v, 5, i1, a, i3, b, i2));

        cds_vector_dtor(v);
}

void test_vector_erase()
{
        cds_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem = NULL;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, &c, &d, NULL) == CDS_STATUS_OK);

        CU_ASSERT(cds_vector_erase(v, 2, &elem) == CDS_STATUS_OK);
        CU_ASSERT(*((int *)elem) == c);
        CU_ASSERT(cds_vector_size(v) == 3);
        CU_ASSERT(vector_range_int_eq(v, 3, a, b, d));

        CU_ASSERT(cds_vector_erase(v, 0, &elem) == CDS_STATUS_OK);
        CU_ASSERT(*((int *)elem) == a);
        CU_ASSERT(cds_vector_size(v) == 2);
        CU_ASSERT(vector_range_int_eq(v, 2, b, d));

        CU_ASSERT(cds_vector_erase(v, cds_vector_size(v) - 1, &elem) == CDS_STATUS_OK);
        CU_ASSERT(*((int *)elem) == d);
        CU_ASSERT(cds_vector_size(v) == 1);
        CU_ASSERT(vector_range_int_eq(v, 1, b));

        cds_vector_dtor(v);
}

void test_vector_clear()
{
        cds_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, &c, &d, NULL) == CDS_STATUS_OK);

        cds_vector_clear(v);

        CU_ASSERT(cds_vector_size(v) == 0);

        cds_vector_dtor(v);
}

void test_vector_push_back()
{
        cds_vector_t *v;
        int a = 0, b = 1, c = 2;

        CU_ASSERT(cds_vector_ctor(&v, NULL) == CDS_STATUS_OK);

        cds_vector_push_back(v, &a);
        CU_ASSERT(cds_vector_size(v) == 1);
        CU_ASSERT(vector_range_int_eq(v, 1, a));

        cds_vector_push_back(v, &b);
        CU_ASSERT(cds_vector_size(v) == 2);
        CU_ASSERT(vector_range_int_eq(v, 1, a, b));

        cds_vector_push_back(v, &c);
        CU_ASSERT(cds_vector_size(v) == 3);
        CU_ASSERT(vector_range_int_eq(v, 3, a, b, c));

        cds_vector_dtor(v);
}

void test_vector_pop_back()
{
        cds_vector_t *v;
        int a = 0, b = 1, c = 2, d = 3;
        void *elem;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, &c, &d, NULL) == CDS_STATUS_OK);

        elem = cds_vector_back(v);
        cds_vector_pop_back(v);
        CU_ASSERT(cds_vector_size(v) == 3);
        CU_ASSERT(*((int *)elem) == d);

        elem = cds_vector_back(v);
        cds_vector_pop_back(v);
        CU_ASSERT(cds_vector_size(v) == 2);
        CU_ASSERT(*((int *)elem) == c);

        elem = cds_vector_back(v);
        cds_vector_pop_back(v);
        CU_ASSERT(cds_vector_size(v) == 1);
        CU_ASSERT(*((int *)elem) == b);

        elem = cds_vector_back(v);
        cds_vector_pop_back(v);
        CU_ASSERT(cds_vector_size(v) == 0);
        CU_ASSERT(*((int *)elem) == a);

        cds_vector_dtor(v);
}

void test_vector_swap()
{
        cds_vector_t *v, *w;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, &c, &d, NULL) == CDS_STATUS_OK);
        CU_ASSERT(cds_vector_ctor_l(&w, NULL, &a, &d, NULL) == CDS_STATUS_OK);

        cds_vector_swap(v, w);

        CU_ASSERT(cds_vector_size(v) == 2);
        CU_ASSERT(vector_range_int_eq(v, 2, a, d));
        CU_ASSERT(cds_vector_size(w) == 4);
        CU_ASSERT(vector_range_int_eq(w, 4, a, b, c, d));

        cds_vector_dtor(v);
        cds_vector_dtor(w);
}

