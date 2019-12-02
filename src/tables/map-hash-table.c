// The MIT License (MIT)
// Copyright (c) 2019 Maksim Andrianov
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
#include "cdcontainers/global.h"
#include "cdcontainers/hash-table.h"
#include "cdcontainers/interfaces/imap.h"

#include <assert.h>
#include <stdlib.h>

static enum cdc_stat ctor(void **cntr, struct cdc_data_info *info)
{
  assert(cntr != NULL);

  struct cdc_hash_table **tree = (struct cdc_hash_table **)cntr;
  return cdc_hash_table_ctor(tree, info);
}

static enum cdc_stat ctorv(void **cntr, struct cdc_data_info *info,
                           va_list args)
{
  assert(cntr != NULL);

  struct cdc_hash_table **tree = (struct cdc_hash_table **)cntr;
  return cdc_hash_table_ctorv(tree, info, args);
}

static void dtor(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  cdc_hash_table_dtor(tree);
}

static enum cdc_stat get(void *cntr, void *key, void **value)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  return cdc_hash_table_get(tree, key, value);
}

static size_t count(void *cntr, void *key)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  return cdc_hash_table_count(tree, key);
}

static void find(void *cntr, void *key, void *it)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  cdc_hash_table_find(tree, key, iter);
}

static size_t size(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  return cdc_hash_table_size(tree);
}

static bool empty(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  return cdc_hash_table_empty(tree);
}

static void clear(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  cdc_hash_table_clear(tree);
}

static enum cdc_stat insert(void *cntr, void *key, void *value, void *it,
                            bool *inserted)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  return cdc_hash_table_insert(tree, key, value, iter, inserted);
}

static enum cdc_stat insert_or_assign(void *cntr, void *key, void *value,
                                      void *it, bool *inserted)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  return cdc_hash_table_insert_or_assign(tree, key, value, iter, inserted);
}

static size_t erase(void *cntr, void *key)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  return cdc_hash_table_erase(tree, key);
}

static void swap(void *a, void *b)
{
  assert(a != NULL);
  assert(b != NULL);

  struct cdc_hash_table *ta = (struct cdc_hash_table *)a;
  struct cdc_hash_table *tb = (struct cdc_hash_table *)b;
  cdc_hash_table_swap(ta, tb);
}

static void begin(void *cntr, void *it)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  cdc_hash_table_begin(tree, iter);
}

static void end(void *cntr, void *it)
{
  assert(cntr != NULL);

  struct cdc_hash_table *tree = (struct cdc_hash_table *)cntr;
  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  cdc_hash_table_end(tree, iter);
}

static void *iter_alloc() { return malloc(sizeof(struct cdc_hash_table_iter)); }

static void iter_free(void *it) { free(it); }

static void iter_next(void *it)
{
  assert(it != NULL);

  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  cdc_hash_table_iter_next(iter);
}

static void iter_prev(void *it)
{
  CDC_UNUSED(it);

  CDC_CHECK(false, "Hash table iterators do not support iter_prev().");
}

static bool iter_has_next(void *it)
{
  assert(it != NULL);

  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  return cdc_hash_table_iter_has_next(iter);
}

static bool iter_has_prev(void *it)
{
  CDC_UNUSED(it);

  CDC_CHECK(false, "Hash table iterators do not support iter_has_prev().");
}

static void *iter_key(void *it)
{
  assert(it != NULL);

  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  return cdc_hash_table_iter_key(iter);
}

static void *iter_value(void *it)
{
  assert(it != NULL);

  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  return cdc_hash_table_iter_value(iter);
}

static struct cdc_pair iter_key_value(void *it)
{
  assert(it != NULL);

  struct cdc_hash_table_iter *iter = (struct cdc_hash_table_iter *)it;
  return cdc_hash_table_iter_key_value(iter);
}

static bool iter_eq(void *it1, void *it2)
{
  assert(it1 != NULL);
  assert(it2 != NULL);

  struct cdc_hash_table_iter *iter1 = (struct cdc_hash_table_iter *)it1;
  struct cdc_hash_table_iter *iter2 = (struct cdc_hash_table_iter *)it2;
  return cdc_hash_table_iter_is_eq(iter1, iter2);
}

static const struct cdc_map_iter_table _iter_table = {
    .alloc = iter_alloc,
    .free = iter_free,
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

const void *cdc_map_htable = &_table;
