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
#include "cdcontainers/vector.h"

static enum cdc_stat stackv_ctor(void **cntr, struct cdc_data_info *info)
{
        assert(cntr != NULL);

        struct cdc_vector **vector = (struct cdc_vector **)cntr;

        return cdc_vector_ctor(vector, info);
}

static enum cdc_stat stackv_ctorv(void **cntr, struct cdc_data_info *info,
                                  va_list args)
{
        assert(cntr != NULL);

        struct cdc_vector **vector = (struct cdc_vector **)cntr;

        return cdc_vector_ctorv(vector, info, args);
}

static void *stackv_top(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_back(vector);
}

static bool stackv_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_empty(vector);
}

static size_t stackv_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_size(vector);
}

static enum cdc_stat stackv_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_push_back(vector, elem);
}

static enum cdc_stat stackv_pop(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_pop_back(vector);
}


static void stackv_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        cdc_vector_dtor(vector);
}

static const struct cdc_stack_table _stackv_table = {
        .ctor = stackv_ctor,
        .ctorv = stackv_ctorv,
        .dtor = stackv_dtor,
        .top = stackv_top,
        .empty = stackv_empty,
        .size = stackv_size,
        .push = stackv_push,
        .pop = stackv_pop
};

const void *cdc_stackv_table = &_stackv_table;

