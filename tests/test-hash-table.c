// The MIT License (MIT)
// Copyright (c) 2018 Maksim Andrianov
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
#include <assert.h>
#include <stdio.h>
#include <float.h>
#include "cdcontainers/casts.h"
#include "cdcontainers/hash-table.h"

static struct cdc_pair a = {CDC_INT_TO_PTR(0), CDC_INT_TO_PTR(0)};
static struct cdc_pair b = {CDC_INT_TO_PTR(1), CDC_INT_TO_PTR(1)};
static struct cdc_pair c = {CDC_INT_TO_PTR(2), CDC_INT_TO_PTR(2)};
static struct cdc_pair d = {CDC_INT_TO_PTR(3), CDC_INT_TO_PTR(3)};
static struct cdc_pair e = {CDC_INT_TO_PTR(4), CDC_INT_TO_PTR(4)};
static struct cdc_pair f = {CDC_INT_TO_PTR(5), CDC_INT_TO_PTR(5)};
static struct cdc_pair g = {CDC_INT_TO_PTR(6), CDC_INT_TO_PTR(6)};
static struct cdc_pair h = {CDC_INT_TO_PTR(7), CDC_INT_TO_PTR(7)};

static int eq_ptr(const void *l, const void *r)
{
        return l == r;
}

static int eq_ptr2(const void *l, const void *r)
{
        return l == r;
}

static bool hash_table_key_int_eq(struct cdc_hash_table *t, size_t count, ...)
{
        va_list args;
        int i;
        struct cdc_pair *val;
        void *tmp;

        va_start(args, count);
        for (i = 0; i < count; ++i) {
                val = va_arg(args, struct cdc_pair *);
                if (cdc_hash_table_get(t, val->first, &tmp) != CDC_STATUS_OK ||
                    tmp != val->second) {
                        va_end(args);
                        return false;
                }
        }

        va_end(args);
        return true;
}

void test_hash_table_ctor()
{
        struct cdc_hash_table *t = NULL;

        CU_ASSERT(cdc_hash_table_ctor2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 0);

        cdc_hash_table_dtor(t);
}

void test_hash_table_ctorl()
{
        struct cdc_hash_table *t = NULL;

        CU_ASSERT(cdc_hash_table_ctorl2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0,
                                        &a, &b, &c, &d, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 4);
        CU_ASSERT(hash_table_key_int_eq(t, 4, &a, &b, &c, &d));

        cdc_hash_table_dtor(t);
}

void test_hash_table_get()
{
        struct cdc_hash_table *t = NULL;
        void *value;

        CU_ASSERT(cdc_hash_table_ctorl2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0,
                                        &a, &b, &c, &d, &g, &h, &e, &f, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 8);
        CU_ASSERT(hash_table_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));
        CU_ASSERT(cdc_hash_table_get(t, CDC_INT_TO_PTR(10), &value) == CDC_STATUS_NOT_FOUND);
        cdc_hash_table_dtor(t);
}

void test_hash_table_count()
{
        struct cdc_hash_table *t = NULL;

        CU_ASSERT(cdc_hash_table_ctorl2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0,
                                        &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 2);
        CU_ASSERT(cdc_hash_table_count(t, a.first) == 1);
        CU_ASSERT(cdc_hash_table_count(t, b.first) == 1);
        CU_ASSERT(cdc_hash_table_count(t, CDC_INT_TO_PTR(10)) == 0);

        cdc_hash_table_dtor(t);
}

