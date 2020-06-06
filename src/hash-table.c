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
#include "cdcontainers/hash-table.h"

#include "cdcontainers/data-info.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_MIN_CAPACITY 8  // must be pow 2
#define HASH_TABLE_COPACITY_SHIFT 1
#define HASH_TABLE_LOAD_FACTOR 0.7f

static hash_table_entry_t *new_node(void *key, void *value, size_t hash)
{
  hash_table_entry_t *new_entry = (hash_table_entry_t *)malloc(sizeof(hash_table_entry_t));
  if (!new_entry) {
    return NULL;
  }

  new_entry->next = NULL;
  new_entry->value = value;
  new_entry->key = key;
  new_entry->hash = hash;
  return new_entry;
}

static void free_entry(hash_table_t *t, hash_table_entry_t *entry)
{
  if (CDC_HAS_DFREE(t->dinfo)) {
    pair_t pair = {entry->key, entry->value};
    t->dinfo->dfree(&pair);
  }

  free(entry);
}

static void free_entries(hash_table_t *t)
{
  hash_table_entry_t *curr = t->buckets[0]->next;
  while (curr) {
    hash_table_entry_t *next = curr->next;
    free_entry(t, curr);
    curr = next;
  }
}

static void free_all_entries(hash_table_t *t)
{
  free_entries(t);
  // free nil entry
  free(t->buckets[0]);
}

static bool should_rehash(hash_table_t *t)
{
  return ((float)t->size / (float)t->bcount) >= t->load_factor;
}

static stat_t rehash(hash_table_t *t)
{
  return hash_table_rehash(t, t->bcount << HASH_TABLE_COPACITY_SHIFT);
}

static size_t get_bucket(size_t hash, size_t count)
{
  return hash & (count - 1);
}

static hash_table_entry_t *find_entry_by_bucket(hash_table_t *t, void *key, size_t bucket)
{
  hash_table_entry_t *entry = t->buckets[bucket];
  if (entry == NULL) {
    return NULL;
  }

  while (entry->next) {
    if (t->dinfo->eq(key, entry->next->key)) {
      return entry;
    }

    if (bucket != get_bucket(entry->next->hash, t->bcount)) {
      return NULL;
    }

    entry = entry->next;
  }

  return NULL;
}

static hash_table_entry_t *find_entry(hash_table_t *t, void *key)
{
  size_t bucket = key ? get_bucket(t->dinfo->hash(key), t->bcount) : 0;
  return find_entry_by_bucket(t, key, bucket);
}

static hash_table_entry_t *add_entry(hash_table_t *t, hash_table_entry_t *new_entry)
{
  size_t bucket = new_entry->key ? get_bucket(new_entry->hash, t->bcount) : 0;
  hash_table_entry_t *entry = t->buckets[bucket];
  hash_table_entry_t *prev_entry = NULL;
  if (entry == NULL) {
    t->buckets[bucket] = t->tail;
    prev_entry = t->tail;
    t->tail->next = new_entry;
    t->tail = new_entry;
  } else {
    if (entry->next) {
      size_t next_bucket = get_bucket(entry->next->hash, t->bcount);
      if (next_bucket != bucket) {
        t->buckets[next_bucket] = new_entry;
      }

      prev_entry = entry;
      hash_table_entry_t *next = entry->next;
      entry->next = new_entry;
      new_entry->next = next;
    } else {
      prev_entry = t->tail;
      t->tail->next = new_entry;
      t->tail = new_entry;
    }
  }

  return prev_entry;
}

