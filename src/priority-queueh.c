#include "cdcontainers/priority-queue.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "cdcontainers/heap.h"
#include "cdcontainers/priority-queue.h"

static enum cdc_stat priority_queueh_ctor(void **cntr,
                                          struct cdc_data_info *info,
                                          cdc_compar_func_t compar)
{
        assert(cntr != NULL);

        struct cdc_heap **heap = (struct cdc_heap **)cntr;

        return cdc_heap_ctor(heap, info, compar);
}

static enum cdc_stat priority_queueh_ctorv(void **cntr,
                                           struct cdc_data_info *info,
                                           cdc_compar_func_t compar,
                                           va_list args)
{
        assert(cntr != NULL);

        struct cdc_heap **heap = (struct cdc_heap **)cntr;

        return cdc_heap_ctorv(heap, info, compar, args);
}

static void priority_queueh_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_heap *heap = (struct cdc_heap *)cntr;

        cdc_heap_dtor(heap);
}

static void *priority_queueh_top(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_heap *heap = (struct cdc_heap *)cntr;

        return cdc_heap_top(heap);
}

static bool priority_queueh_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_heap *heap = (struct cdc_heap *)cntr;

        return cdc_heap_empty(heap);
}

static size_t priority_queueh_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_heap *heap = (struct cdc_heap *)cntr;

        return cdc_heap_size(heap);
}

static enum cdc_stat priority_queueh_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_heap *heap = (struct cdc_heap *)cntr;

        return cdc_heap_insert(heap, elem);
}

static enum cdc_stat priority_queueh_pop(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_heap *heap = (struct cdc_heap *)cntr;

        return cdc_heap_extract_top(heap);
}

static const struct cdc_priority_queue_table _priority_queueh_table = {
        .ctor = priority_queueh_ctor,
        .ctorv = priority_queueh_ctorv,
        .dtor = priority_queueh_dtor,
        .top = priority_queueh_top,
        .empty = priority_queueh_empty,
        .size = priority_queueh_size,
        .push = priority_queueh_push,
        .pop = priority_queueh_pop
};

const void *cdc_priority_queueh_table = &_priority_queueh_table;

