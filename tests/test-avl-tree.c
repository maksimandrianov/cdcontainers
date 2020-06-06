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

#include "cdcontainers/avl-tree.h"
#include "cdcontainers/casts.h"
#include "cdcontainers/common.h"
#include "cdcontainers/tree-utils.h"

#include <float.h>
#include <math.h>
#include <stdarg.h>

#include <CUnit/Basic.h>

CDC_MAKE_TREE_HEIGTH_FN(avl_tree_node_t *)

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

static inline void test_tree_links(avl_tree_node_t *node)
{
  if (!node) return;

  if (node->left) {
    CU_ASSERT_EQUAL(node->left->parent, node);
    test_tree_links(node->left);
  }

  if (node->right) {
    CU_ASSERT_EQUAL(node->right->parent, node);
    test_tree_links(node->right);
  }
}

static bool avl_tree_key_int_eq(avl_tree_t *t, size_t count, ...)
{
  test_tree_links(t->root);
  va_list args;
  va_start(args, count);
  for (size_t i = 0; i < count; ++i) {
    pair_t *val = va_arg(args, pair_t *);
    void *tmp = NULL;
    if (avl_tree_get(t, val->first, &tmp) != CDC_STATUS_OK || tmp != val->second) {
      va_end(args);
      return false;
    }
  }
  va_end(args);
  CU_ASSERT_EQUAL(avl_tree_size(t), count);
  return true;
}

static inline void avl_tree_inorder_print_int(avl_tree_node_t *node)
{
  if (node->left) {
    avl_tree_inorder_print_int(node->left);
  }

  printf("%d ", CDC_TO_INT(node->key));

  if (node->right) {
    avl_tree_inorder_print_int(node->right);
  }
}

void test_avl_tree_ctor()
{
  avl_tree_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctor(&t, &info), CDC_STATUS_OK);
  CU_ASSERT(avl_tree_empty(t));
  avl_tree_dtor(t);
}

void test_avl_tree_ctorl()
{
  avl_tree_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctorl(&t, &info, &a, &g, &h, &d, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 4);
  CU_ASSERT(avl_tree_key_int_eq(t, 4, &a, &g, &h, &d));
  avl_tree_dtor(t);
}

void test_avl_tree_get()
{
  avl_tree_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctorl(&t, &info, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 8);
  CU_ASSERT(avl_tree_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));

  void *value = NULL;
  CU_ASSERT_EQUAL(avl_tree_get(t, CDC_FROM_INT(10), &value), CDC_STATUS_NOT_FOUND);
  avl_tree_dtor(t);
}

void test_avl_tree_count()
{
  avl_tree_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctorl(&t, &info, &a, &b, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 2);
  CU_ASSERT_EQUAL(avl_tree_count(t, a.first), 1);
  CU_ASSERT_EQUAL(avl_tree_count(t, b.first), 1);
  CU_ASSERT_EQUAL(avl_tree_count(t, CDC_FROM_INT(10)), 0);
  avl_tree_dtor(t);
}

void test_avl_tree_find()
{
  avl_tree_t *t = NULL;
  avl_tree_iter_t it = CDC_INIT_STRUCT;
  avl_tree_iter_t it_end = CDC_INIT_STRUCT;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctorl(&t, &info, &a, &b, &c, &d, &g, CDC_END), CDC_STATUS_OK);
  avl_tree_find(t, a.first, &it);
  CU_ASSERT_EQUAL(avl_tree_iter_value(&it), a.second);
  avl_tree_find(t, b.first, &it);
  CU_ASSERT_EQUAL(avl_tree_iter_value(&it), b.second);
  avl_tree_find(t, g.first, &it);
  CU_ASSERT_EQUAL(avl_tree_iter_value(&it), g.second);
  avl_tree_find(t, h.first, &it);
  avl_tree_end(t, &it_end);
  CU_ASSERT(avl_tree_iter_is_eq(&it, &it_end));
  avl_tree_dtor(t);
}

void test_avl_tree_clear()
{
  avl_tree_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctorl(&t, &info, &a, &b, CDC_END), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 2);
  avl_tree_clear(t);
  CU_ASSERT(avl_tree_empty(t));
  avl_tree_clear(t);
  CU_ASSERT(avl_tree_empty(t));
  avl_tree_dtor(t);
}

