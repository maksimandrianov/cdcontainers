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

static struct cdc_hash_table_entry *new_node(void *key, void *value,
                                             size_t hash)
{
  struct cdc_hash_table_entry *new_entry =
      (struct cdc_hash_table_entry *)malloc(
          sizeof(struct cdc_hash_table_entry));
  if (!new_entry) {
    return NULL;
  }

  new_entry->next = NULL;
  new_entry->value = value;
  new_entry->key = key;
  new_entry->hash = hash;
  return new_entry;
}

static void free_entry(struct cdc_hash_table *t,
                       struct cdc_hash_table_entry *entry)
{
  if (CDC_HAS_DFREE(t->dinfo)) {
    struct cdc_pair pair = {.first = entry->key, .second = entry->value};
    t->dinfo->dfree(&pair);
  }

  free(entry);
}

static void free_entries(struct cdc_hash_table *t)
{
  struct cdc_hash_table_entry *curr = t->buckets[0]->next;
  while (curr) {
    struct cdc_hash_table_entry *next = curr->next;
    free_entry(t, curr);
    curr = next;
  }
}

static void free_all_entries(struct cdc_hash_table *t)
{
  free_entries(t);
  // free nil entry
  free(t->buckets[0]);
}

static bool should_rehash(struct cdc_hash_table *t)
{
  return ((float)t->size / (float)t->bcount) >= t->load_factor;
}

static enum cdc_stat rehash(struct cdc_hash_table *t)
{
  return cdc_hash_table_rehash(t, t->bcount << HASH_TABLE_COPACITY_SHIFT);
}

static size_t get_bucket(size_t hash, size_t count)
{
  return hash & (count - 1);
}

static struct cdc_hash_table_entry *find_entry_by_bucket(
    struct cdc_hash_table *t, void *key, size_t bucket)
{
  struct cdc_hash_table_entry *entry = t->buckets[bucket];
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

static struct cdc_hash_table_entry *find_entry(struct cdc_hash_table *t,
                                               void *key)
{
  size_t bucket = key ? get_bucket(t->dinfo->hash(key), t->bcount) : 0;
  return find_entry_by_bucket(t, key, bucket);
}

static struct cdc_hash_table_entry *add_entry(
    struct cdc_hash_table *t, struct cdc_hash_table_entry *new_entry)
{
  size_t bucket = new_entry->key ? get_bucket(new_entry->hash, t->bcount) : 0;
  struct cdc_hash_table_entry *entry = t->buckets[bucket];
  struct cdc_hash_table_entry *prev_entry = NULL;
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
      struct cdc_hash_table_entry *next = entry->next;
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

static enum cdc_stat make_and_insert_unique(struct cdc_hash_table *t, void *key,
                                            void *value,
                                            struct cdc_hash_table_entry **ret)
{
  if (should_rehash(t)) {
    enum cdc_stat stat = rehash(t);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  struct cdc_hash_table_entry *entry =
      new_node(key, value, t->dinfo->hash(key));
  if (!entry) {
    return CDC_STATUS_BAD_ALLOC;
  }

  *ret = add_entry(t, entry);
  ++t->size;
  return CDC_STATUS_OK;
}

static struct cdc_hash_table_entry *erase_entry(
    struct cdc_hash_table *t, struct cdc_hash_table_entry *entry, size_t bucket)
{
  assert(entry != NULL);
  assert(entry->next != NULL);