void test_hash_table_find()
{
        struct cdc_hash_table *t = NULL;
        struct cdc_hash_table_iter it, it_end;

        CU_ASSERT(cdc_hash_table_ctorl2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0,
                                        &a, &b, &c, &d, &g, NULL) == CDC_STATUS_OK);

        cdc_hash_table_find(t, a.first, &it);
        CU_ASSERT(cdc_hash_table_iter_value(&it) == a.second);
        cdc_hash_table_find(t, b.first, &it);
        CU_ASSERT(cdc_hash_table_iter_value(&it) == b.second);
        cdc_hash_table_find(t, g.first, &it);
        CU_ASSERT(cdc_hash_table_iter_value(&it) == g.second);
        cdc_hash_table_find(t, h.first, &it);
        cdc_hash_table_end(t, &it_end);
        CU_ASSERT(cdc_hash_table_iter_is_eq(&it, &it_end));

        cdc_hash_table_dtor(t);

}

void test_hash_table_equal_range()
{
        struct cdc_pair_hash_table_iter res;
        struct cdc_hash_table *t = NULL;

        CU_ASSERT(cdc_hash_table_ctorl2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0,
                                        &a, &b, &c, &d, &g, NULL) == CDC_STATUS_OK);

        cdc_hash_table_equal_range(t, a.first, &res);
        CU_ASSERT(cdc_hash_table_iter_value(&res.first) == a.second);
        cdc_hash_table_iter_next(&res.first);
        CU_ASSERT(cdc_hash_table_iter_is_eq(&res.second, &res.first));

        cdc_hash_table_equal_range(t, b.first, &res);
        CU_ASSERT(cdc_hash_table_iter_value(&res.first) == b.second);
        cdc_hash_table_iter_next(&res.first);
        CU_ASSERT(cdc_hash_table_iter_is_eq(&res.second, &res.first));

        cdc_hash_table_equal_range(t, d.first, &res);
        CU_ASSERT(cdc_hash_table_iter_value(&res.first) == d.second);
        cdc_hash_table_iter_next(&res.first);
        CU_ASSERT(cdc_hash_table_iter_is_eq(&res.second, &res.first));

        cdc_hash_table_equal_range(t, g.first, &res);
        CU_ASSERT(cdc_hash_table_iter_value(&res.first) == g.second);
        cdc_hash_table_iter_next(&res.first);
        CU_ASSERT(cdc_hash_table_iter_is_eq(&res.second, &res.first));

        cdc_hash_table_dtor(t);
}


void test_hash_table_clear()
{
        struct cdc_hash_table *t = NULL;

        CU_ASSERT(cdc_hash_table_ctorl2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0,
                                        &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 2);
        cdc_hash_table_clear(t);
        CU_ASSERT(cdc_hash_table_size(t) == 0);
        cdc_hash_table_clear(t);
        CU_ASSERT(cdc_hash_table_size(t) == 0);

        cdc_hash_table_dtor(t);
}

