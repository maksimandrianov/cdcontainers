#include "cdcontainers/priority-queue.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "cdcontainers/heap.h"

struct cdc_priority_queue {
        cdc_heap_t *heap;
};

enum cdc_stat cdc_priority_queue_ctor(cdc_priority_queue_t **q, cdc_free_func_t func,
                                      cdc_compar_func_t compar)
{
        assert(q != NULL);

        cdc_priority_queue_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_priority_queue_t *)malloc(sizeof(cdc_priority_queue_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        ret = cdc_heap_ctor(&tmp->heap, func, compar);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *q = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_priority_queue_ctorl(cdc_priority_queue_t **q, cdc_free_func_t func,
                                       cdc_compar_func_t compar, ...)
{
        assert(q != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, compar);
        ret = cdc_priority_queue_ctorv(q, func, compar, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_priority_queue_ctorv(cdc_priority_queue_t **q, cdc_free_func_t func,
                                       cdc_compar_func_t compar, va_list args)
{
        assert(q != NULL);

        cdc_priority_queue_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_priority_queue_t *)malloc(sizeof(cdc_priority_queue_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *q = tmp;
        ret = cdc_heap_ctorv(&(*q)->heap, func, compar, args);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        return ret;
}

void cdc_priority_queue_dtor(cdc_priority_queue_t *q)
{
        assert(q != NULL);

        cdc_heap_dtor(q->heap);
        free(q);
}

void *cdc_priority_queue_top(cdc_priority_queue_t *q)
{
        assert(q != NULL);

        return cdc_heap_top(q->heap);
}

bool cdc_priority_queue_empty(cdc_priority_queue_t *q)
{
        assert(q != NULL);

        return cdc_heap_empty(q->heap);
}

size_t cdc_priority_queue_size(cdc_priority_queue_t *q)
{
        assert(q != NULL);

        return cdc_heap_size(q->heap);
}

enum cdc_stat cdc_priority_queue_push(cdc_priority_queue_t *q, void *elem)
{
        assert(q != NULL);

        return cdc_heap_insert(q->heap, elem);
}

enum cdc_stat cdc_priority_queue_pop(cdc_priority_queue_t *q)
{
        assert(q != NULL);
        assert(cdc_priority_queue_size(q) > 0);

        return cdc_heap_extract_top(q->heap);
}

void cdc_priority_queue_swap(cdc_priority_queue_t *a, cdc_priority_queue_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(cdc_heap_t *, a->heap, b->heap);
}
