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

static enum cdc_stat stackv_ctorl(void **cntr, struct cdc_data_info *info, ...)
{
        assert(cntr != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = stackv_ctorv(cntr, info, args);
        va_end(args);

        return ret;
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