void test_hash_table_insert()
{
        struct cdc_hash_table *t = NULL;
        void *value;

        CU_ASSERT(cdc_hash_table_ctor2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0) == CDC_STATUS_OK);

        CU_ASSERT(cdc_hash_table_insert(t, a.first, a.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 1);
        CU_ASSERT(hash_table_key_int_eq(t, 1, &a));

        CU_ASSERT(cdc_hash_table_insert(t, a.first, b.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 1);
        CU_ASSERT(cdc_hash_table_get(t, a.first, &value) == CDC_STATUS_OK);
        CU_ASSERT(value == a.second);
        CU_ASSERT(hash_table_key_int_eq(t, 1, &a));

        CU_ASSERT(cdc_hash_table_insert(t, b.first, b.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 2);
        CU_ASSERT(hash_table_key_int_eq(t, 2, &a, &b));

        CU_ASSERT(cdc_hash_table_insert(t, c.first, c.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 3);
        CU_ASSERT(hash_table_key_int_eq(t, 3, &a, &b, &c));

        CU_ASSERT(cdc_hash_table_insert(t, d.first, d.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 4);
        CU_ASSERT(hash_table_key_int_eq(t, 4, &a, &b, &c, &d));

        CU_ASSERT(cdc_hash_table_insert(t, e.first, e.second, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 5);
        CU_ASSERT(hash_table_key_int_eq(t, 5, &a, &b, &c, &d, &e));

        cdc_hash_table_dtor(t);
}

void test_hash_table_insert_or_assign()
{
        struct cdc_pair_hash_table_iter_bool ret;
        struct cdc_hash_table *t = NULL;
        void *value;

        CU_ASSERT(cdc_hash_table_ctor2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0) == CDC_STATUS_OK);

        CU_ASSERT(cdc_hash_table_insert_or_assign(t, a.first, a.second, &ret) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 1);
        CU_ASSERT(cdc_hash_table_iter_value(&ret.first) == a.second);
        CU_ASSERT(ret.second == true);

        CU_ASSERT(cdc_hash_table_insert_or_assign(t, a.first, b.second, &ret) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 1);
        CU_ASSERT(cdc_hash_table_get(t, a.first, &value) == CDC_STATUS_OK);
        CU_ASSERT(value == b.second);
        CU_ASSERT(cdc_hash_table_iter_value(&ret.first) == b.second);
        CU_ASSERT(ret.second == false);

        CU_ASSERT(cdc_hash_table_insert_or_assign(t, c.first, c.second, &ret) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 2);
        CU_ASSERT(cdc_hash_table_iter_value(&ret.first) == c.second);
        CU_ASSERT(ret.second == true);

        CU_ASSERT(cdc_hash_table_insert_or_assign(t, c.first, d.second, &ret) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 2);
        CU_ASSERT(cdc_hash_table_get(t, c.first, &value) == CDC_STATUS_OK);
        CU_ASSERT(value == d.second);
        CU_ASSERT(cdc_hash_table_iter_value(&ret.first) == d.second);
        CU_ASSERT(ret.second == false);

        cdc_hash_table_dtor(t);
}

void test_hash_table_erase()
{
        struct cdc_hash_table *t = NULL;
        void *value;

        CU_ASSERT(cdc_hash_table_ctorl2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0,
                                        &a, &b, &c, &d, &g, &h, &e, &f, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 8);
        CU_ASSERT(hash_table_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));

        CU_ASSERT(cdc_hash_table_erase(t, a.first) == 1);
        CU_ASSERT(cdc_hash_table_get(t, a.first, &value) == CDC_STATUS_NOT_FOUND);
        CU_ASSERT(hash_table_key_int_eq(t, 7, &b, &c, &d, &g, &h, &e, &f));

        CU_ASSERT(cdc_hash_table_erase(t, h.first) == 1);
        CU_ASSERT(cdc_hash_table_get(t, h.first, &value) == CDC_STATUS_NOT_FOUND);
        CU_ASSERT(cdc_hash_table_size(t) == 6);
        CU_ASSERT(hash_table_key_int_eq(t, 6, &b, &c, &d, &g, &e, &f));

        CU_ASSERT(cdc_hash_table_erase(t, h.first) == 0);
        CU_ASSERT(cdc_hash_table_size(t) == 6);
        CU_ASSERT(hash_table_key_int_eq(t, 6, &b, &c, &d, &g, &e, &f));

        CU_ASSERT(cdc_hash_table_erase(t, b.first) == 1);
        CU_ASSERT(cdc_hash_table_get(t, b.first, &value) == CDC_STATUS_NOT_FOUND);
        CU_ASSERT(cdc_hash_table_size(t) == 5);
        CU_ASSERT(hash_table_key_int_eq(t, 5, &c, &d, &g, &e, &f));

        CU_ASSERT(cdc_hash_table_erase(t, c.first) == 1);
        CU_ASSERT(cdc_hash_table_get(t, c.first, &value) == CDC_STATUS_NOT_FOUND);
        CU_ASSERT(cdc_hash_table_size(t) == 4);
        CU_ASSERT(hash_table_key_int_eq(t, 4, &d, &g, &e, &f));

        CU_ASSERT(cdc_hash_table_erase(t, d.first) == 1);
        CU_ASSERT(cdc_hash_table_get(t, d.first, &value) == CDC_STATUS_NOT_FOUND);
        CU_ASSERT(cdc_hash_table_size(t) == 3);
        CU_ASSERT(hash_table_key_int_eq(t, 3, &g, &e, &f));

        CU_ASSERT(cdc_hash_table_erase(t, f.first) == 1);
        CU_ASSERT(cdc_hash_table_get(t, f.first, &value) == CDC_STATUS_NOT_FOUND);
        CU_ASSERT(cdc_hash_table_size(t) == 2);
        CU_ASSERT(hash_table_key_int_eq(t, 2, &g, &e));

        CU_ASSERT(cdc_hash_table_erase(t, e.first) == 1);
        CU_ASSERT(cdc_hash_table_get(t, e.first, &value) == CDC_STATUS_NOT_FOUND);
        CU_ASSERT(cdc_hash_table_size(t) == 1);
        CU_ASSERT(hash_table_key_int_eq(t, 1, &g));

        CU_ASSERT(cdc_hash_table_erase(t, g.first) == 1);
        CU_ASSERT(cdc_hash_table_get(t, g.first, &value) == CDC_STATUS_NOT_FOUND);
        CU_ASSERT(cdc_hash_table_size(t) == 0);

        cdc_hash_table_dtor(t);
}

void test_hash_table_swap()
{
        struct cdc_hash_table *ta = NULL, *tb = NULL;
        float lf_ta = 1.0, lf_tb = 2.0;

        CU_ASSERT(cdc_hash_table_ctorl2(&ta, NULL, cdc_pdhash_int, eq_ptr, lf_ta,
                                        &a, &b, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_ctorl2(&tb, NULL, cdc_pdhash_uint, eq_ptr2, lf_tb,
                                        &c, &d, NULL) == CDC_STATUS_OK);

        cdc_hash_table_swap(ta, tb);

        CU_ASSERT(ta->eq == eq_ptr2);
        CU_ASSERT(ta->hash == cdc_pdhash_uint);
        CU_ASSERT(cdc_hash_table_max_load_factor(ta) == lf_tb);
        CU_ASSERT(hash_table_key_int_eq(ta, 2, &c, &d));

        CU_ASSERT(tb->eq == eq_ptr);
        CU_ASSERT(tb->hash == cdc_pdhash_int);
        CU_ASSERT(cdc_hash_table_max_load_factor(tb) == lf_ta);
        CU_ASSERT(hash_table_key_int_eq(tb, 2, &a, &b));

        cdc_hash_table_dtor(ta);
        cdc_hash_table_dtor(tb);
}

void test_hash_table_rehash()
{
        struct cdc_hash_table *t = NULL;
        size_t bcount;

        CU_ASSERT(cdc_hash_table_ctorl2(&t, NULL, cdc_pdhash_int, eq_ptr, 0.5,
                                        &a, &b, &c, &d, &g, &h, &e, &f, NULL) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_size(t) == 8);
        CU_ASSERT(hash_table_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));

        bcount = cdc_hash_table_bucket_count(t) * 100;

        CU_ASSERT(cdc_hash_table_rehash(t, bcount) == CDC_STATUS_OK);
        CU_ASSERT(cdc_hash_table_bucket_count(t) >= bcount);

        CU_ASSERT(cdc_hash_table_size(t) == 8);
        CU_ASSERT(hash_table_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));

        cdc_hash_table_dtor(t);
}

void test_hash_table_reserve()
{
        struct cdc_hash_table *t = NULL;
        size_t count = 100;

        CU_ASSERT(cdc_hash_table_ctor2(&t, NULL, cdc_pdhash_int, eq_ptr, 1.0) == CDC_STATUS_OK);

        CU_ASSERT(cdc_hash_table_reserve(t, count) == CDC_STATUS_OK);
        CU_ASSERT((size_t)(cdc_hash_table_bucket_count(t) * cdc_hash_table_max_load_factor(t)) >= count);

        cdc_hash_table_dtor(t);
}
