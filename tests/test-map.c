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

#include "cdcontainers/adapters/map.h"
#include "cdcontainers/casts.h"

#include <float.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

static pair_t a = {CDC_FROM_INT(0), CDC_FROM_INT(0)};
static pair_t b = {CDC_FROM_INT(1), CDC_FROM_INT(1)};
static pair_t c = {CDC_FROM_INT(2), CDC_FROM_INT(2)};
static pair_t d = {CDC_FROM_INT(3), CDC_FROM_INT(3)};
static pair_t e = {CDC_FROM_INT(4), CDC_FROM_INT(4)};
static pair_t f = {CDC_FROM_INT(5), CDC_FROM_INT(5)};
static pair_t g = {CDC_FROM_INT(6), CDC_FROM_INT(6)};
static pair_t h = {CDC_FROM_INT(7), CDC_FROM_INT(7)};

static int lt(const void *l, const void *r)
{
  return CDC_TO_INT(l) < CDC_TO_INT(r);
}

static int eq(const void *l, const void *r)
{
  return CDC_TO_INT(l) == CDC_TO_INT(r);
}

static size_t hash(const void *val)
{
  return cdc_hash_int(CDC_TO_INT(val));
}

static bool map_key_int_eq(map_t *t, size_t count, ...)
{
  va_list args;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    pair_t *val = va_arg(args, pair_t *);
    void *tmp = NULL;
    if (cdc_map_get(t, val->first, &tmp) != CDC_STATUS_OK || tmp != val->second) {
      va_end(args);
      return false;
    }
  }

  va_end(args);
  return true;
}

void test_map_ctor()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctor(tables[t], &m, &info), CDC_STATUS_OK);
    CU_ASSERT(map_empty(m));
    map_dtor(m);
  }
}

void test_map_ctorl()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctorl(tables[t], &m, &info, &a, &g, &h, &d, CDC_END), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 4);
    CU_ASSERT(map_key_int_eq(m, 4, &a, &g, &h, &d));
    map_dtor(m);
  }
}

void test_map_get()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    void *value = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctorl(tables[t], &m, &info, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 8);
    CU_ASSERT(map_key_int_eq(m, 8, &a, &b, &c, &d, &g, &h, &e, &f));
    CU_ASSERT_EQUAL(map_get(m, CDC_FROM_INT(10), &value), CDC_STATUS_NOT_FOUND);
    map_dtor(m);
  }
}

void test_map_count()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctorl(tables[t], &m, &info, &a, &b, CDC_END), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 2);
    CU_ASSERT_EQUAL(map_count(m, a.first), 1);
    CU_ASSERT_EQUAL(map_count(m, b.first), 1);
    CU_ASSERT_EQUAL(map_count(m, CDC_FROM_INT(10)), 0);
    map_dtor(m);
  }
}

void test_map_find()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    map_iter_t it = CDC_INIT_STRUCT;
    map_iter_t it_end = CDC_INIT_STRUCT;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctorl(tables[t], &m, &info, &a, &b, &c, &d, &g, CDC_END), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_iter_ctor(m, &it), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_iter_ctor(m, &it_end), CDC_STATUS_OK);

    map_find(m, a.first, &it);
    CU_ASSERT_EQUAL(map_iter_value(&it), a.second);
    map_find(m, b.first, &it);
    CU_ASSERT_EQUAL(map_iter_value(&it), b.second);
    map_find(m, g.first, &it);
    CU_ASSERT_EQUAL(map_iter_value(&it), g.second);
    map_find(m, h.first, &it);
    map_end(m, &it_end);
    CU_ASSERT(map_iter_is_eq(&it, &it_end));
    map_iter_dtor(&it);
    map_iter_dtor(&it_end);
    map_dtor(m);
  }
}

void test_map_clear()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctorl(tables[t], &m, &info, &a, &b, CDC_END), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 2);
    map_clear(m);
    CU_ASSERT(map_empty(m));
    map_clear(m);
    CU_ASSERT(map_empty(m));
    map_dtor(m);
  }
}

