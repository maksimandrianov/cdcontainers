#include "cdcontainers/stackv.h"

#include <assert.h>
#include "cdcontainers/vector.h"
#include "cdcontainers/common.h"

struct cdc_stack_vector {
        cdc_vector_t *vector;
};

enum cdc_stat cdc_stackv_ctor(cdc_stackv_t **s, void (*fp_free)(void *))
{
        cdc_stackv_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_stackv_t *)malloc(sizeof(cdc_stackv_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

         ret = cdc_vector_ctor(&tmp->vector, fp_free);
         if (ret != CDC_STATUS_OK) {
                 free(tmp);
                 return ret;
         }

         *s = tmp;

         return CDC_STATUS_OK;
}

enum cdc_stat cdc_stackv_ctor_l(cdc_stackv_t **s, void (*fp_free)(void *), ...)
{

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
        assert(cdc_vector_size(s->vector) > 0);

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