void test_avl_tree_insert()
{
  avl_tree_t *t = NULL;
  const int kCount = 100;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctor(&t, &info), CDC_STATUS_OK);

  for (int i = 0; i < kCount; ++i) {
    CU_ASSERT_EQUAL(avl_tree_insert(t, CDC_FROM_INT(i), CDC_FROM_INT(i), NULL), CDC_STATUS_OK);
  }

  CU_ASSERT_EQUAL(avl_tree_size(t), kCount);

  for (int i = 0; i < kCount; ++i) {
    void *val = NULL;
    CU_ASSERT_EQUAL(avl_tree_get(t, CDC_FROM_INT(i), &val), CDC_STATUS_OK);
    CU_ASSERT_EQUAL(CDC_TO_INT(val), i);
  }

  avl_tree_dtor(t);
}

void test_avl_tree_insert_or_assign()
{
  avl_tree_t *t = NULL;
  pair_avl_tree_iter_bool_t ret = CDC_INIT_STRUCT;
  void *value = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctor(&t, &info), CDC_STATUS_OK);

  CU_ASSERT_EQUAL(avl_tree_insert_or_assign(t, a.first, a.second, &ret), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 1);
  CU_ASSERT_EQUAL(avl_tree_iter_value(&ret.first), a.second);
  CU_ASSERT(ret.second);

  CU_ASSERT_EQUAL(avl_tree_insert_or_assign(t, a.first, b.second, &ret), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 1);
  CU_ASSERT_EQUAL(avl_tree_get(t, a.first, &value), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(value, b.second);
  CU_ASSERT_EQUAL(avl_tree_iter_value(&ret.first), b.second);
  CU_ASSERT(!ret.second);

  CU_ASSERT_EQUAL(avl_tree_insert_or_assign(t, c.first, c.second, &ret), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 2);
  CU_ASSERT_EQUAL(avl_tree_iter_value(&ret.first), c.second);
  CU_ASSERT(ret.second);

  CU_ASSERT_EQUAL(avl_tree_insert_or_assign(t, c.first, d.second, &ret), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 2);
  CU_ASSERT_EQUAL(avl_tree_get(t, c.first, &value), CDC_STATUS_OK);
  CU_ASSERT_EQUAL(value, d.second);
  CU_ASSERT_EQUAL(avl_tree_iter_value(&ret.first), d.second);
  CU_ASSERT(!ret.second);
  avl_tree_dtor(t);
}

