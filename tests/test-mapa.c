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
#include <cdcontainers/map.h>
#include <cdcontainers/casts.h>

static struct cdc_pair a = {CDC_INT_TO_PTR(0), CDC_INT_TO_PTR(0)};
static struct cdc_pair b = {CDC_INT_TO_PTR(1), CDC_INT_TO_PTR(1)};
static struct cdc_pair c = {CDC_INT_TO_PTR(2), CDC_INT_TO_PTR(2)};
static struct cdc_pair d = {CDC_INT_TO_PTR(3), CDC_INT_TO_PTR(3)};
static struct cdc_pair e = {CDC_INT_TO_PTR(4), CDC_INT_TO_PTR(4)};
static struct cdc_pair f = {CDC_INT_TO_PTR(5), CDC_INT_TO_PTR(5)};
static struct cdc_pair g = {CDC_INT_TO_PTR(6), CDC_INT_TO_PTR(6)};
static struct cdc_pair h = {CDC_INT_TO_PTR(7), CDC_INT_TO_PTR(7)};

static int lt_int(const void *l, const void *r)
{
  return l < r;
}

static bool map_key_int_eq(struct cdc_map *m, size_t count, ...)
{
  va_list args;
  size_t i;
  struct cdc_pair *val;
  void *tmp;

  va_start(args, count);
  for (i = 0; i < count; ++i) {
    val = va_arg(args, struct cdc_pair *);
    if (cdc_map_get(m, val->first, &tmp) != CDC_STATUS_OK ||
        tmp != val->second) {
      va_end(args);
      return false;
    }
  }

  va_end(args);
  return true;
}

void test_map_ctor()
{
  struct cdc_map *m;

  CU_ASSERT(cdc_mapa_ctor1(&m, NULL, lt_int) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 0);

  cdc_map_dtor(m);
}

