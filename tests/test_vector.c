#include "test_common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include "cdstructures.h"

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

        CU_ASSERT(*((int *)cds_vector_get(v, a)) == a);
        CU_ASSERT(*((int *)cds_vector_get(v, b)) == b);
        CU_ASSERT(*((int *)cds_vector_get(v, c)) == c);
        CU_ASSERT(*((int *)cds_vector_get(v, d)) == d);

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

void test_vector_getters()
{
        cds_vector_t *v;
        int a = 0, b = 1, c = 2;
        void* r = NULL;
        size_t index;

        CU_ASSERT(cds_vector_ctor_l(&v, NULL, &a, &b, &c, NULL) == CDS_STATUS_OK);
        CU_ASSERT(*((int *)cds_vector_get(v, 0)) == a);
        CU_ASSERT(*((int *)cds_vector_get(v, 1)) == b);

        CU_ASSERT(cds_vector_at(v, 2, &r) == CDS_STATUS_OK);
        CU_ASSERT(*(int *)r == c);

        index = cds_vector_size(v) + 1;
        CU_ASSERT(cds_vector_at(v, index, &r) == CDS_STATUS_OUT_OF_RANGE);

        cds_vector_dtor(v);
}
