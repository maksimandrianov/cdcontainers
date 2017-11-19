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
#include "cdcontainers/queue.h"

#include <assert.h>
#include "cdcontainers/deque.h"

static enum cdc_stat queued_ctor(void **cntr, struct cdc_data_info *info)
{
        assert(cntr != NULL);

        struct cdc_deque **deque = (struct cdc_deque **)cntr;

        return cdc_deque_ctor(deque, info);
}

static enum cdc_stat queued_ctorv(void **cntr, struct cdc_data_info *info,
                                  va_list args)
{
        assert(cntr != NULL);

        struct cdc_deque **deque = (struct cdc_deque **)cntr;

        return cdc_deque_ctorv(deque, info, args);
}

static void queued_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        cdc_deque_dtor(deque);
}

static void *queued_front(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_front(deque);
}

static void *queued_back(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_back(deque);
}

static bool queued_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_empty(deque);
}

static size_t queued_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_size(deque);
}

static enum cdc_stat queued_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_push_back(deque, elem);
}

static enum cdc_stat queued_pop(void *cntr)
{
        assert(cntr != NULL);
        assert(queued_size(cntr) > 0);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_pop_front(deque);
}

static const struct cdc_queue_table _queued_table = {
        .ctor = queued_ctor,
        .ctorv = queued_ctorv,
        .dtor = queued_dtor,
        .front = queued_front,
        .back = queued_back,
        .empty = queued_empty,
        .size = queued_size,
        .push = queued_push,
        .pop = queued_pop
};

const void *cdc_queued_table = &_queued_table;
