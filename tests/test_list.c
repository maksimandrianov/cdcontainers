#include "test_common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>
#include "cdstructures.h"

static void list_int_print(void *val, size_t index)
{
        printf("%u\n", *((int *)val));
}

static bool list_range_int_eq(cds_list_t *l, size_t count, ...)
{
        va_list args;
        int elem, i;
        void *val;
        enum cds_stat ret;

        va_start(args, count);
        for (i = 0; i < count; ++i) {
                elem = va_arg(args, int);
                if ((ret = cds_list_at(l, i, &val)) != CDS_STATUS_OK)
                        return false;

                if (elem != *((int *)val))
                        return false;
        }

        return true;
}

void test_list_ctor()
{
        cds_list_t *l;

        CU_ASSERT(cds_list_ctor(&l, NULL) == CDS_STATUS_OK);
        CU_ASSERT(cds_list_size(l) == 0);

        cds_list_dtor(l);
}

void test_list_ctor_list()
{
        cds_list_t *l;

        int a = 0, b = 1, c = 2, d =3;
        void *f;

        CU_ASSERT(cds_list_ctor_l(&l, NULL, &a, &b, &c, &d, NULL) == CDS_STATUS_OK);
        CU_ASSERT(cds_list_size(l) == 4);
        CU_ASSERT(list_range_int_eq(l, 4, a, b, c, d));

        cds_list_dtor(l);
}

void test_list_push_back()
{
        CU_ASSERT(true);
}

void test_list_push_front()
{
        CU_ASSERT(true);
}
