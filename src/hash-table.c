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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "data-info.h"

#define HASH_TABLE_MIN_CAPACITY     4
#define HASH_TABLE_COPACITY_EXP     2.0f
#define HASH_TABLE_LOAD_FACTOR      1.0f

static inline void free_entry(struct cdc_hash_table *t,
                              struct cdc_hash_table_entry *entry,
                              bool must_free)
{
        struct cdc_pair pair;

        if (must_free && CDC_HAS_DFREE(t)) {
                pair.first = entry->key;
                pair.second = entry->value;
                t->dinfo->dfree(&pair);
        }

        free(entry);
}

static inline void free_entries(struct cdc_hash_table *t)
{
        struct cdc_hash_table_entry *current = t->buckets[0]->next;
        struct cdc_hash_table_entry *next;

        while (current) {
                next = current->next;
                free_entry(t, current, true);
                current = next;
        }
}

static inline void free_all_entries(struct cdc_hash_table *t)
{
        free_entries(t);
        // free nil entry
        free(t->buckets[0]);
}

static inline bool should_rehash(struct cdc_hash_table *t)
{
        return ((float)t->size / (float)t->bcount) >= t->load_factor;
}

static inline enum cdc_stat rehash(struct cdc_hash_table *t)
{
        return cdc_hash_table_rehash(t, t->bcount *  HASH_TABLE_COPACITY_EXP);
}

static inline size_t index_for(size_t hash, size_t count)
{
        return hash & (count - 1);
}

struct cdc_hash_table_entry *ffind_entry(struct cdc_hash_table *t, void *key,
                                         size_t index)
{
        struct cdc_hash_table_entry *curr = t->buckets[index];

        if (curr == NULL)
                return NULL;

        while (curr->next) {
                if (t->eq(key, curr->next->key))
                        return curr;

                if (index != index_for(curr->next->hash, t->bcount))
                        return NULL;

                curr = curr->next;
        }

        return NULL;
}

struct cdc_hash_table_entry *find_entry(struct cdc_hash_table *t, void *key)
{
        size_t index = index_for(t->hash(key), t->bcount);

        return ffind_entry(t, key, index);
}

static inline struct cdc_hash_table_entry *add_entry(
                struct cdc_hash_table_entry *new_entry,
                struct cdc_hash_table_entry **tail,
                struct cdc_hash_table_entry **backets, size_t count)
{
        size_t new_index = index_for(new_entry->hash, count), next_index;
        struct cdc_hash_table_entry *entry = backets[new_index], *next, *ret;

        if (entry == NULL) {
                backets[new_index] = *tail;
                ret = *tail;
                new_entry->next = NULL;
                (*tail)->next = new_entry;
                (*tail) = new_entry;
        } else {
                next_index = index_for(entry->next->hash, count);
                if (next_index != new_index)
                        backets[next_index] = new_entry;

                ret = entry;
                next = entry->next;
                entry->next = new_entry;
                new_entry->next = next;
        }

        return ret;
}

static inline enum cdc_stat make_and_insert_unique(struct cdc_hash_table *t,
                                                   void *key, void *value,
                                                   struct cdc_hash_table_entry **ret)
{
        struct cdc_hash_table_entry *entry;
        enum cdc_stat result;

        if (should_rehash(t)) {
                result = rehash(t);
                if (result != CDC_STATUS_OK)
                        return result;
        }

        entry = (struct cdc_hash_table_entry *)malloc(
                                sizeof(struct cdc_hash_table_entry));
        if (!entry)
                return CDC_STATUS_BAD_ALLOC;


        entry->hash = t->hash(key);
        entry->value  = value;
        entry->key = key;

        *ret = add_entry(entry, &t->tail, t->buckets, t->bcount);
        ++t->size;
        return CDC_STATUS_OK;
}

static inline struct cdc_hash_table_entry *erase_entry(struct cdc_hash_table *t,
                                                       struct cdc_hash_table_entry *entry,
                                                       size_t index)
{
        struct cdc_hash_table_entry *next = entry->next->next;
        size_t e_hash = index_for(entry->hash, t->bcount);
        size_t en_hash = index_for(entry->next->hash, t->bcount);
        size_t n_hash;

        free_entry(t, entry->next, true);
        if (next != NULL) {
                n_hash = index_for(next->hash, t->bcount);
                if (e_hash != en_hash && en_hash != n_hash) {
                        entry->next = next;
                        if (entry == t->buckets[0])
                                t->buckets[index] = t->buckets[0];
                        else
                                t->buckets[index] = NULL;

                        index = index_for(next->hash, t->bcount);
                        t->buckets[index] = entry;
                } else if (en_hash == n_hash) {
                        entry->next = next;
                } else {
                        entry->next = next;
                        index = index_for(next->hash, t->bcount);
                        t->buckets[index] = entry;
                }
        } else {
                if (e_hash != en_hash)
                        t->buckets[index] = NULL;

                entry->next = NULL;
                t->tail = entry;
        }

        --t->size;
        return next;
}

