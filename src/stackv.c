#include "cdcontainers/stackv.h"

#include <assert.h>

enum cdc_stat cdc_stackv_ctor(struct cdc_stackv **s, struct cdc_data_info *info)
{
        assert(s != NULL);

        struct cdc_stackv *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_stackv *)malloc(sizeof(struct cdc_stackv));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

         ret = cdc_vector_ctor(&tmp->vector, info);
         if (ret != CDC_STATUS_OK) {
                 free(tmp);
                 return ret;
         }

         *s = tmp;

         return CDC_STATUS_OK;
}

enum cdc_stat cdc_stackv_ctorl(struct cdc_stackv **s,
                               struct cdc_data_info *info, ...)
{
        assert(s != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = cdc_stackv_ctorv(s, info, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_stackv_ctorv(struct cdc_stackv **s,
                               struct cdc_data_info *info, va_list args)
{
        assert(s != NULL);

        struct cdc_stackv *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_stackv *)malloc(sizeof(struct cdc_stackv));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        *s = tmp;
        ret = cdc_vector_ctorv(&(*s)->vector, info, args);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        return ret;
}

void cdc_stackv_dtor(struct cdc_stackv *s)
{
        assert(s != NULL);

        cdc_vector_dtor(s->vector);
        free(s);
}

enum cdc_stat cdc_stackv_push(struct cdc_stackv *s, void *elem)
{
        assert(s != NULL);

        return cdc_vector_push_back(s->vector, elem);
}

enum cdc_stat cdc_stackv_pop(struct cdc_stackv *s)
{
        assert(s != NULL);
        assert(cdc_vector_size(s->vector) > 0);

        return cdc_vector_pop_back(s->vector);
}

void cdc_stackv_swap(struct cdc_stackv *a, struct cdc_stackv *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_vector *, a->vector, b->vector);
}
