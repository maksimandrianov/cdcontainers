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
#include "cdcontainers/stack.h"

#include <assert.h>
#include "cdcontainers/list.h"

static enum cdc_stat stackl_ctor(void **cntr, struct cdc_data_info *info)
{
        assert(cntr != NULL);

        struct cdc_list **list = (struct cdc_list **)cntr;

        return cdc_list_ctor(list, info);
}

static enum cdc_stat stackl_ctorv(void **cntr, struct cdc_data_info *info,
                                  va_list args)
{
        assert(cntr != NULL);

        struct cdc_list **list = (struct cdc_list **)cntr;

        return cdc_list_ctorv(list, info, args);
}

static void *stackl_top(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_back(list);
}

static bool stackl_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_empty(list);
}

static size_t stackl_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_size(list);
}

static enum cdc_stat stackl_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_push_back(list, elem);
}

static enum cdc_stat stackl_pop(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_pop_back(list);
}


static void stackl_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        cdc_list_dtor(list);
}

static const struct cdc_stack_table _stackl_table = {
        .ctor = stackl_ctor,
        .ctorv = stackl_ctorv,
        .dtor = stackl_dtor,
        .top = stackl_top,
        .empty = stackl_empty,
        .size = stackl_size,
        .push = stackl_push,
        .pop = stackl_pop
};

const void *cdc_stackl_table = &_stackl_table;