static inline struct cdc_hash_table_entry *erase_entry_check(struct cdc_hash_table *t,
                                                             void *key, bool *erased)
{
        size_t index = index_for(t->hash(key), t->bcount);
        struct cdc_hash_table_entry *entry = ffind_entry(t, key, index);

        if (entry == NULL) {
                *erased = false;
                return NULL;
        }

        *erased = true;
        return erase_entry(t, entry, index);
}

static inline void transfer(struct cdc_hash_table *t,
                            struct cdc_hash_table_entry **tail,
                            struct cdc_hash_table_entry **backets, size_t count)
{
        struct cdc_hash_table_entry *curr = t->buckets[0]->next, *next;

        while (curr) {
                next = curr->next;
                add_entry(curr, tail, backets, count);
                curr = next;
        }

        (*tail)->next = NULL;
}

static inline enum cdc_stat reallocate(struct cdc_hash_table *t, size_t count)
{
        struct cdc_hash_table_entry **tmp;
        struct cdc_hash_table_entry *tail, *nil;

        if (count < HASH_TABLE_MIN_CAPACITY) {
                if (t->bcount > HASH_TABLE_MIN_CAPACITY)
                        count = HASH_TABLE_MIN_CAPACITY;
                else
                        return CDC_STATUS_OK;
        }

        tmp = (struct cdc_hash_table_entry **)calloc(count * sizeof (void *), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        if (t->buckets == NULL) {
                nil = (struct cdc_hash_table_entry *)calloc(
                                        sizeof(struct cdc_hash_table_entry), 1);
                if (!nil) {
                        free(tmp);
                        return CDC_STATUS_BAD_ALLOC;
                }

                tmp[0] = nil;
                tail = nil;
        } else {
                tail = tmp[0] = t->buckets[0];
                transfer(t, &tail, tmp, count);
                free(t->buckets);
        }

        t->bcount = count;
        t->buckets       = tmp;
        t->tail          = tail;
        return CDC_STATUS_OK;
}

static inline enum cdc_stat init_varg(struct cdc_hash_table *t, va_list args)
{
        enum cdc_stat ret;
        struct cdc_pair *pair;

        while ((pair = va_arg(args, struct cdc_pair *)) != NULL) {
                ret = cdc_hash_table_insert(t, pair->first, pair->second, NULL);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_hash_table_ctor(struct cdc_hash_table **t,
                                  struct cdc_data_info *info,
                                  cdc_hash_fn_t hash, cdc_binary_pred_fn_t equal,
                                  float load_factor)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);
        assert(load_factor > 0);

        struct cdc_hash_table *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_hash_table *)calloc(sizeof(struct cdc_hash_table), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->hash = hash;
        tmp->eq = equal;
        tmp->load_factor = load_factor;
        if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
                ret = CDC_STATUS_BAD_ALLOC;
                goto error1;
        }

        ret = reallocate(tmp, HASH_TABLE_MIN_CAPACITY);
        if (ret != CDC_STATUS_OK)
                goto error2;

        *t = tmp;
        return CDC_STATUS_OK;
error2:
        cdc_di_shared_dtor(tmp->dinfo);
error1:
        free(tmp);
        return ret;
}

enum cdc_stat cdc_hash_table_ctorl(struct cdc_hash_table **t,
                                   struct cdc_data_info *info,
                                   cdc_hash_fn_t hash, cdc_binary_pred_fn_t equal,
                                   float load_factor, ...)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);
        assert(load_factor > 0);

        enum cdc_stat ret;
        va_list args;

        va_start(args, load_factor);
        ret = cdc_hash_table_ctorv(t, info, hash, equal, load_factor, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_hash_table_ctorv(struct cdc_hash_table **t,
                                   struct cdc_data_info *info,
                                   cdc_hash_fn_t hash, cdc_binary_pred_fn_t equal,
                                   float load_factor, va_list args)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);
        assert(load_factor > 0);

        enum cdc_stat ret;

        ret = cdc_hash_table_ctor(t, info, hash, equal, load_factor);
        if (ret != CDC_STATUS_OK)
                return ret;

        return init_varg(*t, args);
}

