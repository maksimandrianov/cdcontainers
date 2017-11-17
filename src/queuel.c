#include "cdcontainers/queuel.h"

#include <assert.h>
#include "cdcontainers/list.h"
#include "cdcontainers/common.h"

struct cdc_queue_list {
        struct cdc_list *list;
};

enum cdc_stat cdc_queuel_ctor(cdc_queuel_t **q, cdc_free_func_t func)
{
        assert(q != NULL);

        cdc_queuel_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_queuel_t *)malloc(sizeof(cdc_queuel_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        ret = cdc_list_ctor(&tmp->list, func);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *q = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_queuel_ctorl(cdc_queuel_t **q, cdc_free_func_t func, ...)
{
        assert(q != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, func);
        ret = cdc_queuel_ctorv(q, func, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_queuel_ctorv(cdc_queuel_t **q, cdc_free_func_t func,
                               va_list args)
{
        assert(q != NULL);

        cdc_queuel_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_queuel_t *)malloc(sizeof(cdc_queuel_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        *q = tmp;
        ret = cdc_list_ctorv(&(*q)->list, func, args);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        return ret;
}

void cdc_queuel_dtor(cdc_queuel_t *q)
{
        assert(q != NULL);

        cdc_list_dtor(q->list);
        free(q);
}

void *cdc_queuel_front(cdc_queuel_t *q)
{
        assert(q != NULL);

        return cdc_list_front(q->list);
}

void *cdc_queuel_back(cdc_queuel_t *q)
{
        assert(q != NULL);

        return cdc_list_back(q->list);
}

bool cdc_queuel_empty(cdc_queuel_t *q)
{
        assert(q != NULL);

        return cdc_list_empty(q->list);
}

size_t cdc_queuel_size(cdc_queuel_t *q)
{
        assert(q != NULL);

        return cdc_list_size(q->list);
}

enum cdc_stat cdc_queuel_push(cdc_queuel_t *q, void *elem)
{
        assert(q != NULL);

        return cdc_list_push_back(q->list, elem);
}

enum cdc_stat cdc_queuel_pop(cdc_queuel_t *q)
{
        assert(q != NULL);
        assert(cdc_queuel_size(q) > 0);

        return cdc_list_pop_front(q->list);
}

void cdc_queuel_swap(cdc_queuel_t *a, cdc_queuel_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_list *, a->list, b->list);
}
