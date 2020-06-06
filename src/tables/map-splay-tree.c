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
#include "cdcontainers/splay-tree.h"
#include "cdcontainers/tables/imap.h"

#include <assert.h>
#include <stdlib.h>

static stat_t ctor(void **cntr, data_info_t *info)
{
  assert(cntr != NULL);

  splay_tree_t **tree = (splay_tree_t **)cntr;
  return splay_tree_ctor(tree, info);
}

static stat_t ctorv(void **cntr, data_info_t *info, va_list args)
{
  assert(cntr != NULL);

  splay_tree_t **tree = (splay_tree_t **)cntr;
  return splay_tree_ctorv(tree, info, args);
}

static void dtor(void *cntr)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  splay_tree_dtor(tree);
}

static stat_t get(void *cntr, void *key, void **value)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  return splay_tree_get(tree, key, value);
}

static size_t count(void *cntr, void *key)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  return splay_tree_count(tree, key);
}

static void find(void *cntr, void *key, void *it)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  splay_tree_find(tree, key, iter);
}

static size_t size(void *cntr)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  return splay_tree_size(tree);
}

static bool empty(void *cntr)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  return splay_tree_empty(tree);
}

static void clear(void *cntr)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  splay_tree_clear(tree);
}

static stat_t insert(void *cntr, void *key, void *value, void *it, bool *inserted)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  return splay_tree_insert1(tree, key, value, iter, inserted);
}

static stat_t insert_or_assign(void *cntr, void *key, void *value, void *it, bool *inserted)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  return splay_tree_insert_or_assign1(tree, key, value, iter, inserted);
}

static size_t erase(void *cntr, void *key)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  return splay_tree_erase(tree, key);
}

static void swap(void *a, void *b)
{
  assert(a != NULL);
  assert(b != NULL);

  splay_tree_t *ta = (splay_tree_t *)a;
  splay_tree_t *tb = (splay_tree_t *)b;
  splay_tree_swap(ta, tb);
}

static void begin(void *cntr, void *it)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  splay_tree_begin(tree, iter);
}

static void end(void *cntr, void *it)
{
  assert(cntr != NULL);

  splay_tree_t *tree = (splay_tree_t *)cntr;
  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  splay_tree_end(tree, iter);
}

static void *iter_ctor()
{
  return malloc(sizeof(splay_tree_iter_t));
}

static void iter_dtor(void *it)
{
  free(it);
}

static enum cdc_iterator_type type()
{
  return CDC_BIDIR_ITERATOR;
}

static void iter_next(void *it)
{
  assert(it != NULL);

  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  splay_tree_iter_next(iter);
}

static void iter_prev(void *it)
{
  assert(it != NULL);

  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  splay_tree_iter_prev(iter);
}

static bool iter_has_next(void *it)
{
  assert(it != NULL);

  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  return splay_tree_iter_has_next(iter);
}

static bool iter_has_prev(void *it)
{
  assert(it != NULL);

  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  return splay_tree_iter_has_prev(iter);
}

static void *iter_key(void *it)
{
  assert(it != NULL);

  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  return splay_tree_iter_key(iter);
}

static void *iter_value(void *it)
{
  assert(it != NULL);

  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  return splay_tree_iter_value(iter);
}

static pair_t iter_key_value(void *it)
{
  assert(it != NULL);

  splay_tree_iter_t *iter = (splay_tree_iter_t *)it;
  return splay_tree_iter_key_value(iter);
}

static bool iter_eq(void *it1, void *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);

  splay_tree_iter_t *iter1 = (splay_tree_iter_t *)it1;
  splay_tree_iter_t *iter2 = (splay_tree_iter_t *)it2;
  return splay_tree_iter_is_eq(iter1, iter2);
}

static const map_iter_table_t _iter_table = {.ctor = iter_ctor,
                                             .dtor = iter_dtor,
                                             .type = type,
                                             .next = iter_next,
                                             .prev = iter_prev,
                                             .has_next = iter_has_next,
                                             .has_prev = iter_has_prev,
                                             .key = iter_key,
                                             .value = iter_value,
                                             .key_value = iter_key_value,
                                             .eq = iter_eq};

static const map_table_t _table = {.ctor = ctor,
                                   .ctorv = ctorv,
                                   .dtor = dtor,
                                   .get = get,
                                   .count = count,
                                   .find = find,
                                   .size = size,
                                   .empty = empty,
                                   .clear = clear,
                                   .insert = insert,
                                   .insert_or_assign = insert_or_assign,
                                   .erase = erase,
                                   .swap = swap,
                                   .begin = begin,
                                   .end = end,
                                   .iter_table = &_iter_table};

const map_table_t *cdc_map_splay = &_table;