void cdc_hash_table_dtor(struct cdc_hash_table *t)
{
        assert(t != NULL);

        free_all_entries(t);
        cdc_di_shared_dtor(t->dinfo);
        free(t);
}

void *cdc_hash_table_get(struct cdc_hash_table *t, void *key)
{
        assert(t != NULL);

        struct cdc_hash_table_entry *entry = find_entry(t, key);

        if (entry == NULL)
                return NULL;

        return entry->next->value;
}

size_t cdc_hash_table_count(struct cdc_hash_table *t, void *key)
{
        return cdc_hash_table_get(t, key) ? 1 : 0;
}

struct cdc_hash_table_iter cdc_hash_table_find(struct cdc_hash_table *t,
                                               void *key)
{
        assert(t != NULL);

        struct cdc_hash_table_entry *entry = find_entry(t, key);
        struct cdc_hash_table_iter ret = {t, entry ? entry->next : NULL};

        return ret;
}

struct cdc_pair_hash_table_iter cdc_hash_table_equal_range(struct cdc_hash_table *t,
                                                           void *key)
{
        assert(t != NULL);
        struct cdc_hash_table_entry *entry = find_entry(t, key);
        struct cdc_pair_hash_table_iter ret = {{t, entry ? entry->next : NULL},
                {t, entry ? entry->next->next : NULL}};

        return ret;
}

void cdc_hash_table_clear(struct cdc_hash_table *t)
{
        free_entries(t);
        t->buckets[0]->next = NULL;
        t->tail = t->buckets[0];
        t->size = 0;
}

enum cdc_stat cdc_hash_table_insert(struct cdc_hash_table *t,
                                    void *key, void *value,
                                    struct cdc_pair_hash_table_iter_bool *ret)
{
        struct cdc_hash_table_entry *entry = find_entry(t, key), *new_entry;
        enum cdc_stat result;


        if (entry == NULL) {
                result = make_and_insert_unique(t, key, value, &new_entry);
                if (result != CDC_STATUS_OK)
                        return result;
        }

        if (ret) {
                (*ret).first.container = t;
                (*ret).first.current = entry ? entry->next : new_entry->next;
                (*ret).second = !entry;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_hash_table_insert_or_assign(struct cdc_hash_table *t,
                                              void *key, void *value,
                                              struct cdc_pair_hash_table_iter_bool *ret)
{
        struct cdc_hash_table_entry *entry = find_entry(t, key), *new_entry;
        enum cdc_stat result;

        if (entry == NULL) {
                result = make_and_insert_unique(t, key, value, &new_entry);
                if (result != CDC_STATUS_OK)
                        return result;
        } else {
                entry->next->value = value;
        }

        if (ret) {
                (*ret).first.container = t;
                (*ret).first.current = entry ? entry->next : new_entry->next;
                (*ret).second = !entry;
        }

        return CDC_STATUS_OK;
}

size_t cdc_hash_table_erase(struct cdc_hash_table *t, void *key)
{
        assert(t != NULL);

        bool erased;

        erase_entry_check(t, key, &erased);
        return (size_t)erased;
}

void cdc_hash_table_swap(struct cdc_hash_table *a, struct cdc_hash_table *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_hash_table_entry *, a->tail, b->tail);
        CDC_SWAP(struct cdc_hash_table_entry **, a->buckets, b->buckets);
        CDC_SWAP(size_t, a->bcount, b->bcount);
        CDC_SWAP(float, a->load_factor, b->load_factor);
        CDC_SWAP(cdc_hash_fn_t, a->hash, b->hash);
        CDC_SWAP(cdc_binary_pred_fn_t, a->eq, b->eq);
        CDC_SWAP(size_t, a->size,  b->size);
        CDC_SWAP(struct cdc_data_info *, a->dinfo,  b->dinfo);
}

void cdc_hash_table_set_max_load_factor(struct cdc_hash_table *t,
                                        float load_factor)
{
        assert(t != NULL);

        t->load_factor = load_factor;
}

enum cdc_stat cdc_hash_table_rehash(struct cdc_hash_table *t, size_t count)
{
        assert(t != NULL);

        if (count <= t->bcount)
                return CDC_STATUS_OK;

        return reallocate(t, count);
}

enum cdc_stat cdc_hash_table_reserve(struct cdc_hash_table *t, size_t count)
{
        assert(t != NULL);

        return cdc_hash_table_rehash(t,  ((float)count / t->load_factor) + 1);
}
