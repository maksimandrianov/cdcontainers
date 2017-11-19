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
#include "cdcontainers/list.h"

static enum cdc_stat queuel_ctor(void **cntr, struct cdc_data_info *info)
{
        assert(cntr != NULL);

        struct cdc_list **list = (struct cdc_list **)cntr;

        return cdc_list_ctor(list, info);
}

static enum cdc_stat queuel_ctorv(void **cntr, struct cdc_data_info *info,
                                  va_list args)
{
        assert(cntr != NULL);

        struct cdc_list **list = (struct cdc_list **)cntr;

        return cdc_list_ctorv(list, info, args);
}

static void queuel_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        cdc_list_dtor(list);
}

static void *queuel_front(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_front(list);
}

static void *queuel_back(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_back(list);
}

static bool queuel_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_empty(list);
}

static size_t queuel_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_size(list);
}

static enum cdc_stat queuel_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_push_back(list, elem);
}

static enum cdc_stat queuel_pop(void *cntr)
{
        assert(cntr != NULL);
        assert(queuel_size(cntr) > 0);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_pop_front(list);
}

static const struct cdc_queue_table _queuel_table = {
        .ctor = queuel_ctor,
        .ctorv = queuel_ctorv,
        .dtor = queuel_dtor,
        .front = queuel_front,
        .back = queuel_back,
        .empty = queuel_empty,
        .size = queuel_size,
        .push = queuel_push,
        .pop = queuel_pop
};

const void *cdc_queuel_table = &_queuel_table;