void test_map_insert()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    const int count = 100;
    void *val = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctor(tables[t], &m, &info), CDC_STATUS_OK);
    for (int i = 0; i < count; ++i) {
      CU_ASSERT_EQUAL(map_insert(m, CDC_FROM_INT(i), CDC_FROM_INT(i), NULL, NULL), CDC_STATUS_OK);
    }

    CU_ASSERT_EQUAL(map_size(m), count);
    for (int i = 0; i < count; ++i) {
      CU_ASSERT_EQUAL(map_get(m, CDC_FROM_INT(i), &val), CDC_STATUS_OK);
      CU_ASSERT_EQUAL(CDC_TO_INT(val), i);
    }
    map_dtor(m);
  }
}

void test_map_insert_or_assign()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    map_iter_t it = CDC_INIT_STRUCT;
    bool inserted = false;
    void *value = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctor(tables[t], &m, &info), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_iter_ctor(m, &it), CDC_STATUS_OK);

    CU_ASSERT_EQUAL(map_insert_or_assign(m, a.first, a.second, &it, &inserted), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 1);
    CU_ASSERT_EQUAL(map_iter_value(&it), a.second);
    CU_ASSERT(inserted);

    CU_ASSERT_EQUAL(map_insert_or_assign(m, a.first, b.second, &it, &inserted), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 1);
    CU_ASSERT_EQUAL(map_get(m, a.first, &value), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(value, b.second);
    CU_ASSERT_EQUAL(map_iter_value(&it), b.second);
    CU_ASSERT(!inserted);

    CU_ASSERT_EQUAL(map_insert_or_assign(m, c.first, c.second, &it, &inserted), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 2);
    CU_ASSERT_EQUAL(map_iter_value(&it), c.second);
    CU_ASSERT(inserted);

    CU_ASSERT_EQUAL(map_insert_or_assign(m, c.first, d.second, &it, &inserted), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 2);
    CU_ASSERT_EQUAL(map_get(m, c.first, &value), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(value, d.second);
    CU_ASSERT_EQUAL(map_iter_value(&it), d.second);
    CU_ASSERT(!inserted);
    map_iter_dtor(&it);
    map_dtor(m);
  }
}

void test_map_erase()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    void *value = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctorl(tables[t], &m, &info, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 8);
    CU_ASSERT(map_key_int_eq(m, 8, &a, &b, &c, &d, &g, &h, &e, &f));
    CU_ASSERT_EQUAL(map_erase(m, a.first), 1);

    CU_ASSERT_EQUAL(map_get(m, a.first, &value), CDC_STATUS_NOT_FOUND);
    CU_ASSERT(map_key_int_eq(m, 7, &b, &c, &d, &g, &h, &e, &f));

    CU_ASSERT_EQUAL(map_erase(m, h.first), 1);
    CU_ASSERT_EQUAL(map_get(m, h.first, &value), CDC_STATUS_NOT_FOUND);
    CU_ASSERT_EQUAL(map_size(m), 6);
    CU_ASSERT(map_key_int_eq(m, 6, &b, &c, &d, &g, &e, &f));

    CU_ASSERT_EQUAL(map_erase(m, h.first), 0);
    CU_ASSERT_EQUAL(map_size(m), 6);
    CU_ASSERT(map_key_int_eq(m, 6, &b, &c, &d, &g, &e, &f));

    CU_ASSERT_EQUAL(map_erase(m, b.first), 1);
    CU_ASSERT_EQUAL(map_get(m, b.first, &value), CDC_STATUS_NOT_FOUND);
    CU_ASSERT_EQUAL(map_size(m), 5);
    CU_ASSERT(map_key_int_eq(m, 5, &c, &d, &g, &e, &f));

    CU_ASSERT_EQUAL(map_erase(m, c.first), 1);
    CU_ASSERT_EQUAL(map_get(m, c.first, &value), CDC_STATUS_NOT_FOUND);
    CU_ASSERT_EQUAL(map_size(m), 4);
    CU_ASSERT(map_key_int_eq(m, 4, &d, &g, &e, &f));

    CU_ASSERT_EQUAL(map_erase(m, d.first), 1);
    CU_ASSERT_EQUAL(map_get(m, d.first, &value), CDC_STATUS_NOT_FOUND);
    CU_ASSERT_EQUAL(map_size(m), 3);
    CU_ASSERT(map_key_int_eq(m, 3, &g, &e, &f));

    CU_ASSERT_EQUAL(map_erase(m, f.first), 1);
    CU_ASSERT_EQUAL(map_get(m, f.first, &value), CDC_STATUS_NOT_FOUND);
    CU_ASSERT_EQUAL(map_size(m), 2);
    CU_ASSERT(map_key_int_eq(m, 2, &g, &e));

    CU_ASSERT_EQUAL(map_erase(m, e.first), 1);
    CU_ASSERT_EQUAL(map_get(m, e.first, &value), CDC_STATUS_NOT_FOUND);
    CU_ASSERT_EQUAL(map_size(m), 1);
    CU_ASSERT(map_key_int_eq(m, 1, &g));

    CU_ASSERT_EQUAL(map_erase(m, g.first), 1);
    CU_ASSERT_EQUAL(map_get(m, g.first, &value), CDC_STATUS_NOT_FOUND);
    CU_ASSERT(map_empty(m));
    map_dtor(m);
  }
}

