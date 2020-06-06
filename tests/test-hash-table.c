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
#define CDC_USE_SHORT_NAMES
#include "test-common.h"

#include "cdcontainers/casts.h"
#include "cdcontainers/hash-table.h"

#include <assert.h>
#include <float.h>
#include <stdarg.h>
#include <stdio.h>

#include <CUnit/Basic.h>

static pair_t a = {CDC_FROM_INT(0), CDC_FROM_INT(0)};
static pair_t b = {CDC_FROM_INT(1), CDC_FROM_INT(1)};
static pair_t c = {CDC_FROM_INT(2), CDC_FROM_INT(2)};
static pair_t d = {CDC_FROM_INT(3), CDC_FROM_INT(3)};
static pair_t e = {CDC_FROM_INT(4), CDC_FROM_INT(4)};
static pair_t f = {CDC_FROM_INT(5), CDC_FROM_INT(5)};
static pair_t g = {CDC_FROM_INT(6), CDC_FROM_INT(6)};
static pair_t h = {CDC_FROM_INT(7), CDC_FROM_INT(7)};

static int eq(const void *l, const void *r)
{
  return CDC_TO_INT(l) == CDC_TO_INT(r);
}

static int eq_plus_1(const void *l, const void *r)
{
  return CDC_TO_INT(l) + 1 == CDC_TO_INT(r) + 1;
}

static size_t hash(const void *val)
{
  return cdc_hash_int(CDC_TO_INT(val));
}

static size_t hash1(const void *val)
{
  return cdc_hash_uint(CDC_TO_UINT(val));
}

static bool hash_table_key_int_eq(hash_table_t *t, size_t count, ...)
{
  va_list args;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    pair_t *val = va_arg(args, pair_t *);
    void *tmp = NULL;
    if (hash_table_get(t, val->first, &tmp) != CDC_STATUS_OK || tmp != val->second) {
      va_end(args);
      return false;
    }
  }

  va_end(args);
  return true;
}

void test_hash_table_ctor()
{
  hash_table_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctor1(&t, &info, 1.0), CDC_STATUS_OK);
  CU_ASSERT(hash_table_empty(t));
  hash_table_dtor(t);
}

void test_hash_table_ctorl()
{
  hash_table_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctorl1(&t, &info, 1.0, &a, &b, &c, &d, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 4);
  CU_ASSERT(hash_table_key_int_eq(t, 4, &a, &b, &c, &d));
  hash_table_dtor(t);
}

void test_hash_table_get()
{
  hash_table_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctorl1(&t, &info, 1.0, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 8);
  CU_ASSERT(hash_table_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));
  void *value = NULL;
  CU_ASSERT_EQUAL(hash_table_get(t, CDC_FROM_INT(10), &value), CDC_STATUS_NOT_FOUND);
  hash_table_dtor(t);
}

void test_hash_table_count()
{
  hash_table_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctorl1(&t, &info, 1.0, &a, &b, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 2);
  CU_ASSERT_EQUAL(hash_table_count(t, a.first), 1);
  CU_ASSERT_EQUAL(hash_table_count(t, b.first), 1);
  CU_ASSERT_EQUAL(hash_table_count(t, CDC_FROM_INT(10)), 0);
  hash_table_dtor(t);
}

void test_hash_table_find()
{
  hash_table_t *t = NULL;
  hash_table_iter_t it = CDC_INIT_STRUCT;
  hash_table_iter_t it_end = CDC_INIT_STRUCT;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctorl1(&t, &info, 1.0, &a, &b, &c, &d, &g, CDC_END), CDC_STATUS_OK);
  hash_table_find(t, a.first, &it);
  CU_ASSERT_EQUAL(hash_table_iter_value(&it), a.second);
  hash_table_find(t, b.first, &it);
  CU_ASSERT_EQUAL(hash_table_iter_value(&it), b.second);
  hash_table_find(t, g.first, &it);
  CU_ASSERT_EQUAL(hash_table_iter_value(&it), g.second);
  hash_table_find(t, h.first, &it);
  hash_table_end(t, &it_end);
  CU_ASSERT(hash_table_iter_is_eq(&it, &it_end));
  hash_table_dtor(t);
}