static stat_t make_and_insert_unique(hash_table_t *t, void *key, void *value,
                                     hash_table_entry_t **ret)
{
  if (should_rehash(t)) {
    stat_t stat = rehash(t);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  hash_table_entry_t *entry = new_node(key, value, t->dinfo->hash(key));
  if (!entry) {
    return CDC_STATUS_BAD_ALLOC;
  }

  *ret = add_entry(t, entry);
  ++t->size;
  return CDC_STATUS_OK;
}

static hash_table_entry_t *erase_entry(hash_table_t *t, hash_table_entry_t *entry, size_t bucket)
{
  assert(entry != NULL);
  assert(entry->next != NULL);

  hash_table_entry_t *next = entry->next->next;
  size_t e_hash = get_bucket(entry->hash, t->bcount);
  size_t en_hash = get_bucket(entry->next->hash, t->bcount);
  free_entry(t, entry->next);
  if (next != NULL) {
    size_t n_hash = get_bucket(next->hash, t->bcount);
    if (e_hash != en_hash && en_hash != n_hash) {
      entry->next = next;
      if (entry == t->buckets[0]) {
        t->buckets[bucket] = t->buckets[0];
      } else {
        t->buckets[bucket] = NULL;
      }

      bucket = get_bucket(next->hash, t->bcount);
      t->buckets[bucket] = entry;
    } else if (en_hash == n_hash) {
      entry->next = next;
    } else {
      entry->next = next;
      bucket = get_bucket(next->hash, t->bcount);
      t->buckets[bucket] = entry;
    }
  } else {
    if (e_hash != en_hash) {
      t->buckets[bucket] = NULL;
    }

    entry->next = NULL;
    t->tail = entry;
  }

  --t->size;
  return next;
}

static hash_table_entry_t **transfer(hash_table_t *t, hash_table_entry_t **buckets, size_t count)
{
  hash_table_entry_t *entry = t->buckets[0]->next;
  hash_table_entry_t **old_buffer = t->buckets;
  // Set a new buffer and a new tail.
  t->buckets[0]->next = NULL;
  buckets[0] = t->buckets[0];
  t->buckets = buckets;
  t->tail = buckets[0];
  t->bcount = count;

  hash_table_entry_t *next_entry = NULL;
  while (entry) {
    next_entry = entry->next;
    entry->next = NULL;
    add_entry(t, entry);
    entry = next_entry;
  }

  return old_buffer;
}

static stat_t reallocate(hash_table_t *t, size_t count)
{
  if (count < HASH_TABLE_MIN_CAPACITY) {
    if (t->bcount > HASH_TABLE_MIN_CAPACITY) {
      count = HASH_TABLE_MIN_CAPACITY;
    } else {
      return CDC_STATUS_OK;
    }
  }

  hash_table_entry_t **new_buckets = (hash_table_entry_t **)calloc(count * sizeof(void *), 1);
  if (!new_buckets) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (t->buckets) {
    hash_table_entry_t **old_buffer = transfer(t, new_buckets, count);
    free(old_buffer);
  } else {
    hash_table_entry_t *nil = (hash_table_entry_t *)calloc(sizeof(hash_table_entry_t), 1);
    if (!nil) {
      free(new_buckets);
      return CDC_STATUS_BAD_ALLOC;
    }

    t->buckets = new_buckets;
    t->buckets[0] = nil;
    t->tail = nil;
    t->bcount = count;
  }

  return CDC_STATUS_OK;
}

static stat_t init_varg(hash_table_t *t, va_list args)
{
  pair_t *pair = NULL;
  while ((pair = va_arg(args, pair_t *)) != CDC_END) {
    stat_t stat = hash_table_insert(t, pair->first, pair->second, NULL, NULL);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  return CDC_STATUS_OK;
}

stat_t hash_table_ctor1(hash_table_t **t, data_info_t *info, float load_factor)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));
  assert(load_factor > 0);

  hash_table_t *tmp = (hash_table_t *)calloc(sizeof(hash_table_t), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->load_factor = load_factor;
  stat_t stat = CDC_STATUS_OK;
  if (info && !(tmp->dinfo = di_shared_ctorc(info))) {
    stat = CDC_STATUS_BAD_ALLOC;
    goto free_hash_table;
  }

  stat = reallocate(tmp, HASH_TABLE_MIN_CAPACITY);
  if (stat != CDC_STATUS_OK) {
    goto free_di;
  }

  *t = tmp;
  return stat;
free_di:
  di_shared_dtor(tmp->dinfo);
free_hash_table:
  free(tmp);
  return stat;
}

stat_t hash_table_ctorl1(hash_table_t **t, data_info_t *info, float load_factor, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));
  assert(load_factor > 0);

  va_list args;
  va_start(args, load_factor);
  stat_t stat = cdc_hash_table_ctorv1(t, info, load_factor, args);
  va_end(args);
  return stat;
}