void test_map_swap()
{
}

void test_map_iterators()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    CU_ASSERT_EQUAL(map_ctorl(tables[t], &m, &info, &a, &b, &c, &d, &e, &f, &g, &h, CDC_END),
                    CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_size(m), 8);

    map_iter_t it1 = CDC_INIT_STRUCT;
    map_iter_t it2 = CDC_INIT_STRUCT;

    CU_ASSERT_EQUAL(map_iter_ctor(m, &it1), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(map_iter_ctor(m, &it2), CDC_STATUS_OK);

    pair_t *arr[] = {&a, &b, &c, &d, &e, &f, &g, &h};

    size_t i = 0;
    map_begin(m, &it1);
    map_end(m, &it2);
    for (; !map_iter_is_eq(&it1, &it2); map_iter_next(&it1)) {
      CU_ASSERT_EQUAL(map_iter_key(&it1), arr[i]->first)
      ++i;
    }
    CU_ASSERT_EQUAL(map_size(m), i);

    if (map_iter_type(&it1) == CDC_BIDIR_ITERATOR) {
      i = map_size(m) - 1;
      map_end(m, &it1);
      map_iter_prev(&it1);
      map_begin(m, &it2);
      for (; !map_iter_is_eq(&it1, &it2); map_iter_prev(&it1)) {
        CU_ASSERT_EQUAL(map_iter_key(&it1), arr[i]->first)
        --i;
      }
      CU_ASSERT_EQUAL(i, 0);
    }

    i = 0;
    map_begin(m, &it1);
    while (map_iter_has_next(&it1)) {
      CU_ASSERT_EQUAL(map_iter_key(&it1), arr[i]->first)
      ++i;
      map_iter_next(&it1);
    }
    if (map_size(m) != i) printf("h");
    CU_ASSERT_EQUAL(map_size(m), i);

    if (map_iter_type(&it1) == CDC_BIDIR_ITERATOR) {
      i = map_size(m) - 1;
      map_end(m, &it1);
      map_iter_prev(&it1);
      while (map_iter_has_prev(&it1)) {
        CU_ASSERT_EQUAL(map_iter_key(&it1), arr[i]->first);
        --i;
        map_iter_prev(&it1);
      }
      CU_ASSERT_EQUAL(i, 0);
    }

    map_iter_dtor(&it1);
    map_iter_dtor(&it2);
    map_dtor(m);
  }
}

void test_map_iter_type()
{
  const map_table_t *tables[] = {cdc_map_avl, cdc_map_splay, cdc_map_treap, cdc_map_htable};
  const iterator_type_t answers[] = {CDC_BIDIR_ITERATOR, CDC_BIDIR_ITERATOR, CDC_BIDIR_ITERATOR,
                                     CDC_FWD_ITERATOR};
  CU_ASSERT_EQUAL(CDC_ARRAY_SIZE(tables), CDC_ARRAY_SIZE(answers));
  for (size_t t = 0; t < CDC_ARRAY_SIZE(tables); ++t) {
    map_t *m = NULL;
    map_iter_t it = CDC_INIT_STRUCT;
    data_info_t info = CDC_INIT_STRUCT;
    info.cmp = lt;
    info.eq = eq;
    info.hash = hash;

    map_ctor(tables[t], &m, &info);
    map_iter_ctor(m, &it);
    CU_ASSERT_EQUAL(map_iter_type(&it), answers[t]);
    map_iter_dtor(&it);
    map_dtor(m);
  }
}
