// The MIT License (MIT)
// Copyright (c) 2017 Maksim Andrianov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
#include "test-common.h"

#include <CUnit/Basic.h>
#include <float.h>
#include <stdarg.h>

static bool list_range_int_eq(struct cdc_list *l, size_t count, ...)
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
        struct cdc_list *l = NULL;

        CU_ASSERT(cdc_list_ctor(&l, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 0);

        cdc_list_dtor(l);
}

void test_list_ctorl()
{
        struct cdc_list *l = NULL;
        int a = 0, b = 1, c = 2, d =3;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_list_size(l) == 4);
        CU_ASSERT(list_range_int_eq(l, 4, a, b, c, d));

        cdc_list_dtor(l);
}

void test_list_push_back()
{
        struct cdc_list *l = NULL;
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
        struct cdc_list *l = NULL;
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
        struct cdc_list *l;
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
        struct cdc_list *l;
        int a = 1, b = 2;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_list_front(l)) == a);

        cdc_list_dtor(l);
}

void test_list_back()
{
        struct cdc_list *l;
        int a = 1, b = 2;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(*((int *)cdc_list_back(l)) == b);

        cdc_list_dtor(l);
}

void test_list_pop_back()
{
        struct cdc_list *l;
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
        struct cdc_list *l;
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
        struct cdc_list *v, *w;
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
        struct cdc_list *l;
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
        struct cdc_list *l;
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
        struct cdc_list *l;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        cdc_list_clear(l);

        CU_ASSERT(cdc_list_size(l) == 0);

        cdc_list_dtor(l);
}

void test_list_iterators()
{
        struct cdc_list *l;
        struct cdc_list_iter it;
        struct cdc_list_reverse_iter rit;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        it = cdc_list_begin(l);
        CU_ASSERT(cdc_list_iter_has_next(it) == true);
        CU_ASSERT(cdc_list_iter_has_prev(it) == false);
        CU_ASSERT(cdc_list_iter_data(it) == (void *)&a);
        rit = cdc_list_riter_from(it);
        CU_ASSERT(cdc_list_riter_is_eq(cdc_list_rend(l), rit) == true);

        it = cdc_list_iter_next(it);
        CU_ASSERT(cdc_list_iter_has_next(it) == true);
        CU_ASSERT(cdc_list_iter_has_prev(it) == true);
        CU_ASSERT(cdc_list_iter_data(it) == (void *)&b);
        rit = cdc_list_riter_from(it);
        CU_ASSERT(cdc_list_riter_data(rit) == (void *)&a);

        it = cdc_list_iter_next(it);
        CU_ASSERT(cdc_list_iter_has_next(it) == true);
        CU_ASSERT(cdc_list_iter_has_prev(it) == true);
        CU_ASSERT(cdc_list_iter_data(it) == (void *)&c);
        rit = cdc_list_riter_from(it);
        CU_ASSERT(cdc_list_riter_data(rit) == (void *)&b);

        it = cdc_list_iter_next(it);
        CU_ASSERT(cdc_list_iter_has_next(it) == false);
        CU_ASSERT(cdc_list_iter_has_prev(it) == true);
        CU_ASSERT(cdc_list_iter_data(it) == (void *)&d);
        rit = cdc_list_riter_from(it);
        CU_ASSERT(cdc_list_riter_data(rit) == (void *)&c);

        it = cdc_list_iter_next(it);
        CU_ASSERT(cdc_list_iter_is_eq(cdc_list_end(l), it) == true);
        rit = cdc_list_riter_from(it);
        CU_ASSERT(cdc_list_riter_is_eq(cdc_list_rbegin(l), rit) == true);

        it = cdc_list_iter_prev(it);
        CU_ASSERT(cdc_list_iter_has_next(it) == false);
        CU_ASSERT(cdc_list_iter_has_prev(it) == true);
        CU_ASSERT(cdc_list_iter_data(it) == (void *)&d);

        cdc_list_dtor(l);
}

void test_list_reverse_iterators()
{
        struct cdc_list *l;
        struct cdc_list_iter it;
        struct cdc_list_reverse_iter rit;
        int a = 0, b = 1, c = 2, d = 3;

        CU_ASSERT(cdc_list_ctorl(&l, NULL, &a, &b, &c, &d, NULL) == CDC_STATUS_OK);

        rit = cdc_list_rbegin(l);
        CU_ASSERT(cdc_list_riter_has_next(rit) == true);
        CU_ASSERT(cdc_list_riter_has_prev(rit) == false);
        CU_ASSERT(cdc_list_riter_data(rit) == (void *)&d);
        it = cdc_list_iter_from(rit);
        CU_ASSERT(cdc_list_iter_is_eq(cdc_list_end(l), it) == true);

        rit = cdc_list_riter_next(rit);
        CU_ASSERT(cdc_list_riter_has_next(rit) == true);
        CU_ASSERT(cdc_list_riter_has_prev(rit) == true);
        CU_ASSERT(cdc_list_riter_data(rit) == (void *)&c);
        it = cdc_list_iter_from(rit);
        CU_ASSERT(cdc_list_iter_data(it) == (void *)&d);

        rit = cdc_list_riter_next(rit);
        CU_ASSERT(cdc_list_riter_has_next(rit) == true);
        CU_ASSERT(cdc_list_riter_has_prev(rit) == true);
        CU_ASSERT(cdc_list_riter_data(rit) == (void *)&b);
        it = cdc_list_iter_from(rit);
        CU_ASSERT(cdc_list_iter_data(it) == (void *)&c);

        rit = cdc_list_riter_next(rit);
        CU_ASSERT(cdc_list_riter_has_next(rit) == false);
        CU_ASSERT(cdc_list_riter_has_prev(rit) == true);
        CU_ASSERT(cdc_list_riter_data(rit) == (void *)&a);
        it = cdc_list_iter_from(rit);
        CU_ASSERT(cdc_list_iter_data(it) == (void *)&b);

        rit = cdc_list_riter_next(rit);
        CU_ASSERT(cdc_list_riter_is_eq(cdc_list_rend(l), rit) == true);
        it = cdc_list_iter_from(rit);
        CU_ASSERT(cdc_list_iter_is_eq(cdc_list_begin(l), it) == true);

        rit = cdc_list_riter_prev(rit);
        CU_ASSERT(cdc_list_riter_has_next(rit) == false);
        CU_ASSERT(cdc_list_riter_has_prev(rit) == true);
        CU_ASSERT(cdc_list_riter_data(rit) == (void *)&a);

        cdc_list_dtor(l);
}
