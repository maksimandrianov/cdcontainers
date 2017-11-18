#include "cdcontainers/queued.h"

#include <assert.h>
#include "cdcontainers/deque.h"
#include "cdcontainers/common.h"

struct cdc_queue_deque {
        struct cdc_deque *deque;
};

enum cdc_stat cdc_queued_ctor(cdc_queued_t **q, cdc_free_func_t func)
{
        assert(q != NULL);

        cdc_queued_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_queued_t *)malloc(sizeof(cdc_queued_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        ret = cdc_deque_ctor(&tmp->deque, func);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *q = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_queued_ctorl(cdc_queued_t **q, cdc_free_func_t func, ...)
{
        assert(q != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, func);
        ret = cdc_queued_ctorv(q, func, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_queued_ctorv(cdc_queued_t **q, cdc_free_func_t func,
                               va_list args)
{
        assert(q != NULL);

        cdc_queued_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_queued_t *)malloc(sizeof(cdc_queued_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        *q = tmp;
        ret = cdc_deque_ctorv(&(*q)->deque, func, args);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        return ret;
}

void cdc_queued_dtor(cdc_queued_t *q)
{
        assert(q != NULL);

        cdc_deque_dtor(q->deque);
        free(q);
}

void *cdc_queued_front(cdc_queued_t *q)
{
        assert(q != NULL);

        return cdc_deque_front(q->deque);
}

void *cdc_queued_back(cdc_queued_t *q)
{
        assert(q != NULL);

        return cdc_deque_back(q->deque);
}

bool cdc_queued_empty(cdc_queued_t *q)
{
        assert(q != NULL);

        return cdc_deque_empty(q->deque);
}

size_t cdc_queued_size(cdc_queued_t *q)
{
        assert(q != NULL);

        return cdc_deque_size(q->deque);
}

enum cdc_stat cdc_queued_push(cdc_queued_t *q, void *elem)
{
        assert(q != NULL);

        return cdc_deque_push_back(q->deque, elem);
}

enum cdc_stat cdc_queued_pop(cdc_queued_t *q)
{
        assert(q != NULL);
        assert(cdc_queued_size(q) > 0);

        return cdc_deque_pop_front(q->deque);
}

void cdc_queued_swap(cdc_queued_t *a, cdc_queued_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_deque *, a->deque, b->deque);
}
