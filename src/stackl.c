#include "cdcontainers/stack.h"

#include <assert.h>
#include "cdcontainers/list.h"

static enum cdc_stat cdc_stackl_ctor(void **cntr, struct cdc_data_info *info)
{
        assert(cntr != NULL);

        struct cdc_list **list = (struct cdc_list **)cntr;

        return cdc_list_ctor(list, info);
}

static enum cdc_stat cdc_stackl_ctorv(void **cntr,
                                      struct cdc_data_info *info, va_list args)
{
        assert(cntr != NULL);

        struct cdc_list **list = (struct cdc_list **)cntr;

        return cdc_list_ctorv(list, info, args);
}

static enum cdc_stat cdc_stackl_ctorl(void **cntr,
                                      struct cdc_data_info *info, ...)
{
        assert(cntr != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = cdc_stackl_ctorv(cntr, info, args);
        va_end(args);

        return ret;
}

static void *cdc_stackl_top(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_back(list);
}

static bool cdc_stackl_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_empty(list);
}

static size_t cdc_stackl_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_size(list);
}

static enum cdc_stat cdc_stackl_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_push_back(list, elem);
}

static enum cdc_stat cdc_stackl_pop(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        return cdc_list_pop_back(list);
}


static void cdc_stackl_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_list *list = (struct cdc_list *)cntr;

        cdc_list_dtor(list);
}

static const struct cdc_stack_table _cdc_stackl_table = {
        .ctor = cdc_stackl_ctor,
        .ctorv = cdc_stackl_ctorv,
        .dtor = cdc_stackl_dtor,
        .top = cdc_stackl_top,
        .empty = cdc_stackl_empty,
        .size = cdc_stackl_size,
        .push = cdc_stackl_push,
        .pop = cdc_stackl_pop
};

const void *cdc_stackl_table = &_cdc_stackl_table;
