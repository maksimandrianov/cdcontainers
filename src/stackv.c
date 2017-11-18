#include "cdcontainers/stack.h"

#include <assert.h>
#include "cdcontainers/vector.h"

static enum cdc_stat cdc_stackv_ctor(void **cntr, struct cdc_data_info *info)
{
        assert(cntr != NULL);

        struct cdc_vector **vector = (struct cdc_vector **)cntr;

        return cdc_vector_ctor(vector, info);
}

static enum cdc_stat cdc_stackv_ctorv(void **cntr,
                                      struct cdc_data_info *info, va_list args)
{
        assert(cntr != NULL);

        struct cdc_vector **vector = (struct cdc_vector **)cntr;

        return cdc_vector_ctorv(vector, info, args);
}

static enum cdc_stat cdc_stackv_ctorl(void **cntr,
                                      struct cdc_data_info *info, ...)
{
        assert(cntr != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = cdc_stackv_ctorv(cntr, info, args);
        va_end(args);

        return ret;
}

static void *cdc_stackv_top(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_back(vector);
}

static bool cdc_stackv_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_empty(vector);
}

static size_t cdc_stackv_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_size(vector);
}

static enum cdc_stat cdc_stackv_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_push_back(vector, elem);
}

static enum cdc_stat cdc_stackv_pop(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        return cdc_vector_pop_back(vector);
}


static void cdc_stackv_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_vector *vector = (struct cdc_vector *)cntr;

        cdc_vector_dtor(vector);
}

static const struct cdc_stack_table _cdc_stackv_table = {
        .ctor = cdc_stackv_ctor,
        .ctorv = cdc_stackv_ctorv,
        .dtor = cdc_stackv_dtor,
        .top = cdc_stackv_top,
        .empty = cdc_stackv_empty,
        .size = cdc_stackv_size,
        .push = cdc_stackv_push,
        .pop = cdc_stackv_pop
};

const void *cdc_stackv_table = &_cdc_stackv_table;

