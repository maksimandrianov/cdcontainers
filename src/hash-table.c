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

#define HASH_TABLE_MIN_CAPACITY     4  // must be pow 2
#define HASH_TABLE_COPACITY_SHIFT   1
#define HASH_TABLE_LOAD_FACTOR      1.0f

static void free_entry(struct cdc_hash_table *t,
                       struct cdc_hash_table_entry *entry)
{
        struct cdc_pair pair;

        if (CDC_HAS_DFREE(t)) {
                pair.first = entry->key;
                pair.second = entry->value;
                t->dinfo->dfree(&pair);
        }

        free(entry);
}

static void free_entries(struct cdc_hash_table *t)
{
        struct cdc_hash_table_entry *curr = t->buckets[0]->next, *next;

        while (curr) {
                next = curr->next;
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

static size_t cbacket(size_t hash, size_t count)
{
        return hash & (count - 1);
}

static struct cdc_hash_table_entry *ffind_entry(struct cdc_hash_table *t,
                                                void *key, size_t backet)
{
        struct cdc_hash_table_entry *entry = t->buckets[backet];

        if (entry == NULL)
                return NULL;

        while (entry->next) {
                if (t->eq(key, entry->next->key))
                        return entry;

                if (backet != cbacket(entry->next->hash, t->bcount))
                        return NULL;

                entry = entry->next;
        }

        return NULL;
}

static struct cdc_hash_table_entry *find_entry(struct cdc_hash_table *t, void *key)
{
        size_t backet = key ? cbacket(t->hash(key), t->bcount) : 0;

        return ffind_entry(t, key, backet);
}

static struct cdc_hash_table_entry *add_entry(struct cdc_hash_table_entry *new_entry,
                                              struct cdc_hash_table_entry **tail,
                                              struct cdc_hash_table_entry **backets,
                                              size_t count)
{
        size_t next_backet;
        size_t new_backet = new_entry->key ? cbacket(new_entry->hash, count) : 0;
        struct cdc_hash_table_entry *entry = backets[new_backet], *next, *retval;

        if (entry == NULL) {
                backets[new_backet] = *tail;
                retval = *tail;
                new_entry->next = NULL;
                (*tail)->next = new_entry;
                (*tail) = new_entry;
        } else {
                if (entry->next) {
                        next_backet = cbacket(entry->next->hash, count);
                        if (next_backet != new_backet)
                                backets[next_backet] = new_entry;

                        retval = entry;
                        next = entry->next;
                        entry->next = new_entry;
                        new_entry->next = next;
                } else {
                        retval = *tail;
                        new_entry->next = NULL;
                        (*tail)->next = new_entry;
                        (*tail) = new_entry;
                }
        }

        return retval;
}

static enum cdc_stat make_and_insert_unique(struct cdc_hash_table *t, void *key,
                                            void *value, struct cdc_hash_table_entry **ret)
{
        struct cdc_hash_table_entry *new_entry;
        enum cdc_stat stat;

        if (should_rehash(t)) {
                stat = rehash(t);
                if (stat != CDC_STATUS_OK)
                        return stat;
        }

        new_entry = (struct cdc_hash_table_entry *)malloc(
                                sizeof(struct cdc_hash_table_entry));
        if (!new_entry)
                return CDC_STATUS_BAD_ALLOC;

        new_entry->hash  = t->hash(key);
        new_entry->value = value;
        new_entry->key   = key;

        *ret = add_entry(new_entry, &t->tail, t->buckets, t->bcount);
        ++t->size;
        return CDC_STATUS_OK;
}

static struct cdc_hash_table_entry *erase_entry(struct cdc_hash_table *t,
                                                struct cdc_hash_table_entry *entry,
                                                size_t backet)
{
        struct cdc_hash_table_entry *next = entry->next->next;
        size_t e_hash = cbacket(entry->hash, t->bcount);
        size_t en_hash = cbacket(entry->next->hash, t->bcount);
        size_t n_hash;

        free_entry(t, entry->next);
        if (next != NULL) {
                n_hash = cbacket(next->hash, t->bcount);
                if (e_hash != en_hash && en_hash != n_hash) {
                        entry->next = next;
                        if (entry == t->buckets[0])
                                t->buckets[backet] = t->buckets[0];
                        else
                                t->buckets[backet] = NULL;

                        backet = cbacket(next->hash, t->bcount);
                        t->buckets[backet] = entry;
                } else if (en_hash == n_hash) {
                        entry->next = next;
                } else {
                        entry->next = next;
                        backet = cbacket(next->hash, t->bcount);
                        t->buckets[backet] = entry;
                }
        } else {
                if (e_hash != en_hash)
                        t->buckets[backet] = NULL;

                entry->next = NULL;
                t->tail = entry;
        }

        --t->size;
        return next;
}

static void transfer(struct cdc_hash_table *t, struct cdc_hash_table_entry **tail,
                     struct cdc_hash_table_entry **backets, size_t count)
{
        struct cdc_hash_table_entry *curr = t->buckets[0]->next, *next;

        backets[0]->next = NULL;
        while (curr) {
                next = curr->next;
                add_entry(curr, tail, backets, count);
                curr = next;
        }

        (*tail)->next = NULL;
}

static enum cdc_stat reallocate(struct cdc_hash_table *t, size_t count)
{
        struct cdc_hash_table_entry **new_backets;
        struct cdc_hash_table_entry *tail, *nil;

        if (count < HASH_TABLE_MIN_CAPACITY) {
                if (t->bcount > HASH_TABLE_MIN_CAPACITY)
                        count = HASH_TABLE_MIN_CAPACITY;
                else
                        return CDC_STATUS_OK;
        }

        new_backets = (struct cdc_hash_table_entry **)calloc(count * sizeof (void *), 1);
        if (!new_backets)
                return CDC_STATUS_BAD_ALLOC;

        if (t->buckets == NULL) {
                nil = (struct cdc_hash_table_entry *)calloc(
                                        sizeof(struct cdc_hash_table_entry), 1);
                if (!nil) {
                        free(new_backets);
                        return CDC_STATUS_BAD_ALLOC;
                }

                new_backets[0] = nil;
                tail = nil;
        } else {
                tail = new_backets[0] = t->buckets[0];
                transfer(t, &tail, new_backets, count);
                free(t->buckets);
        }

        t->bcount  = count;
        t->buckets = new_backets;
        t->tail    = tail;
        return CDC_STATUS_OK;
}

static enum cdc_stat init_varg(struct cdc_hash_table *t, va_list args)
{
        enum cdc_stat stat;
        struct cdc_pair *pair;

        while ((pair = va_arg(args, struct cdc_pair *)) != NULL) {
                stat = cdc_hash_table_insert(t, pair->first, pair->second, NULL);
                if (stat != CDC_STATUS_OK)
                        return stat;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_hash_table_ctor1(struct cdc_hash_table **t,
                                   struct cdc_data_info *info,
                                   cdc_hash_fn_t hash, cdc_binary_pred_fn_t equal,
                                   float load_factor)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);
        assert(load_factor > 0);

        struct cdc_hash_table *tmp;
        enum cdc_stat stat;

        tmp = (struct cdc_hash_table *)calloc(sizeof(struct cdc_hash_table), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->hash = hash;
        tmp->eq = equal;
        tmp->load_factor = load_factor;
        if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
                stat = CDC_STATUS_BAD_ALLOC;
                goto error1;
        }

        stat = reallocate(tmp, HASH_TABLE_MIN_CAPACITY);
        if (stat != CDC_STATUS_OK)
                goto error2;

        *t = tmp;
        return CDC_STATUS_OK;
error2:
        cdc_di_shared_dtor(tmp->dinfo);
error1:
        free(tmp);
        return stat;
}

enum cdc_stat cdc_hash_table_ctorl1(struct cdc_hash_table **t,
                                    struct cdc_data_info *info,
                                    cdc_hash_fn_t hash, cdc_binary_pred_fn_t equal,
                                    float load_factor, ...)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);
        assert(load_factor > 0);

        enum cdc_stat stat;
        va_list args;

        va_start(args, load_factor);
        stat = cdc_hash_table_ctorv1(t, info, hash, equal, load_factor, args);
        va_end(args);

        return stat;
}