void test_avl_tree_erase()
{
  avl_tree_t *t = NULL;
  void *value = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctorl(&t, &info, &a, &b, &c, &d, &g, &h, &e, &f, CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 8);
  avl_tree_inorder_print_int(t->root);
  CU_ASSERT(avl_tree_key_int_eq(t, 8, &a, &b, &c, &d, &g, &h, &e, &f));
  CU_ASSERT_EQUAL(avl_tree_erase(t, a.first), 1);
  avl_tree_inorder_print_int(t->root);
  CU_ASSERT_EQUAL(avl_tree_get(t, a.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT(avl_tree_key_int_eq(t, 7, &b, &c, &d, &g, &h, &e, &f));

  CU_ASSERT_EQUAL(avl_tree_erase(t, h.first), 1);
  CU_ASSERT_EQUAL(avl_tree_get(t, h.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(avl_tree_size(t), 6);
  CU_ASSERT(avl_tree_key_int_eq(t, 6, &b, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(avl_tree_erase(t, h.first), 0);
  CU_ASSERT_EQUAL(avl_tree_size(t), 6);
  CU_ASSERT(avl_tree_key_int_eq(t, 6, &b, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(avl_tree_erase(t, b.first), 1);
  CU_ASSERT_EQUAL(avl_tree_get(t, b.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(avl_tree_size(t), 5);
  CU_ASSERT(avl_tree_key_int_eq(t, 5, &c, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(avl_tree_erase(t, c.first), 1);
  CU_ASSERT_EQUAL(avl_tree_get(t, c.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(avl_tree_size(t), 4);
  CU_ASSERT(avl_tree_key_int_eq(t, 4, &d, &g, &e, &f));

  CU_ASSERT_EQUAL(avl_tree_erase(t, d.first), 1);
  CU_ASSERT_EQUAL(avl_tree_get(t, d.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(avl_tree_size(t), 3);
  CU_ASSERT(avl_tree_key_int_eq(t, 3, &g, &e, &f));

  CU_ASSERT_EQUAL(avl_tree_erase(t, g.first), 1);
  CU_ASSERT_EQUAL(avl_tree_get(t, g.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(avl_tree_size(t), 2);
  CU_ASSERT(avl_tree_key_int_eq(t, 2, &e, &f));

  CU_ASSERT_EQUAL(avl_tree_erase(t, f.first), 1);
  CU_ASSERT_EQUAL(avl_tree_get(t, f.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT_EQUAL(avl_tree_size(t), 1);
  CU_ASSERT(avl_tree_key_int_eq(t, 1, &e));

  CU_ASSERT_EQUAL(avl_tree_erase(t, e.first), 1);
  CU_ASSERT_EQUAL(avl_tree_get(t, e.first, &value), CDC_STATUS_NOT_FOUND);
  CU_ASSERT(avl_tree_empty(t));
  avl_tree_dtor(t);
}

void test_avl_tree_iterators()
{
  avl_tree_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  CU_ASSERT_EQUAL(avl_tree_ctorl(&t, &info, &a, &b, &c, &d, &e, &f, &g, &h, CDC_END),
                  CDC_STATUS_OK);
  CU_ASSERT_EQUAL(avl_tree_size(t), 8);

  avl_tree_iter_t it1 = CDC_INIT_STRUCT;
  avl_tree_iter_t it2 = CDC_INIT_STRUCT;
  pair_t *arr[] = {&a, &b, &c, &d, &e, &f, &g, &h};

  size_t i = 0;
  avl_tree_begin(t, &it1);
  avl_tree_end(t, &it2);
  for (; !avl_tree_iter_is_eq(&it1, &it2); avl_tree_iter_next(&it1)) {
    CU_ASSERT_EQUAL(avl_tree_iter_key(&it1), arr[i]->first)
    ++i;
  }
  CU_ASSERT_EQUAL(avl_tree_size(t), i);

  i = avl_tree_size(t) - 1;
  avl_tree_end(t, &it1);
  avl_tree_iter_prev(&it1);
  avl_tree_begin(t, &it2);
  for (; !avl_tree_iter_is_eq(&it1, &it2); avl_tree_iter_prev(&it1)) {
    CU_ASSERT_EQUAL(avl_tree_iter_key(&it1), arr[i]->first)
    --i;
  }
  CU_ASSERT_EQUAL(i, 0);

  avl_tree_begin(t, &it1);
  while (avl_tree_iter_has_next(&it1)) {
    CU_ASSERT_EQUAL(avl_tree_iter_key(&it1), arr[i]->first)
    ++i;
    avl_tree_iter_next(&it1);
  }
  CU_ASSERT_EQUAL(avl_tree_size(t), i);

  i = avl_tree_size(t) - 1;
  avl_tree_end(t, &it1);
  avl_tree_iter_prev(&it1);
  while (avl_tree_iter_has_prev(&it1)) {
    CU_ASSERT_EQUAL(avl_tree_iter_key(&it1), arr[i]->first);
    --i;
    avl_tree_iter_prev(&it1);
  }
  CU_ASSERT_EQUAL(i, 0);
  avl_tree_dtor(t);
}

void test_avl_tree_swap()
{
}

void test_avl_tree_height()
{
  size_t count = 100000;
  avl_tree_t *t = NULL;
  data_info_t info = CDC_INIT_STRUCT;
  info.cmp = lt;

  avl_tree_ctor(&t, &info);
  for (size_t i = 0; i < count; ++i) {
    int val = rand();
    if (avl_tree_insert(t, CDC_FROM_INT(val), NULL, NULL) != CDC_STATUS_OK) {
      CU_ASSERT(true);
    }
  }

  double experimental_height = cdc_tree_height(t->root);
  double theoretical_max_height = 1.44 * log2((double)count);
  printf(
      "\nExperimental avl tree heigth: %f, theoretical max avl tree heigth: "
      "%f, tree size: %zu\n",
      experimental_height, theoretical_max_height, count);
  CU_ASSERT(experimental_height <= theoretical_max_height);
  avl_tree_dtor(t);
}
