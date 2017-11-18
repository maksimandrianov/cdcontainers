#include "cdcontainers/queue.h"

#include <assert.h>
#include "cdcontainers/list.h"

static enum cdc_stat cdc_queuel_ctor(void **cntr, struct cdc_data_info *info)
{
        assert(cntr != NULL);

        struct cdc_list **list = (struct cdc_list **)cntr;

        return cdc_list_ctor(list, info);
}

static enum cdc_stat cdc_queuel_ctorv(void **cntr,
                                      struct cdc_data_info *info, va_list args)
{
        assert(cntr != NULL);

        struct cdc_list **list = (struct cdc_list **)cntr;

        return cdc_list_ctorv(list, info, args);
}

static enum cdc_stat cdc_queuel_ctorl(void **cntr,
                                      struct cdc_data_info *info, ...)
{
        assert(cntr != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = cdc_queuel_ctorv(cntr, info, args);
        va_end(args);

        return ret;
}

static void cdc_queuel_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        cdc_list_dtor(list);
}

static void *cdc_queuel_front(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_front(list);
}

static void *cdc_queuel_back(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_back(list);
}

static bool cdc_queuel_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_empty(list);
}

static size_t cdc_queuel_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_size(list);
}

static enum cdc_stat cdc_queuel_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_push_back(list, elem);
}

static enum cdc_stat cdc_queuel_pop(void *cntr)
{
        assert(cntr != NULL);
        assert(cdc_queuel_size(cntr) > 0);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_pop_front(list);
}

static const struct cdc_queue_table _cdc_queuel_table = {
        .ctor = cdc_queuel_ctor,
        .ctorv = cdc_queuel_ctorv,
        .dtor = cdc_queuel_dtor,
        .front = cdc_queuel_front,
        .back = cdc_queuel_back,
        .empty = cdc_queuel_empty,
        .size = cdc_queuel_size,
        .push = cdc_queuel_push,
        .pop = cdc_queuel_pop
};

const void *cdc_queuel_table = &_cdc_queuel_table;