enum cdc_stat cdc_hash_table_ctorv1(struct cdc_hash_table **t,
                                    struct cdc_data_info *info,
                                    cdc_hash_fn_t hash, cdc_binary_pred_fn_t equal,
                                    float load_factor, va_list args)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);
        assert(load_factor > 0);

        enum cdc_stat stat;

        stat = cdc_hash_table_ctor1(t, info, hash, equal, load_factor);
        if (stat != CDC_STATUS_OK)
                return stat;

        return init_varg(*t, args);
}

enum cdc_stat cdc_hash_table_ctor(struct cdc_hash_table **t,
                                  struct cdc_data_info *info,
                                  cdc_hash_fn_t hash, cdc_binary_pred_fn_t equal)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);

        return cdc_hash_table_ctor1(t, info, hash, equal, HASH_TABLE_LOAD_FACTOR);

}


enum cdc_stat cdc_hash_table_ctorl(struct cdc_hash_table **t,
                                   struct cdc_data_info *info,
                                   cdc_hash_fn_t hash,
                                   cdc_binary_pred_fn_t equal, ...)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);

        enum cdc_stat stat;
        va_list args;

        va_start(args, equal);
        stat = cdc_hash_table_ctorv(t, info, hash, equal, args);
        va_end(args);

        return stat;
}