void test_map_ctorl()
{
  struct cdc_map *m;

  CU_ASSERT(cdc_mapa_ctorl1(&m, NULL, lt_int, &a, &g, &h, &d, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 4);
  CU_ASSERT(map_key_int_eq(m, 4, &a, &g, &h, &d));

  cdc_map_dtor(m);
}

void test_map_get()
{
  struct cdc_map *m = NULL;
  void *value;

  CU_ASSERT(cdc_mapa_ctorl1(&m, NULL, lt_int,
                            &a, &b, &c, &d, &g, &h, &e, &f, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 8);
  CU_ASSERT(map_key_int_eq(m, 8, &a, &b, &c, &d, &g, &h, &e, &f));
  CU_ASSERT(cdc_map_get(m, CDC_INT_TO_PTR(10), &value) == CDC_STATUS_NOT_FOUND);
  cdc_map_dtor(m);
}

void test_map_count()
{
  struct cdc_map *m = NULL;

  CU_ASSERT(cdc_mapa_ctorl1(&m, NULL, lt_int, &a, &b, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 2);
  CU_ASSERT(cdc_map_count(m, a.first) == 1);
  CU_ASSERT(cdc_map_count(m, b.first) == 1);
  CU_ASSERT(cdc_map_count(m, CDC_INT_TO_PTR(10)) == 0);

  cdc_map_dtor(m);
}

void test_map_find()
{
  struct cdc_map *m = NULL;
  struct cdc_map_iter it, it_end;

  CU_ASSERT(cdc_mapa_ctorl1(&m, NULL, lt_int, &a, &b, &c, &d, &g, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_iter_init(m, &it) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_iter_init(m, &it_end) == CDC_STATUS_OK);

  cdc_map_find(m, a.first, &it);
  CU_ASSERT(cdc_map_iter_value(&it) == a.second);
  cdc_map_find(m, b.first, &it);
  CU_ASSERT(cdc_map_iter_value(&it) == b.second);
  cdc_map_find(m, g.first, &it);
  CU_ASSERT(cdc_map_iter_value(&it) == g.second);
  cdc_map_find(m, h.first, &it);
  cdc_map_end(m, &it_end);
  CU_ASSERT(cdc_map_iter_is_eq(&it, &it_end));

  cdc_map_iter_free(&it);
  cdc_map_iter_free(&it_end);
  cdc_map_dtor(m);
}

void test_map_clear()
{
  struct cdc_map *m = NULL;

  CU_ASSERT(cdc_mapa_ctorl1(&m, NULL, lt_int, &a, &b, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 2);
  cdc_map_clear(m);
  CU_ASSERT(cdc_map_size(m) == 0);
  cdc_map_clear(m);
  CU_ASSERT(cdc_map_size(m) == 0);

  cdc_map_dtor(m);
}

void test_map_insert()
{
  struct cdc_map *m;
  size_t i, count = 100;
  bool failed = false;
  void *val;

  CU_ASSERT(cdc_mapa_ctor1(&m, NULL, lt_int) == CDC_STATUS_OK);

  for (i = 0; i < count; ++i) {
    if (cdc_map_insert(m, CDC_INT_TO_PTR(i), CDC_INT_TO_PTR(i), NULL,
                       NULL) != CDC_STATUS_OK) {
      failed = true;
      break;
    }
  }

  CU_ASSERT(cdc_map_size(m) == count);
  CU_ASSERT(failed == false);

  for (i = 0; i < count; ++i) {
    if (cdc_map_get(m, CDC_INT_TO_PTR(i), &val) != CDC_STATUS_OK &&
        val != CDC_INT_TO_PTR(i)) {
      failed = true;
      break;
    }
  }

  CU_ASSERT(failed == false);

  cdc_map_dtor(m);
}

void test_map_insert_or_assign()
{
  struct cdc_map *m = NULL;
  struct cdc_map_iter it;
  bool inserted;
  void *value;

  CU_ASSERT(cdc_mapa_ctor1(&m, NULL, lt_int) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_iter_init(m, &it) == CDC_STATUS_OK);

  CU_ASSERT(cdc_map_insert_or_assign(m, a.first, a.second, &it, &inserted) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 1);
  CU_ASSERT(cdc_map_iter_value(&it) == a.second);
  CU_ASSERT(inserted == true);

  CU_ASSERT(cdc_map_insert_or_assign(m, a.first, b.second, &it, &inserted) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 1);
  CU_ASSERT(cdc_map_get(m, a.first, &value) == CDC_STATUS_OK);
  CU_ASSERT(value == b.second);
  CU_ASSERT(cdc_map_iter_value(&it) == b.second);
  CU_ASSERT(inserted == false);

  CU_ASSERT(cdc_map_insert_or_assign(m, c.first, c.second, &it, &inserted) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 2);
  CU_ASSERT(cdc_map_iter_value(&it) == c.second);
  CU_ASSERT(inserted == true);

  CU_ASSERT(cdc_map_insert_or_assign(m, c.first, d.second, &it, &inserted) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 2);
  CU_ASSERT(cdc_map_get(m, c.first, &value) == CDC_STATUS_OK);
  CU_ASSERT(value == d.second);
  CU_ASSERT(cdc_map_iter_value(&it) == d.second);
  CU_ASSERT(inserted == false);

  cdc_map_iter_free(&it);
  cdc_map_dtor(m);
}

void test_map_erase()
{
  struct cdc_map *m = NULL;
  void *value;

  CU_ASSERT(cdc_mapa_ctorl1(&m, NULL, lt_int,
                            &a, &b, &c, &d, &g, &h, &e, &f, NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_size(m) == 8);
  CU_ASSERT(map_key_int_eq(m, 8, &a, &b, &c, &d, &g, &h, &e, &f));
  CU_ASSERT(cdc_map_erase(m, a.first) == 1);

  CU_ASSERT(cdc_map_get(m, a.first, &value) == CDC_STATUS_NOT_FOUND);
  CU_ASSERT(map_key_int_eq(m, 7, &b, &c, &d, &g, &h, &e, &f));

  CU_ASSERT(cdc_map_erase(m, h.first) == 1);
  CU_ASSERT(cdc_map_get(m, h.first, &value) == CDC_STATUS_NOT_FOUND);
  CU_ASSERT(cdc_map_size(m) == 6);
  CU_ASSERT(map_key_int_eq(m, 6, &b, &c, &d, &g, &e, &f));

  CU_ASSERT(cdc_map_erase(m, h.first) == 0);
  CU_ASSERT(cdc_map_size(m) == 6);
  CU_ASSERT(map_key_int_eq(m, 6, &b, &c, &d, &g, &e, &f));

  CU_ASSERT(cdc_map_erase(m, b.first) == 1);
  CU_ASSERT(cdc_map_get(m, b.first, &value) == CDC_STATUS_NOT_FOUND);
  CU_ASSERT(cdc_map_size(m) == 5);
  CU_ASSERT(map_key_int_eq(m, 5, &c, &d, &g, &e, &f));

  CU_ASSERT(cdc_map_erase(m, c.first) == 1);
  CU_ASSERT(cdc_map_get(m, c.first, &value) == CDC_STATUS_NOT_FOUND);
  CU_ASSERT(cdc_map_size(m) == 4);
  CU_ASSERT(map_key_int_eq(m, 4, &d, &g, &e, &f));

  CU_ASSERT(cdc_map_erase(m, d.first) == 1);
  CU_ASSERT(cdc_map_get(m, d.first, &value) == CDC_STATUS_NOT_FOUND);
  CU_ASSERT(cdc_map_size(m) == 3);
  CU_ASSERT(map_key_int_eq(m, 3, &g, &e, &f));

  CU_ASSERT(cdc_map_erase(m, f.first) == 1);
  CU_ASSERT(cdc_map_get(m, f.first, &value) == CDC_STATUS_NOT_FOUND);
  CU_ASSERT(cdc_map_size(m) == 2);
  CU_ASSERT(map_key_int_eq(m, 2, &g, &e));

  CU_ASSERT(cdc_map_erase(m, e.first) == 1);
  CU_ASSERT(cdc_map_get(m, e.first, &value) == CDC_STATUS_NOT_FOUND);
  CU_ASSERT(cdc_map_size(m) == 1);
  CU_ASSERT(map_key_int_eq(m, 1, &g));

  CU_ASSERT(cdc_map_erase(m, g.first) == 1);
  CU_ASSERT(cdc_map_get(m, g.first, &value) == CDC_STATUS_NOT_FOUND);
  CU_ASSERT(cdc_map_size(m) == 0);

  cdc_map_dtor(m);
}

void test_map_iterators()
{
  struct cdc_map *m;
  struct cdc_map_iter it1, it2;
  struct cdc_pair *arr[] = {&a, &b, &c, &d, &e, &f, &g, &h};
  bool check;
  size_t i;

  CU_ASSERT(cdc_mapa_ctorl1(&m, NULL, lt_int, &a, &b, &c, &d, &e, &f, &g, &h,
                            NULL) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_iter_init(m, &it1) == CDC_STATUS_OK);
  CU_ASSERT(cdc_map_iter_init(m, &it2) == CDC_STATUS_OK);

  CU_ASSERT(cdc_map_size(m) == 8);

  check = true;
  i = 0;
  cdc_map_begin(m, &it1);
  cdc_map_end(m, &it2);
  for ( ; !cdc_map_iter_is_eq(&it1, &it2); cdc_map_iter_next(&it1)) {
    if (cdc_map_iter_key(&it1) != arr[i]->first) {
      check = false;
      break;
    }
    ++i;
  }
  CU_ASSERT(check == true);
  CU_ASSERT(i == cdc_map_size(m));

  check = true;
  i = cdc_map_size(m) - 1;
  cdc_map_end(m, &it1);
  cdc_map_iter_prev(&it1);
  cdc_map_begin(m, &it2);
  for ( ; !cdc_map_iter_is_eq(&it1, &it2); cdc_map_iter_prev(&it1)) {
    if (cdc_map_iter_key(&it1) != arr[i]->first) {
      check = false;
      break;
    }
    --i;
  }
  CU_ASSERT(check == true);
  CU_ASSERT(i == 0);

  check = true;
  i = 0;
  cdc_map_begin(m, &it1);
  while (cdc_map_iter_has_next(&it1)) {
    if (cdc_map_iter_key(&it1) != arr[i]->first) {
      check = false;
      break;
    }
    ++i;
    cdc_map_iter_next(&it1);
  }
  CU_ASSERT(check == true);
  CU_ASSERT(i == cdc_map_size(m));

  check = true;
  i = cdc_map_size(m) - 1;
  cdc_map_end(m, &it1);
  cdc_map_iter_prev(&it1);
  while (cdc_map_iter_has_prev(&it1)) {
    if (cdc_map_iter_key(&it1) != arr[i]->first) {
      check = false;
      break;
    }
    --i;
    cdc_map_iter_prev(&it1);
  }
  CU_ASSERT(check == true);
  CU_ASSERT(i == 0);

  cdc_map_iter_free(&it1);
  cdc_map_iter_free(&it2);
  cdc_map_dtor(m);
}

void test_map_swap()
{

}
