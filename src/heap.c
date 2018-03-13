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
#include "cdcontainers/heap.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "data-info.h"

static inline size_t parent(size_t i)
{
        return (i - 1) / 2;
}

static inline size_t left(size_t i)
{
        return 2 * i + 1;
}

static inline size_t right(size_t i)
{
        return 2 * i + 2;
}

static inline size_t sift_down(struct cdc_heap *h, size_t i)
{
        assert(h != NULL);

        size_t l, r, largest;
        size_t size = cdc_vector_size(h->vector);
        void **data = cdc_vector_data(h->vector);
        bool cond;

        do {
                l = left(i);
                r = right(i);

                if (l < size && h->compar(data[l], data[i]))
                        largest = l;
                else
                        largest = i;

                if (r < size && h->compar(data[r], data[largest]))
                        largest = r;

                cond = largest != i;
                if (cond) {
                        CDC_SWAP(void *, data[i], data[largest]);
                        i = largest;
                }

        } while (cond);

        return i;
}

static inline size_t sift_up(struct cdc_heap *h, size_t i)
{
        assert(h != NULL);
        assert(i < cdc_heap_size(h));

        void **data = cdc_vector_data(h->vector);
        size_t p = parent(i);

        while (i > 0 && h->compar(data[i], data[p])) {
                CDC_SWAP(void *, data[i], data[p]);
                i = parent(i);
                p = parent(i);
        }

        return i;
}

static inline void build_heap(struct cdc_heap *h)
{
        assert(h != NULL);

        ssize_t i;

        for (i = cdc_vector_size(h->vector) / 2; i >= 0; --i)
                sift_down(h, i);
}

static inline enum cdc_stat init_varg(struct cdc_heap *h, va_list args)
{
        assert(h != NULL);

        enum cdc_stat ret;
        void *elem;

        while ((elem = va_arg(args, void *)) != NULL) {
                ret = cdc_vector_push_back(h->vector, elem);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        build_heap(h);
        return CDC_STATUS_OK;
}

enum cdc_stat cdc_heap_ctor(struct cdc_heap **h, struct cdc_data_info *info,
                            cdc_binary_pred_fn_t compar)
{
        assert(h != NULL);
        assert(compar != NULL);

        struct cdc_heap *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_heap *)calloc(sizeof(struct cdc_heap), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->compar = compar;
        ret = cdc_vector_ctor(&tmp->vector, info);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *h = tmp;
        return ret;
}

enum cdc_stat cdc_heap_ctorl(struct cdc_heap **h, struct cdc_data_info *info,
                             cdc_binary_pred_fn_t compar, ...)
{
        assert(h != NULL);
        assert(compar != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, compar);
        ret = cdc_heap_ctorv(h, info, compar, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_heap_ctorv(struct cdc_heap **h, struct cdc_data_info *info,
                             cdc_binary_pred_fn_t compar, va_list args)
{
        assert(h != NULL);
        assert(compar != NULL);

        enum cdc_stat ret;

        ret = cdc_heap_ctor(h, info, compar);
        if (ret != CDC_STATUS_OK)
                return ret;

        return init_varg(*h, args);
}

void cdc_heap_dtor(struct cdc_heap *h)
{
        assert(h != NULL);

        cdc_vector_dtor(h->vector);
        free(h);
}

enum cdc_stat cdc_heap_extract_top(struct cdc_heap *h)
{
        assert(h != NULL);
        assert(cdc_heap_size(h) > 0);

        enum cdc_stat ret;
        void *elem = cdc_vector_back(h->vector);

        ret = cdc_vector_pop_back(h->vector);
        if (ret != CDC_STATUS_OK)
                return ret;

        if (cdc_vector_empty(h->vector))
                return ret;

        cdc_vector_set(h->vector, 0, elem);
        sift_down(h, 0);
        return ret;
}

void cdc_heap_change_key(struct cdc_heap *h, struct cdc_heap_iter *pos,
                         void *key)
{
        assert(h != NULL);
        assert(pos != NULL);
        assert(h->vector == pos->container);

        void **data = cdc_vector_data(h->vector);
        bool is_sift_down = h->compar(data[pos->current], key);

        cdc_vector_set(h->vector, pos->current, key);
        if (is_sift_down)
                pos->current = sift_down(h, pos->current);
        else
                pos->current = sift_up(h, pos->current);
}

enum cdc_stat cdc_heap_riinsert(struct cdc_heap *h, void *key,
                                struct cdc_heap_iter *ret)
{
        assert(h != NULL);

        enum cdc_stat stat;
        size_t i;

        stat = cdc_vector_push_back(h->vector, key);
        if (stat != CDC_STATUS_OK)
                return stat;

        i = cdc_vector_size(h->vector) - 1;
        i = sift_up(h, i);
        if (ret) {
                ret->container = h->vector;
                ret->current = i;
        }

        return stat;
}

void cdc_heap_swap(struct cdc_heap *a, struct cdc_heap *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_vector *, a->vector, b->vector);
        CDC_SWAP(cdc_binary_pred_fn_t, a->compar, b->compar);
}

enum cdc_stat cdc_heap_merge(struct cdc_heap *h, struct cdc_heap *other)
{
        assert(h != NULL);
        assert(other != NULL);

        enum cdc_stat ret;

        ret = cdc_vector_vappend(h->vector, other->vector);
        if (ret != CDC_STATUS_OK)
                return ret;

        cdc_vector_cclear(other->vector, NULL);
        build_heap(h);
        return CDC_STATUS_OK;
}

bool cdc_heap_is_heap(struct cdc_heap *h)
{
        assert(h != NULL);

        size_t i, size = cdc_heap_size(h);;
        void **data = cdc_vector_data(h->vector);

        if (size == 0)
                return true;

        for (i = 1; i < size; ++i) {
                if (h->compar(data[i], data[parent(i)]))
                        return false;
        }

        return true;
}
