#include "cdcontainers/stackv.h"

#include <assert.h>
#include "cdcontainers/vector.h"
#include "cdcontainers/common.h"

struct cdc_stack_vector {
        cdc_vector_t *vector;
};

enum cdc_stat cdc_stackv_ctor(cdc_stackv_t **s, cdc_free_func_t func)
{
        assert(s != NULL);

        cdc_stackv_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_stackv_t *)malloc(sizeof(cdc_stackv_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

         ret = cdc_vector_ctor(&tmp->vector, func);
         if (ret != CDC_STATUS_OK) {
                 free(tmp);
                 return ret;
         }

         *s = tmp;

         return CDC_STATUS_OK;
}

enum cdc_stat cdc_stackv_ctorl(cdc_stackv_t **s, cdc_free_func_t func, ...)
{
        assert(s != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, func);
        ret = cdc_stackv_ctorv(s, func, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_stackv_ctorv(cdc_stackv_t **s, cdc_free_func_t func,
                               va_list args)
{
        assert(s != NULL);

        cdc_stackv_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_stackv_t *)malloc(sizeof(cdc_stackv_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        *s = tmp;
        ret = cdc_vector_ctorv(&(*s)->vector, func, args);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        return ret;
}

void cdc_stackv_dtor(cdc_stackv_t *s)
{
        assert(s != NULL);

        cdc_vector_dtor(s->vector);
        free(s);
}

void *cdc_stackv_top(cdc_stackv_t *s)
{
        assert(s != NULL);
        assert(cdc_stackv_size(s) > 0);

        return cdc_vector_back(s->vector);
}

bool cdc_stackv_empty(cdc_stackv_t *s)
{
      assert(s != NULL);

      return cdc_vector_empty(s->vector);
}

size_t cdc_stackv_size(cdc_stackv_t *s)
{
        assert(s != NULL);

        return cdc_vector_size(s->vector);
}

enum cdc_stat cdc_stackv_push(cdc_stackv_t *s, void *elem)
{
        assert(s != NULL);

        return cdc_vector_push_back(s->vector, elem);
}

enum cdc_stat cdc_stackv_pop(cdc_stackv_t *s)
{
        assert(s != NULL);
        assert(cdc_vector_size(s->vector) > 0);

        return cdc_vector_pop_back(s->vector);
}

void cdc_stackv_swap(cdc_stackv_t *a, cdc_stackv_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(cdc_vector_t *, a->vector, b->vector);
}