stat_t hash_table_ctorv1(hash_table_t **t, data_info_t *info, float load_factor, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));
  assert(load_factor > 0);

  stat_t stat = hash_table_ctor1(t, info, load_factor);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  return init_varg(*t, args);
}

stat_t hash_table_ctor(hash_table_t **t, data_info_t *info)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));

  return hash_table_ctor1(t, info, HASH_TABLE_LOAD_FACTOR);
}

stat_t hash_table_ctorl(hash_table_t **t, data_info_t *info, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));

  va_list args;
  va_start(args, info);
  stat_t stat = hash_table_ctorv(t, info, args);
  va_end(args);
  return stat;
}

stat_t hash_table_ctorv(hash_table_t **t, data_info_t *info, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));

  return hash_table_ctorv1(t, info, HASH_TABLE_LOAD_FACTOR, args);
}

void hash_table_dtor(hash_table_t *t)
{
  assert(t != NULL);

  free_all_entries(t);
  free(t->buckets);
  di_shared_dtor(t->dinfo);
  free(t);
}

stat_t hash_table_get(hash_table_t *t, void *key, void **value)
{
  assert(t != NULL);

  hash_table_entry_t *entry = find_entry(t, key);
  if (!entry) {
    return CDC_STATUS_NOT_FOUND;
  }

  *value = entry->next->value;
  return CDC_STATUS_OK;
}

size_t hash_table_count(hash_table_t *t, void *key)
{
  assert(t != NULL);

  return (size_t)(find_entry(t, key) != NULL);
}

void hash_table_find(hash_table_t *t, void *key, hash_table_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  hash_table_entry_t *entry = find_entry(t, key);
  it->container = t;
  it->current = entry ? entry->next : NULL;
}

void hash_table_clear(hash_table_t *t)
{
  assert(t != NULL);

  free_entries(t);
  t->buckets[0]->next = NULL;
  t->tail = t->buckets[0];
  t->size = 0;
}

stat_t hash_table_insert(hash_table_t *t, void *key, void *value, hash_table_iter_t *it,
                         bool *inserted)
{
  assert(t != NULL);

  hash_table_entry_t *entry = find_entry(t, key);
  bool finded = entry;
  if (!finded) {
    stat_t stat = make_and_insert_unique(t, key, value, &entry);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  if (it) {
    it->container = t;
    it->current = entry->next;
  }

  if (inserted) {
    *inserted = !finded;
  }

  return CDC_STATUS_OK;
}

stat_t hash_table_insert_or_assign(hash_table_t *t, void *key, void *value, hash_table_iter_t *it,
                                   bool *inserted)
{
  assert(t != NULL);

  hash_table_entry_t *entry = find_entry(t, key);
  bool finded = entry;
  if (!finded) {
    stat_t stat = make_and_insert_unique(t, key, value, &entry);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  } else {
    entry->next->value = value;
  }

  if (it) {
    it->container = t;
    it->current = entry->next;
  }

  if (inserted) {
    *inserted = !finded;
  }

  return CDC_STATUS_OK;
}

size_t hash_table_erase(hash_table_t *t, void *key)
{
  assert(t != NULL);

  size_t bucket = key ? get_bucket(t->dinfo->hash(key), t->bcount) : 0;
  hash_table_entry_t *entry = find_entry_by_bucket(t, key, bucket);
  if (!entry) {
    return 0;
  }

  erase_entry(t, entry, bucket);
  return 1;
}

void hash_table_swap(hash_table_t *a, hash_table_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(hash_table_entry_t *, a->tail, b->tail);
  CDC_SWAP(hash_table_entry_t **, a->buckets, b->buckets);
  CDC_SWAP(size_t, a->bcount, b->bcount);
  CDC_SWAP(float, a->load_factor, b->load_factor);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}

stat_t hash_table_rehash(hash_table_t *t, size_t count)
{
  assert(t != NULL);

  if (count <= t->bcount) {
    return CDC_STATUS_OK;
  }

  count = cdc_up_to_pow2(count);
  assert(count != 0);
  return reallocate(t, count);
}

stat_t hash_table_reserve(hash_table_t *t, size_t count)
{
  assert(t != NULL);

  return hash_table_rehash(t, (size_t)((float)count / t->load_factor) + 1);
}
