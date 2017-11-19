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

enum cdc_stat cdc_queue_ctor(const struct cdc_queue_table *table,
                             struct cdc_queue **q, struct cdc_data_info *info)
{
        assert(table != NULL);
        assert(q != NULL);

        struct cdc_queue *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_queue *)malloc(sizeof(struct cdc_queue));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->table = table;
        ret = tmp->table->ctor(&tmp->container, info);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *q = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_queue_ctorl(const struct cdc_queue_table *table,
                              struct cdc_queue **q,
                              struct cdc_data_info *info, ...)
{
        assert(table != NULL);
        assert(q != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = cdc_queue_ctorv(table, q, info, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_queue_ctorv(const struct cdc_queue_table *table,
                              struct cdc_queue **q,
                              struct cdc_data_info *info, va_list args)
{
        assert(table != NULL);
        assert(q != NULL);

        struct cdc_queue *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_queue *)malloc(sizeof(struct cdc_queue));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->table = table;
        ret = tmp->table->ctorv(&tmp->container, info, args);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *q = tmp;

        return ret;
}

void cdc_queue_dtor(struct cdc_queue *q)
{
        assert(q != NULL);

        q->table->dtor(q->container);
        free(q);
}

void cdc_queue_swap(struct cdc_queue *a, struct cdc_queue *b)
{
        assert(a != NULL);
        assert(b != NULL);
        assert(a->table == b->table);

        CDC_SWAP(void *, a->container, b->container);
}