enum cdc_stat cdc_hash_table_ctorv(struct cdc_hash_table **t,
                                   struct cdc_data_info *info,
                                   cdc_hash_fn_t hash, cdc_binary_pred_fn_t equal,
                                   va_list args)
{
        assert(t != NULL);
        assert(hash != NULL);
        assert(equal != NULL);

        return cdc_hash_table_ctorv1(t, info, hash, equal, HASH_TABLE_LOAD_FACTOR, args);
}

void cdc_hash_table_dtor(struct cdc_hash_table *t)
{
        assert(t != NULL);

        free_all_entries(t);
        cdc_di_shared_dtor(t->dinfo);
        free(t);
}

enum cdc_stat cdc_hash_table_get(struct cdc_hash_table *t, void *key, void **value)
{
        assert(t != NULL);

        struct cdc_hash_table_entry *entry = find_entry(t, key);

        if (!entry)
                return CDC_STATUS_NOT_FOUND;

        *value = entry->next->value;
        return CDC_STATUS_OK;
}

size_t cdc_hash_table_count(struct cdc_hash_table *t, void *key)
{
        assert(t != NULL);

        return (size_t)(find_entry(t, key) != NULL);
}

struct cdc_hash_table_iter cdc_hash_table_find(struct cdc_hash_table *t,
                                               void *key)
{
        assert(t != NULL);

        struct cdc_hash_table_entry *entry = find_entry(t, key);
        struct cdc_hash_table_iter retval;

        retval.container = t;
        retval.current = (entry ? entry->next : NULL);
        return retval;
}

struct cdc_pair_hash_table_iter cdc_hash_table_equal_range(struct cdc_hash_table *t,
                                                           void *key)
{
        assert(t != NULL);

        struct cdc_hash_table_entry *entry = find_entry(t, key);
        struct cdc_pair_hash_table_iter retval;

        retval.first.container = retval.second.container = t;
        retval.first.current = (entry ? entry->next : NULL);
        retval.second.current = (entry ? entry->next->next : NULL);
        return retval;
}

void cdc_hash_table_clear(struct cdc_hash_table *t)
{
        assert(t != NULL);

        free_entries(t);
        t->buckets[0]->next = NULL;
        t->tail = t->buckets[0];
        t->size = 0;
}

enum cdc_stat cdc_hash_table_insert(struct cdc_hash_table *t,
                                    void *key, void *value,
                                    struct cdc_pair_hash_table_iter_bool *ret)
{
        assert(t != NULL);

        struct cdc_hash_table_entry *entry = find_entry(t, key), *new_entry;
        enum cdc_stat stat;

        if (!entry) {
                stat = make_and_insert_unique(t, key, value, &new_entry);
                if (stat != CDC_STATUS_OK)
                        return stat;
        }

        if (ret) {
                (*ret).first.container = t;
                (*ret).first.current = (entry ? entry->next : new_entry->next);
                (*ret).second = !entry;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_hash_table_insert_or_assign(struct cdc_hash_table *t,
                                              void *key, void *value,
                                              struct cdc_pair_hash_table_iter_bool *ret)
{
        assert(t != NULL);

        struct cdc_hash_table_entry *entry = find_entry(t, key), *new_entry;
        enum cdc_stat stat;

        if (!entry) {
                stat = make_and_insert_unique(t, key, value, &new_entry);
                if (stat != CDC_STATUS_OK)
                        return stat;
        } else {
                entry->next->value = value;
        }

        if (ret) {
                (*ret).first.container = t;
                (*ret).first.current = (entry ? entry->next : new_entry->next);
                (*ret).second = !entry;
        }

        return CDC_STATUS_OK;
}

size_t cdc_hash_table_erase(struct cdc_hash_table *t, void *key)
{
        assert(t != NULL);

        size_t backet = key ? cbacket(t->hash(key), t->bcount) : 0;
        struct cdc_hash_table_entry *entry = ffind_entry(t, key, backet);

        if (!entry)
                return 0;

        erase_entry(t, entry, backet);
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
        CDC_SWAP(cdc_hash_fn_t, a->hash, b->hash);
        CDC_SWAP(cdc_binary_pred_fn_t, a->eq, b->eq);
        CDC_SWAP(size_t, a->size,  b->size);
        CDC_SWAP(struct cdc_data_info *, a->dinfo,  b->dinfo);
}

enum cdc_stat cdc_hash_table_rehash(struct cdc_hash_table *t, size_t count)
{
        assert(t != NULL);

        if (count <= t->bcount)
                return CDC_STATUS_OK;

        count = cdc_up_to_pow2(count);
        assert(count != 0);
        return reallocate(t, count);
}

enum cdc_stat cdc_hash_table_reserve(struct cdc_hash_table *t, size_t count)
{
        assert(t != NULL);

        return cdc_hash_table_rehash(t,  ((float)count / t->load_factor) + 1);
}
