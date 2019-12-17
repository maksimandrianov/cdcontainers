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

#include "cdcontainers/casts.h"
#include "cdcontainers/treap.h"
#include "cdcontainers/tree-utils.h"

#include <float.h>
#include <math.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

CDC_MAKE_TREE_HEIGTH_FN(struct cdc_treap_node *)

static struct cdc_pair a = {CDC_FROM_INT(0), CDC_FROM_INT(0)};
static struct cdc_pair b = {CDC_FROM_INT(1), CDC_FROM_INT(1)};
static struct cdc_pair c = {CDC_FROM_INT(2), CDC_FROM_INT(2)};
static struct cdc_pair d = {CDC_FROM_INT(3), CDC_FROM_INT(3)};
static struct cdc_pair e = {CDC_FROM_INT(4), CDC_FROM_INT(4)};
static struct cdc_pair f = {CDC_FROM_INT(5), CDC_FROM_INT(5)};
static struct cdc_pair g = {CDC_FROM_INT(6), CDC_FROM_INT(6)};
static struct cdc_pair h = {CDC_FROM_INT(7), CDC_FROM_INT(7)};

static int lt(const void *l, const void *r)
{
  return CDC_TO_INT(l) < CDC_TO_INT(r);
}

static bool treap_key_int_eq(struct cdc_treap *t, size_t count, ...)
{
  va_list args;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    struct cdc_pair *val = va_arg(args, struct cdc_pair *);
    void *tmp = NULL;
    if (cdc_treap_get(t, val->first, &tmp) != CDC_STATUS_OK ||
        tmp != val->second) {
      va_end(args);
      return false;
    }
  }

  va_end(args);
  return true;
}

static inline void treap_inorder_print_int(struct cdc_treap_node *node)
{
  if (node->left) {
    treap_inorder_print_int(node->left);
  }

  printf("%d ", CDC_TO_INT(node->key));

  if (node->right) {
    treap_inorder_print_int(node->right);
  }
}

void test_treap_ctor()
{
  struct cdc_treap *t = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(cdc_treap_ctor(&t, &info), CDC_STATUS_OK);
  CU_ASSERT(cdc_treap_empty(t));
  cdc_treap_dtor(t);
}

