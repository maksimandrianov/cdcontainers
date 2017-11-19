#include "cdcontainers/priority-queue.h"

enum cdc_stat cdc_priority_queue_ctor(const struct cdc_priority_queue_table *table,
                                      struct cdc_priority_queue **q,
                                      struct cdc_data_info *info,
                                      cdc_compar_func_t compar)
{
        assert(table != NULL);
        assert(q != NULL);

        struct cdc_priority_queue *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_priority_queue *)malloc(
                                sizeof(struct cdc_priority_queue));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->table = table;
        ret = tmp->table->ctor(&tmp->container, info, compar);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *q = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_priority_queue_ctorl(const struct cdc_priority_queue_table *table,
                                       struct cdc_priority_queue **q,
                                       struct cdc_data_info *info,
                                       cdc_compar_func_t compar, ...)
{
        assert(table != NULL);
        assert(q != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, compar);
        ret = cdc_priority_queue_ctorv(table, q, info, compar, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_priority_queue_ctorv(const struct cdc_priority_queue_table *table,
                                       struct cdc_priority_queue **q,
                                       struct cdc_data_info *info,
                                       cdc_compar_func_t compar, va_list args)
{
        assert(table != NULL);
        assert(q != NULL);

        struct cdc_priority_queue *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_priority_queue *)malloc(
                                sizeof(struct cdc_priority_queue));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->table = table;
        ret = tmp->table->ctorv(&tmp->container, info, compar, args);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *q = tmp;

        return ret;
}

void cdc_priority_queue_dtor(struct cdc_priority_queue *q)
{
        assert(q != NULL);

        q->table->dtor(q->container);
        free(q);
}

void cdc_priority_queue_swap(struct cdc_priority_queue *a,
                             struct cdc_priority_queue *b)
{
        assert(a != NULL);
        assert(b != NULL);
        assert(a->table == b->table);

        CDC_SWAP(void *, a->container, b->container);
}