void test_hash_table_clear()
{
  hash_table_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctorl1(&t, &info, 1.0, &a, &b, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 2);
  hash_table_clear(t);
  CU_ASSERT(hash_table_empty(t));
  hash_table_clear(t);
  CU_ASSERT(hash_table_empty(t));
  hash_table_dtor(t);
}

void test_hash_table_insert()
{
  hash_table_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctor1(&t, &info, 1.0), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_insert(t, a.first, a.second, NULL, NULL), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 1);
  CU_ASSERT(hash_table_key_int_eq(t, 1, &a));

  CU_ASSERT_EQUAL(hash_table_insert(t, a.first, b.second, NULL, NULL), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 1);

  void *value = NULL;
  CU_ASSERT_EQUAL(hash_table_get(t, a.first, &value), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(value, a.second);
  CU_ASSERT(hash_table_key_int_eq(t, 1, &a));

  CU_ASSERT_EQUAL(hash_table_insert(t, b.first, b.second, NULL, NULL), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 2);
  CU_ASSERT(hash_table_key_int_eq(t, 2, &a, &b));

  CU_ASSERT_EQUAL(hash_table_insert(t, c.first, c.second, NULL, NULL), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 3);
  CU_ASSERT(hash_table_key_int_eq(t, 3, &a, &b, &c));

  CU_ASSERT_EQUAL(hash_table_insert(t, d.first, d.second, NULL, NULL), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 4);
  CU_ASSERT(hash_table_key_int_eq(t, 4, &a, &b, &c, &d));

  CU_ASSERT_EQUAL(hash_table_insert(t, e.first, e.second, NULL, NULL), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 5);
  CU_ASSERT(hash_table_key_int_eq(t, 5, &a, &b, &c, &d, &e));
  hash_table_dtor(t);
}

