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
#include "cdcontainers/interfaces/imap.h"
#include "cdcontainers/treap.h"

#include <assert.h>
#include <stdlib.h>

static enum cdc_stat ctor(void **cntr, struct cdc_data_info *info)
{
  assert(cntr != NULL);

  struct cdc_treap **tree = (struct cdc_treap **)cntr;
  return cdc_treap_ctor(tree, info);
}

static enum cdc_stat ctorv(void **cntr, struct cdc_data_info *info,
                           va_list args)
{
  assert(cntr != NULL);

  struct cdc_treap **tree = (struct cdc_treap **)cntr;
  return cdc_treap_ctorv(tree, info, args);
}

static void dtor(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  cdc_treap_dtor(tree);
}

static enum cdc_stat get(void *cntr, void *key, void **value)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  return cdc_treap_get(tree, key, value);
}

static size_t count(void *cntr, void *key)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  return cdc_treap_count(tree, key);
}

static void find(void *cntr, void *key, void *it)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  cdc_treap_find(tree, key, iter);
}

static size_t size(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  return cdc_treap_size(tree);
}

static bool empty(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  return cdc_treap_empty(tree);
}

static void clear(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  cdc_treap_clear(tree);
}

static enum cdc_stat insert(void *cntr, void *key, void *value, void *it,
                            bool *inserted)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  return cdc_treap_insert1(tree, key, value, iter, inserted);
}

static enum cdc_stat insert_or_assign(void *cntr, void *key, void *value,
                                      void *it, bool *inserted)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  return cdc_treap_insert_or_assign1(tree, key, value, iter, inserted);
}

static size_t erase(void *cntr, void *key)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  return cdc_treap_erase(tree, key);
}

static void swap(void *a, void *b)
{
  assert(a != NULL);
  assert(b != NULL);

  struct cdc_treap *ta = (struct cdc_treap *)a;
  struct cdc_treap *tb = (struct cdc_treap *)b;
  cdc_treap_swap(ta, tb);
}

static void begin(void *cntr, void *it)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  cdc_treap_begin(tree, iter);
}

static void end(void *cntr, void *it)
{
  assert(cntr != NULL);

  struct cdc_treap *tree = (struct cdc_treap *)cntr;
  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  cdc_treap_end(tree, iter);
}

static void *iter_ctor() { return malloc(sizeof(struct cdc_treap_iter)); }

static void iter_dtor(void *it) { free(it); }

static enum cdc_iterator_type type() { return CDC_BIDIR_ITERATOR; }

static void iter_next(void *it)
{
  assert(it != NULL);

  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  cdc_treap_iter_next(iter);
}

static void iter_prev(void *it)
{
  assert(it != NULL);

  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  cdc_treap_iter_prev(iter);
}

static bool iter_has_next(void *it)
{
  assert(it != NULL);

  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  return cdc_treap_iter_has_next(iter);
}

static bool iter_has_prev(void *it)
{
  assert(it != NULL);

  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  return cdc_treap_iter_has_prev(iter);
}

static void *iter_key(void *it)
{
  assert(it != NULL);

  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  return cdc_treap_iter_key(iter);
}

static void *iter_value(void *it)
{
  assert(it != NULL);

  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  return cdc_treap_iter_value(iter);
}

static struct cdc_pair iter_key_value(void *it)
{
  assert(it != NULL);

  struct cdc_treap_iter *iter = (struct cdc_treap_iter *)it;
  return cdc_treap_iter_key_value(iter);
}

static bool iter_eq(void *it1, void *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);

  struct cdc_treap_iter *iter1 = (struct cdc_treap_iter *)it1;
  struct cdc_treap_iter *iter2 = (struct cdc_treap_iter *)it2;
  return cdc_treap_iter_is_eq(iter1, iter2);
}

static const struct cdc_map_iter_table _iter_table = {
    .ctor = iter_ctor,
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

static const struct cdc_map_table _table = {
    .ctor = ctor,
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

const struct cdc_map_table *cdc_map_treap = &_table;