  struct cdc_hash_table_entry *next = entry->next->next;
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

static struct cdc_hash_table_entry **transfer(
    struct cdc_hash_table *t, struct cdc_hash_table_entry **buckets,
    size_t count)
{
  struct cdc_hash_table_entry *entry = t->buckets[0]->next;
  struct cdc_hash_table_entry **old_buffer = t->buckets;
  // Set a new buffer and a new tail.
  t->buckets[0]->next = NULL;
  buckets[0] = t->buckets[0];
  t->buckets = buckets;
  t->tail = buckets[0];
  t->bcount = count;

  struct cdc_hash_table_entry *next_entry = NULL;
  while (entry) {
    next_entry = entry->next;
    entry->next = NULL;
    add_entry(t, entry);
    entry = next_entry;
  }

  return old_buffer;
}

static enum cdc_stat reallocate(struct cdc_hash_table *t, size_t count)
{
  if (count < HASH_TABLE_MIN_CAPACITY) {
    if (t->bcount > HASH_TABLE_MIN_CAPACITY) {
      count = HASH_TABLE_MIN_CAPACITY;
    } else {
      return CDC_STATUS_OK;
    }
  }

  struct cdc_hash_table_entry **new_buckets =
      (struct cdc_hash_table_entry **)calloc(count * sizeof(void *), 1);
  if (!new_buckets) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (t->buckets) {
    struct cdc_hash_table_entry **old_buffer = transfer(t, new_buckets, count);
    free(old_buffer);
  } else {
    struct cdc_hash_table_entry *nil = (struct cdc_hash_table_entry *)calloc(
        sizeof(struct cdc_hash_table_entry), 1);
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

static enum cdc_stat init_varg(struct cdc_hash_table *t, va_list args)
{
  struct cdc_pair *pair = NULL;
  while ((pair = va_arg(args, struct cdc_pair *)) != CDC_END) {
    enum cdc_stat stat =
        cdc_hash_table_insert(t, pair->first, pair->second, NULL, NULL);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  return CDC_STATUS_OK;
}

enum cdc_stat cdc_hash_table_ctor1(struct cdc_hash_table **t,
                                   struct cdc_data_info *info,
                                   float load_factor)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));
  assert(load_factor > 0);

  struct cdc_hash_table *tmp =
      (struct cdc_hash_table *)calloc(sizeof(struct cdc_hash_table), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->load_factor = load_factor;
  enum cdc_stat stat = CDC_STATUS_OK;
  if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
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
  cdc_di_shared_dtor(tmp->dinfo);
free_hash_table:
  free(tmp);
  return stat;
}

enum cdc_stat cdc_hash_table_ctorl1(struct cdc_hash_table **t,
                                    struct cdc_data_info *info,
                                    float load_factor, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));
  assert(load_factor > 0);

  va_list args;
  va_start(args, load_factor);
  enum cdc_stat stat = cdc_hash_table_ctorv1(t, info, load_factor, args);
  va_end(args);
  return stat;
}

enum cdc_stat cdc_hash_table_ctorv1(struct cdc_hash_table **t,
                                    struct cdc_data_info *info,
                                    float load_factor, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));
  assert(load_factor > 0);

  enum cdc_stat stat = cdc_hash_table_ctor1(t, info, load_factor);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  return init_varg(*t, args);
}

enum cdc_stat cdc_hash_table_ctor(struct cdc_hash_table **t,
                                  struct cdc_data_info *info)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));

  return cdc_hash_table_ctor1(t, info, HASH_TABLE_LOAD_FACTOR);
}

enum cdc_stat cdc_hash_table_ctorl(struct cdc_hash_table **t,
                                   struct cdc_data_info *info, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));

  va_list args;
  va_start(args, info);
  enum cdc_stat stat = cdc_hash_table_ctorv(t, info, args);
  va_end(args);
  return stat;
}

enum cdc_stat cdc_hash_table_ctorv(struct cdc_hash_table **t,
                                   struct cdc_data_info *info, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_HASH(info));
  assert(CDC_HAS_EQ(info));

  return cdc_hash_table_ctorv1(t, info, HASH_TABLE_LOAD_FACTOR, args);
}

void cdc_hash_table_dtor(struct cdc_hash_table *t)
{
  assert(t != NULL);

  free_all_entries(t);
  free(t->buckets);
  cdc_di_shared_dtor(t->dinfo);
  free(t);
}