void test_hash_table_insert_or_assign()
{
  hash_table_t *t = NULL;
  hash_table_iter_t it = CDC_INIT_STRUCT;
  bool inserted = false;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctor1(&t, &info, 1.0), CDC_STATUS_OK);

  CU_ASSERT_EQUAL(hash_table_insert_or_assign(t, a.first, a.second, &it, &inserted), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 1);
  CU_ASSERT_EQUAL(hash_table_iter_value(&it), a.second);
  CU_ASSERT(inserted);

  CU_ASSERT_EQUAL(hash_table_insert_or_assign(t, a.first, b.second, &it, &inserted), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 1);

  void *value = NULL;
  CU_ASSERT_EQUAL(hash_table_get(t, a.first, &value), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(value, b.second);
  CU_ASSERT_EQUAL(hash_table_iter_value(&it), b.second);
  CU_ASSERT(!inserted);

  CU_ASSERT_EQUAL(hash_table_insert_or_assign(t, c.first, c.second, &it, &inserted), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 2);
  CU_ASSERT_EQUAL(hash_table_iter_value(&it), c.second);
  CU_ASSERT(inserted);

  CU_ASSERT_EQUAL(hash_table_insert_or_assign(t, c.first, d.second, &it, &inserted), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 2);
  CU_ASSERT_EQUAL(hash_table_get(t, c.first, &value), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(value, d.second);
  CU_ASSERT_EQUAL(hash_table_iter_value(&it), d.second);
  CU_ASSERT(!inserted);
  hash_table_dtor(t);
}

void test_hash_table_erase()
{
  hash_table_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;
  void *value = NULL;

  CU_ASSERT_EQUAL(hash_table_ctorl1(&t, &info, 1.0, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 8);
  CU_ASSERT(hash_table_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));

  CU_ASSERT_EQUAL(hash_table_erase(t, a.first), 1);
  CU_ASSERT_EQUAL(hash_table_get(t, a.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT(hash_table_key_int_eq(t, 7, &b, &c, &d, &g, &h, &e, &f));

  CU_ASSERT_EQUAL(hash_table_erase(t, h.first), 1);
  CU_ASSERT_EQUAL(hash_table_get(t, h.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(hash_table_size(t), 6);
  CU_ASSERT(hash_table_key_int_eq(t, 6, &b, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(hash_table_erase(t, h.first), 0);
  CU_ASSERT_EQUAL(hash_table_size(t), 6);
  CU_ASSERT(hash_table_key_int_eq(t, 6, &b, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(hash_table_erase(t, b.first), 1);
  CU_ASSERT_EQUAL(hash_table_get(t, b.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(hash_table_size(t), 5);
  CU_ASSERT(hash_table_key_int_eq(t, 5, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(hash_table_erase(t, c.first), 1);
  CU_ASSERT_EQUAL(hash_table_get(t, c.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(hash_table_size(t), 4);
  CU_ASSERT(hash_table_key_int_eq(t, 4, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(hash_table_erase(t, d.first), 1);
  CU_ASSERT_EQUAL(hash_table_get(t, d.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(hash_table_size(t), 3);
  CU_ASSERT(hash_table_key_int_eq(t, 3, &g, &e, &f));

  CU_ASSERT_EQUAL(hash_table_erase(t, f.first), 1);
  CU_ASSERT_EQUAL(hash_table_get(t, f.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(hash_table_size(t), 2);
  CU_ASSERT(hash_table_key_int_eq(t, 2, &g, &e));

  CU_ASSERT_EQUAL(hash_table_erase(t, e.first), 1);
  CU_ASSERT_EQUAL(hash_table_get(t, e.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(hash_table_size(t), 1);
  CU_ASSERT(hash_table_key_int_eq(t, 1, &g));

  CU_ASSERT_EQUAL(hash_table_erase(t, g.first), 1);
  CU_ASSERT_EQUAL(hash_table_get(t, g.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT(hash_table_empty(t));
  hash_table_dtor(t);
}

void test_hash_table_swap()
{
  hash_table_t *ta = NULL;
  hash_table_t *tb = NULL;
  float lf_ta = 1.0;
  float lf_tb = 2.0;

  data_info_t infoa = CDC_INIT_STRUCT;
  infoa.eq = eq;
  infoa.hash = hash;

  data_info_t infob = CDC_INIT_STRUCT;
  infob.eq = eq_plus_1;
  infob.hash = hash1;

  CU_ASSERT_EQUAL(hash_table_ctorl1(&ta, &infoa, lf_ta, &a, &b, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_ctorl1(&tb, &infob, lf_tb, &c, &d, CDC_END), CDC_STATUS_OK);

  hash_table_swap(ta, tb);

  CU_ASSERT_EQUAL(ta->dinfo->eq, eq_plus_1);
  CU_ASSERT_EQUAL(ta->dinfo->hash, hash1);
  CU_ASSERT_EQUAL(hash_table_max_load_factor(ta), lf_tb);
  CU_ASSERT(hash_table_key_int_eq(ta, 2, &c, &d));

  CU_ASSERT_EQUAL(tb->dinfo->eq, eq);
  CU_ASSERT_EQUAL(tb->dinfo->hash, hash);
  CU_ASSERT_EQUAL(hash_table_max_load_factor(tb), lf_ta);
  CU_ASSERT(hash_table_key_int_eq(tb, 2, &a, &b));
  hash_table_dtor(ta);
  hash_table_dtor(tb);
}

void test_hash_table_rehash()
{
  hash_table_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctorl(&t, &info, 0.5, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_size(t), 8);
  CU_ASSERT(hash_table_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));

  size_t bcount = hash_table_bucket_count(t) * 100;
  CU_ASSERT_EQUAL(hash_table_rehash(t, bcount), CDC_STATUS_OK);
  CU_ASSERT(hash_table_bucket_count(t) >= bcount);

  CU_ASSERT_EQUAL(hash_table_size(t), 8);
  CU_ASSERT(hash_table_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));
  hash_table_dtor(t);
}

void test_hash_table_reserve()
{
  hash_table_t *t = NULL;
  size_t count = 100;
  data_info_t info = CDC_INIT_STRUCT;
  info.eq = eq;
  info.hash = hash;

  CU_ASSERT_EQUAL(hash_table_ctor1(&t, &info, 1.0), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(hash_table_reserve(t, count), CDC_STATUS_OK);
  CU_ASSERT((size_t)(hash_table_bucket_count(t) * cdc_hash_table_max_load_factor(t)) >= count);
  hash_table_dtor(t);
}
