#include "cdcontainers/heap.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "data-info.h"
#include "cdcontainers/vector.h"

static inline size_t parent(size_t i)
{
        return i / 2;
}

static inline size_t left(size_t i)
{
        return 2 * i;
}

static inline size_t right(size_t i)
{
        return 2 * i + 1;
}

static inline void heapify(struct cdc_heap *h, size_t i)
{
        assert(h != NULL);

        size_t l, r, largest;
        size_t size = cdc_vector_size(h->vector);
        void **data = cdc_vector_data(h->vector);
        bool cond;

        do {
                l = left(i);
                r = right(i);

                if (l <= size && h->compar(data[l], data[i]))
                        largest = l;
                else
                        largest = i;

                if (r <= size && h->compar(data[r], data[largest]))
                        largest = r;

                cond = largest != i;
                if (cond) {
                        CDC_SWAP(void *, data[i], data[largest]);
                        i = largest;
                }

        } while (cond);
}

static inline void build_heap(struct cdc_heap *h)
{
        assert(h != NULL);

        size_t i;

        for (i = cdc_vector_size(h->vector) / 2; i > 0; --i)
                heapify(h, i);
}

static inline enum cdc_stat init_varg(struct cdc_heap *h, va_list args)
{
        assert(h != NULL);

        enum cdc_stat ret;
        void *elem;

        while ((elem = va_arg(args, void *)) != NULL) {
                ret = cdc_heap_insert(h, elem);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_heap_ctor(struct cdc_heap **h, struct cdc_data_info *info,
                            cdc_compar_func_t compar)
{
        assert(h != NULL);
        assert(compar != NULL);

        struct cdc_heap *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_heap *)calloc(sizeof(struct cdc_heap), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->compar = compar;
        if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
                ret = CDC_STATUS_BAD_ALLOC;
                goto error1;
        }

        ret = cdc_vector_ctor(&tmp->vector, tmp->dinfo);
        if (ret != CDC_STATUS_OK)
                goto error2;

        *h = tmp;
        return CDC_STATUS_OK;
error2:
        cdc_di_shared_dtor(tmp->dinfo);
error1:
        free(tmp);
        return ret;
}

enum cdc_stat cdc_heap_ctorl(struct cdc_heap **h, struct cdc_data_info *info,
                             cdc_compar_func_t compar, ...)
{
        assert(h != NULL);
        assert(compar != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, compar);
        ret = cdc_heap_ctorv(h, info, compar, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_heap_ctorv(struct cdc_heap **h, struct cdc_data_info *info,
                             cdc_compar_func_t compar, va_list args)
{
        assert(h != NULL);
        assert(compar != NULL);

        enum cdc_stat ret;

        ret = cdc_heap_ctor(h, info, compar);
        if (ret != CDC_STATUS_OK)
                return ret;

        return init_varg(*h, args);
}

void cdc_heap_dtor(struct cdc_heap *h)
{
        assert(h != NULL);

        cdc_vector_dtor(h->vector);
        cdc_di_shared_dtor(h->dinfo);
        free(h);
}

void *cdc_heap_top(struct cdc_heap *h)
{
        assert(h != NULL);
        assert(cdc_heap_size(h) > 0);

        return cdc_vector_front(h->vector);
}

enum cdc_stat cdc_heap_extract_top(struct cdc_heap *h)
{
        assert(h != NULL);
        assert(cdc_heap_size(h) > 0);

        enum cdc_stat ret;
        void *elem = cdc_vector_back(h->vector);

        ret = cdc_vector_pop_back(h->vector);
        if (ret != CDC_STATUS_OK)
                return ret;

        if (cdc_vector_empty(h->vector))
                return ret;

        cdc_vector_set(h->vector, 0, elem);
        heapify(h, 0);

        return ret;
}

void cdc_heap_increase_key(struct cdc_heap *h, size_t i, void *key)
{
        assert(h != NULL);
        assert(i < cdc_heap_size(h));
        assert(cdc_heap_size(h) > 0);

        size_t p;
        void **data = cdc_vector_data(h->vector);

        if (h->compar(data[i], key))
                return;

        cdc_vector_set(h->vector, i, key);
        p = parent(i);
        while (i > 0 && h->compar(data[i], data[p])) {
                CDC_SWAP(void *, data[i], data[p]);
                i = parent(i);
        }
}

enum cdc_stat cdc_heap_insert(struct cdc_heap *h, void *key)
{
        assert(h != NULL);

        enum cdc_stat ret;
        size_t i, p;
        void **data = cdc_vector_data(h->vector);

        ret = cdc_vector_push_back(h->vector, key);
        if (ret != CDC_STATUS_OK)
                return ret;

        i = cdc_vector_size(h->vector) - 1;
        p = parent(i);
        while (i > 0 && h->compar(data[i], data[p])) {
                CDC_SWAP(void *, data[i], data[p]);
                i = parent(i);
        }

        return ret;
}

size_t cdc_heap_size(struct cdc_heap *h)
{
        assert(h != NULL);

        return cdc_vector_size(h->vector);
}

bool cdc_heap_empty(struct cdc_heap *h)
{
        assert(h != NULL);

        return cdc_vector_size(h->vector) == 0;
}

void cdc_heap_swap (struct cdc_heap *a, struct cdc_heap *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_vector *, a->vector, b->vector);
        CDC_SWAP(cdc_compar_func_t, a->compar, b->compar);
        CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}
