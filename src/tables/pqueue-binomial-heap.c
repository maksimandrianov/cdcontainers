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
#include <assert.h>
#include "cdcontainers/interfaces/ipqueue.h"
#include "cdcontainers/binomial-heap.h"

static enum cdc_stat ctor(void **cntr, struct cdc_data_info *info,
                          cdc_compar_fn_t compar)
{
        assert(cntr != NULL);

        struct cdc_binomial_heap **heap = (struct cdc_binomial_heap **)cntr;

        return cdc_binomial_heap_ctor(heap, info, compar);
}

static enum cdc_stat ctorv(void **cntr, struct cdc_data_info *info,
                           cdc_compar_fn_t compar, va_list args)
{
        assert(cntr != NULL);

        struct cdc_binomial_heap **heap = (struct cdc_binomial_heap **)cntr;

        return cdc_binomial_heap_ctorv(heap, info, compar, args);
}

static void dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_binomial_heap *heap = (struct cdc_binomial_heap *)cntr;

        cdc_binomial_heap_dtor(heap);
}

static void *top(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_binomial_heap *heap = (struct cdc_binomial_heap *)cntr;

        return cdc_binomial_heap_top(heap);
}

static bool empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_binomial_heap *heap = (struct cdc_binomial_heap *)cntr;

        return cdc_binomial_heap_empty(heap);
}

static size_t size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_binomial_heap *heap = (struct cdc_binomial_heap *)cntr;

        return cdc_binomial_heap_size(heap);
}

static enum cdc_stat push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_binomial_heap *heap = (struct cdc_binomial_heap *)cntr;

        return cdc_binomial_heap_insert(heap, elem);
}

static enum cdc_stat pop(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_binomial_heap *heap = (struct cdc_binomial_heap *)cntr;

        return cdc_binomial_heap_extract_top(heap);
}

static const struct cdc_priority_queue_table _table = {
        .ctor = ctor,
        .ctorv = ctorv,
        .dtor = dtor,
        .top = top,
        .empty = empty,
        .size = size,
        .push = push,
        .pop = pop
};

const void *cdc_pq_binheap = &_table;
