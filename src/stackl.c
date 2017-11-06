#include "cdcontainers/stackl.h"

#include <assert.h>
#include "cdcontainers/list.h"
#include "cdcontainers/common.h"

struct cdc_stack_list {
        cdc_list_t *list;
};

enum cdc_stat cdc_stackl_ctor(cdc_stackl_t **s, cdc_free_func_t func)
{
        assert(s != NULL);

        cdc_stackl_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_stackl_t *)malloc(sizeof(cdc_stackl_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

         ret = cdc_list_ctor(&tmp->list, func);
         if (ret != CDC_STATUS_OK) {
                 free(tmp);
                 return ret;
         }

         *s = tmp;

         return CDC_STATUS_OK;
}

enum cdc_stat cdc_stackl_ctorl(cdc_stackl_t **s, cdc_free_func_t func, ...)
{
        assert(s != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, func);
        ret = cdc_stackl_ctorv(s, func, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_stackl_ctorv(cdc_stackl_t **s, cdc_free_func_t func,
                               va_list args)
{
        assert(s != NULL);

        enum cdc_stat ret;

        ret = cdc_stackl_ctor(s, func);
        if (ret != CDC_STATUS_OK)
                return ret;

        return cdc_list_ctorv(&(*s)->list, func, args);
}

void cdc_stackl_dtor(cdc_stackl_t *s)
{
        assert(s != NULL);

        cdc_list_dtor(s->list);
        free(s);
}

void *cdc_stackl_top(cdc_stackl_t *s)
{
        assert(s != NULL);
        assert(cdc_list_size(s->list) > 0);

        return cdc_list_back(s->list);
}

bool cdc_stackl_empty(cdc_stackl_t *s)
{
      assert(s != NULL);

      return cdc_list_empty(s->list);
}

size_t cdc_stackl_size(cdc_stackl_t *s)
{
        assert(s != NULL);

        return cdc_list_size(s->list);
}

enum cdc_stat cdc_stackl_push(cdc_stackl_t *s, void *elem)
{
        assert(s != NULL);

        return cdc_list_push_back(s->list, elem);
}

enum cdc_stat cdc_stackl_pop(cdc_stackl_t *s)
{
        assert(s != NULL);
        assert(cdc_list_size(s->list) > 0);

        return cdc_list_pop_back(s->list);
}

void cdc_stackl_swap(cdc_stackl_t *a, cdc_stackl_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(cdc_list_t *, a->list, b->list);
}
