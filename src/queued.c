#include "cdcontainers/queue.h"

#include <assert.h>
#include "cdcontainers/deque.h"

static enum cdc_stat cdc_queued_ctor(void **cntr, struct cdc_data_info *info)
{
        assert(cntr != NULL);

        struct cdc_deque **deque = (struct cdc_deque **)cntr;

        return cdc_deque_ctor(deque, info);
}

static enum cdc_stat cdc_queued_ctorv(void **cntr,
                                      struct cdc_data_info *info, va_list args)
{
        assert(cntr != NULL);

        struct cdc_deque **deque = (struct cdc_deque **)cntr;

        return cdc_deque_ctorv(deque, info, args);
}

static enum cdc_stat cdc_queued_ctorl(void **cntr,
                                      struct cdc_data_info *info, ...)
{
        assert(cntr != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = cdc_queued_ctorv(cntr, info, args);
        va_end(args);

        return ret;
}

static void cdc_queued_dtor(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        cdc_deque_dtor(deque);
}

static void *cdc_queued_front(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_front(deque);
}

static void *cdc_queued_back(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_back(deque);
}

static bool cdc_queued_empty(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_empty(deque);
}

static size_t cdc_queued_size(void *cntr)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_size(deque);
}

static enum cdc_stat cdc_queued_push(void *cntr, void *elem)
{
        assert(cntr != NULL);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_push_back(deque, elem);
}

static enum cdc_stat cdc_queued_pop(void *cntr)
{
        assert(cntr != NULL);
        assert(cdc_queued_size(cntr) > 0);

        struct cdc_deque *deque = (struct cdc_deque *)cntr;

        return cdc_deque_pop_front(deque);
}

static const struct cdc_queue_table _cdc_queued_table = {
        .ctor = cdc_queued_ctor,
        .ctorv = cdc_queued_ctorv,
        .dtor = cdc_queued_dtor,
        .front = cdc_queued_front,
        .back = cdc_queued_back,
        .empty = cdc_queued_empty,
        .size = cdc_queued_size,
        .push = cdc_queued_push,
        .pop = cdc_queued_pop
};

const void *cdc_queued_table = &_cdc_queued_table;