enum cdc_stat cdc_hash_table_get(struct cdc_hash_table *t, void *key,
                                 void **value)
{
  assert(t != NULL);

  struct cdc_hash_table_entry *entry = find_entry(t, key);
  if (!entry) {
    return CDC_STATUS_NOT_FOUND;
  }

  *value = entry->next->value;
  return CDC_STATUS_OK;
}

size_t cdc_hash_table_count(struct cdc_hash_table *t, void *key)
{
  assert(t != NULL);

  return (size_t)(find_entry(t, key) != NULL);
}

void cdc_hash_table_find(struct cdc_hash_table *t, void *key,
                         struct cdc_hash_table_iter *it)
{
  assert(t != NULL);
  assert(it != NULL);

  struct cdc_hash_table_entry *entry = find_entry(t, key);
  it->container = t;
  it->current = entry ? entry->next : NULL;
}

void cdc_hash_table_equal_range(struct cdc_hash_table *t, void *key,
                                struct cdc_pair_hash_table_iter *pair)
{
  assert(t != NULL);
  assert(pair != NULL);

  struct cdc_hash_table_entry *entry = find_entry(t, key);
  pair->first.container = pair->second.container = t;
  if (entry) {
    pair->first.current = entry->next;
    pair->second.current = entry->next->next;
  } else {
    pair->first.current = pair->second.current = NULL;
  }
}

void cdc_hash_table_clear(struct cdc_hash_table *t)
{
  assert(t != NULL);

  free_entries(t);
  t->buckets[0]->next = NULL;
  t->tail = t->buckets[0];
  t->size = 0;
}

enum cdc_stat cdc_hash_table_insert(struct cdc_hash_table *t, void *key,
                                    void *value, struct cdc_hash_table_iter *it,
                                    bool *inserted)
{
  assert(t != NULL);

  struct cdc_hash_table_entry *entry = find_entry(t, key);
  bool finded = entry;
  if (!finded) {
    enum cdc_stat stat = make_and_insert_unique(t, key, value, &entry);
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

enum cdc_stat cdc_hash_table_insert_or_assign(struct cdc_hash_table *t,
                                              void *key, void *value,
                                              struct cdc_hash_table_iter *it,
                                              bool *inserted)
{
  assert(t != NULL);

  struct cdc_hash_table_entry *entry = find_entry(t, key);
  bool finded = entry;
  if (!finded) {
    enum cdc_stat stat = make_and_insert_unique(t, key, value, &entry);
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

size_t cdc_hash_table_erase(struct cdc_hash_table *t, void *key)
{
  assert(t != NULL);

  size_t bucket = key ? get_bucket(t->dinfo->hash(key), t->bcount) : 0;
  struct cdc_hash_table_entry *entry = find_entry_by_bucket(t, key, bucket);
  if (!entry) {
    return 0;
  }

  erase_entry(t, entry, bucket);
  return 1;
}

void cdc_hash_table_swap(struct cdc_hash_table *a, struct cdc_hash_table *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(struct cdc_hash_table_entry *, a->tail, b->tail);
  CDC_SWAP(struct cdc_hash_table_entry **, a->buckets, b->buckets);
  CDC_SWAP(size_t, a->bcount, b->bcount);
  CDC_SWAP(float, a->load_factor, b->load_factor);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}

enum cdc_stat cdc_hash_table_rehash(struct cdc_hash_table *t, size_t count)
{
  assert(t != NULL);

  if (count <= t->bcount) {
    return CDC_STATUS_OK;
  }

  count = cdc_up_to_pow2(count);
  assert(count != 0);
  return reallocate(t, count);
}

enum cdc_stat cdc_hash_table_reserve(struct cdc_hash_table *t, size_t count)
{
  assert(t != NULL);

  return cdc_hash_table_rehash(t, (size_t)((float)count / t->load_factor) + 1);
}