void test_treap_ctorl()
{
  struct cdc_treap *t = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(cdc_treap_ctorl(&t, &info, &a, &g, &h, &d, CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 4);
  CU_ASSERT(treap_key_int_eq(t, 4, &a, &g, &h, &d));
  cdc_treap_dtor(t);
}

void test_treap_get()
{
  struct cdc_treap *t = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(
      cdc_treap_ctorl(&t, &info, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 8);
  CU_ASSERT(treap_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));

  void *value = NULL;
  CU_ASSERT_EQUAL(cdc_treap_get(t, CDC_FROM_INT(10), &value),
                  CDC_STATUS_NOT_FOUND);
  cdc_treap_dtor(t);
}

void test_treap_count()
{
  struct cdc_treap *t = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(cdc_treap_ctorl(&t, &info, &a, &b, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 2);
  CU_ASSERT_EQUAL(cdc_treap_count(t, a.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_count(t, b.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_count(t, CDC_FROM_INT(10)), 0);
  cdc_treap_dtor(t);
}

void test_treap_find()
{
  struct cdc_treap *t = NULL;
  struct cdc_treap_iter it = CDC_INIT_STRUCT;
  struct cdc_treap_iter it_end = CDC_INIT_STRUCT;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(cdc_treap_ctorl(&t, &info, &a, &b, &c, &d, &g, CDC_END),
                  CDC_STATUS_OK);
  cdc_treap_find(t, a.first, &it);
  CU_ASSERT_EQUAL(cdc_treap_iter_value(&it), a.second);
  cdc_treap_find(t, b.first, &it);
  CU_ASSERT_EQUAL(cdc_treap_iter_value(&it), b.second);
  cdc_treap_find(t, g.first, &it);
  CU_ASSERT_EQUAL(cdc_treap_iter_value(&it), g.second);
  cdc_treap_find(t, h.first, &it);
  cdc_treap_end(t, &it_end);
  CU_ASSERT(cdc_treap_iter_is_eq(&it, &it_end));
  cdc_treap_dtor(t);
}

void test_treap_clear()
{
  struct cdc_treap *t = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(cdc_treap_ctorl(&t, &info, &a, &b, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 2);
  cdc_treap_clear(t);
  CU_ASSERT(cdc_treap_empty(t));
  cdc_treap_clear(t);
  CU_ASSERT(cdc_treap_empty(t));
  cdc_treap_dtor(t);
}

void test_treap_insert()
{
  struct cdc_treap *t = NULL;
  const int kCount = 100;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(cdc_treap_ctor(&t, &info), CDC_STATUS_OK);

  for (int i = 0; i < kCount; ++i) {
    CU_ASSERT_EQUAL(cdc_treap_insert(t, CDC_FROM_INT(i), CDC_FROM_INT(i), NULL),
                    CDC_STATUS_OK);
  }

  CU_ASSERT_EQUAL(cdc_treap_size(t), kCount);

  for (int i = 0; i < kCount; ++i) {
    void *val = NULL;
    CU_ASSERT_EQUAL(cdc_treap_get(t, CDC_FROM_INT(i), &val), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(val), i);
  }
  cdc_treap_dtor(t);
}

void test_treap_insert_or_assign()
{
  struct cdc_treap *t = NULL;
  struct cdc_pair_treap_iter_bool ret = CDC_INIT_STRUCT;
  void *value = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(cdc_treap_ctor(&t, &info), CDC_STATUS_OK);

  CU_ASSERT_EQUAL(cdc_treap_insert_or_assign(t, a.first, a.second, &ret),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 1);
  CU_ASSERT_EQUAL(cdc_treap_iter_value(&ret.first), a.second);
  CU_ASSERT(ret.second);

  CU_ASSERT_EQUAL(cdc_treap_insert_or_assign(t, a.first, b.second, &ret),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 1);
  CU_ASSERT_EQUAL(cdc_treap_get(t, a.first, &value), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(value, b.second);
  CU_ASSERT_EQUAL(cdc_treap_iter_value(&ret.first), b.second);
  CU_ASSERT(!ret.second);

  CU_ASSERT_EQUAL(cdc_treap_insert_or_assign(t, c.first, c.second, &ret),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 2);
  CU_ASSERT_EQUAL(cdc_treap_iter_value(&ret.first), c.second);
  CU_ASSERT(ret.second);

  CU_ASSERT_EQUAL(cdc_treap_insert_or_assign(t, c.first, d.second, &ret),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 2);
  CU_ASSERT_EQUAL(cdc_treap_get(t, c.first, &value), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(value, d.second);
  CU_ASSERT_EQUAL(cdc_treap_iter_value(&ret.first), d.second);
  CU_ASSERT(!ret.second);
  cdc_treap_dtor(t);
}

void test_treap_erase()
{
  struct cdc_treap *t = NULL;
  void *value = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(
      cdc_treap_ctorl(&t, &info, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 8);
  CU_ASSERT(treap_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));
  CU_ASSERT_EQUAL(cdc_treap_erase(t, a.first), 1);

  CU_ASSERT_EQUAL(cdc_treap_get(t, a.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT(treap_key_int_eq(t, 7, &b, &c, &d, &g, &h, &e, &f));

  CU_ASSERT_EQUAL(cdc_treap_erase(t, h.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_get(t, h.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 6);
  CU_ASSERT(treap_key_int_eq(t, 6, &b, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(cdc_treap_erase(t, h.first), 0);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 6);
  CU_ASSERT(treap_key_int_eq(t, 6, &b, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(cdc_treap_erase(t, b.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_get(t, b.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 5);
  CU_ASSERT(treap_key_int_eq(t, 5, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(cdc_treap_erase(t, c.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_get(t, c.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 4);
  CU_ASSERT(treap_key_int_eq(t, 4, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(cdc_treap_erase(t, d.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_get(t, d.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 3);
  CU_ASSERT(treap_key_int_eq(t, 3, &g, &e, &f));

  CU_ASSERT_EQUAL(cdc_treap_erase(t, f.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_get(t, f.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 2);
  CU_ASSERT(treap_key_int_eq(t, 2, &g, &e));

  CU_ASSERT_EQUAL(cdc_treap_erase(t, e.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_get(t, e.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 1);
  CU_ASSERT(treap_key_int_eq(t, 1, &g));

  CU_ASSERT_EQUAL(cdc_treap_erase(t, g.first), 1);
  CU_ASSERT_EQUAL(cdc_treap_get(t, g.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT(cdc_treap_empty(t));
  cdc_treap_dtor(t);
}

void test_treap_iterators()
{
  struct cdc_treap *t = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(
      cdc_treap_ctorl(&t, &info, &a, &b, &c, &d, &e, &f, &g, &h, CDC_END),
      CDC_STATUS_OK);
  CU_ASSERT_EQUAL(cdc_treap_size(t), 8);

  struct cdc_treap_iter it1 = CDC_INIT_STRUCT;
  struct cdc_treap_iter it2 = CDC_INIT_STRUCT;
  struct cdc_pair *arr[] = {&a, &b, &c, &d, &e, &f, &g, &h};

  size_t i = 0;
  cdc_treap_begin(t, &it1);
  cdc_treap_end(t, &it2);
  for (; !cdc_treap_iter_is_eq(&it1, &it2); cdc_treap_iter_next(&it1)) {
    CU_ASSERT_EQUAL(cdc_treap_iter_key(&it1), arr[i]->first)
    ++i;
  }
  CU_ASSERT_EQUAL(cdc_treap_size(t), i);

  i = cdc_treap_size(t) - 1;
  cdc_treap_end(t, &it1);
  cdc_treap_iter_prev(&it1);
  cdc_treap_begin(t, &it2);
  for (; !cdc_treap_iter_is_eq(&it1, &it2); cdc_treap_iter_prev(&it1)) {
    CU_ASSERT_EQUAL(cdc_treap_iter_key(&it1), arr[i]->first)
    --i;
  }
  CU_ASSERT_EQUAL(i, 0);

  cdc_treap_begin(t, &it1);
  while (cdc_treap_iter_has_next(&it1)) {
    CU_ASSERT_EQUAL(cdc_treap_iter_key(&it1), arr[i]->first)
    ++i;
    cdc_treap_iter_next(&it1);
  }
  CU_ASSERT_EQUAL(cdc_treap_size(t), i);

  i = cdc_treap_size(t) - 1;
  cdc_treap_end(t, &it1);
  cdc_treap_iter_prev(&it1);
  while (cdc_treap_iter_has_prev(&it1)) {
    CU_ASSERT_EQUAL(cdc_treap_iter_key(&it1), arr[i]->first);
    --i;
    cdc_treap_iter_prev(&it1);
  }
  CU_ASSERT_EQUAL(i, 0);
  cdc_treap_dtor(t);
}

void test_treap_swap() {}

void test_treap_height()
{
  size_t count = 100000;
  struct cdc_treap *t = NULL;
  struct cdc_data_info info = CDC_INIT_STRUCT;
  info.cmp = lt;

  cdc_treap_ctor(&t, &info);
  for (size_t i = 0; i < count; ++i) {
    int val = rand();
    if (cdc_treap_insert(t, CDC_FROM_INT(val), NULL, NULL) != CDC_STATUS_OK) {
      CU_ASSERT(true);
    }
  }

  double experimental_height = cdc_tree_height(t->root);
  printf("\nExperimental treap heigth: %f, tree size: %zu\n",
         experimental_height, count);
  cdc_treap_dtor(t);
}
