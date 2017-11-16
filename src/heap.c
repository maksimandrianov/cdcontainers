#include "cdcontainers/heap.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "cdcontainers/vector.h"

struct cdc_heap {
        cdc_vector_t *vector;
        cdc_compar_func_t compar;
};

static inline size_t cdc_heap_parent(size_t i)
{
        return i / 2;
}

static inline size_t cdc_heap_left(size_t i)
{
        return 2 * i;
}

static inline size_t cdc_heap_right(size_t i)
{
        return 2 * i + 1;
}

static inline void cdc_heap_heapify(cdc_heap_t *h, size_t i)
{
        assert(h != NULL);

        size_t l, r, largest;
        size_t size = cdc_vector_size(h->vector);
        void **data = cdc_vector_data(h->vector);
        bool cond;

        do {
                l = cdc_heap_left(i);
                r = cdc_heap_right(i);

                if (l <= size && (*h->compar)(data[l], data[i]))
                        largest = l;
                else
                        largest = i;

                if (r <= size && (*h->compar)(data[r], data[largest]))
                        largest = r;

                if (cond = (largest != i)) {
                        CDC_SWAP(void *, data[i], data[largest]);
                        i = largest;
                }

        } while (cond);
}

static inline void cdc_heap_build(cdc_heap_t *h)
{
        assert(h != NULL);

        size_t i;

        for (i = cdc_vector_size(h->vector) / 2; i > 0; --i)
                cdc_heap_heapify(h, i);
}

static inline enum cdc_stat cdc_heap_init_varg(cdc_heap_t *h, va_list args)
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

enum cdc_stat cdc_heap_ctor(cdc_heap_t **h, cdc_free_func_t func,
                            cdc_compar_func_t compar)
{
        assert(h != NULL);
        assert(compar != NULL);

        cdc_heap_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_heap_t *)malloc(sizeof(cdc_heap_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->compar = compar;
        ret = cdc_vector_ctor(&tmp->vector, func);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *h = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_heap_ctorl(cdc_heap_t **h, cdc_free_func_t func,
                             cdc_compar_func_t compar, ...)
{
        assert(h != NULL);
        assert(compar != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, compar);
        ret = cdc_heap_ctorv(h, func, compar, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_heap_ctorv(cdc_heap_t **h, cdc_free_func_t func,
                             cdc_compar_func_t compar, va_list args)
{
        assert(h != NULL);
        assert(compar != NULL);


        enum cdc_stat ret;

        ret = cdc_heap_ctor(h, func, compar);
        if (ret != CDC_STATUS_OK)
                return ret;

        return cdc_heap_init_varg(*h, args);
}

void cdc_heap_dtor(cdc_heap_t *h)
{
        assert(h != NULL);

        cdc_vector_dtor(h->vector);
        free(h);
}

void *cdc_heap_top(cdc_heap_t *h)
{
        assert(h != NULL);
        assert(cdc_heap_size(h) > 0);

        return cdc_vector_front(h->vector);
}

enum cdc_stat cdc_heap_extract_top(cdc_heap_t *h)
{
        assert(h != NULL);
        assert(cdc_heap_size(h) > 0);

        enum cdc_stat ret;
        void *elem = cdc_vector_back(h->vector);

        ret = cdc_vector_pop_back(h->vector);
        if (ret != CDC_STATUS_OK)
                return ret;

        cdc_vector_set(h->vector, 0, elem);
        cdc_heap_heapify(h, 0);

        return ret;
}

void cdc_heap_increase_key(cdc_heap_t *h, size_t i, void *key)
{
        assert(h != NULL);
        assert(i < cdc_heap_size(h));
        assert(cdc_heap_size(h) > 0);

        size_t parent;
        void **data = cdc_vector_data(h->vector);

        if ((*h->compar)(data[i], key))
                return;

        cdc_vector_set(h->vector, i, key);
        parent = cdc_heap_parent(i);
        while (i > 0 && (*h->compar)(data[i], data[parent])) {
                CDC_SWAP(void *, data[i], data[parent]);
                i = cdc_heap_parent(i);
        }
}

enum cdc_stat cdc_heap_insert(cdc_heap_t *h, void *key)
{
        assert(h != NULL);

        enum cdc_stat ret;
        size_t i, parent;
        void **data = cdc_vector_data(h->vector);

        ret = cdc_vector_push_back(h->vector, key);
        if (ret != CDC_STATUS_OK)
                return ret;

        i = cdc_vector_size(h->vector);
        parent = cdc_heap_parent(i);
        while (i > 0 && (*h->compar)(data[i], data[parent])) {
                CDC_SWAP(void *, data[i], data[parent]);
                i = cdc_heap_parent(i);
        }

        return ret;
}

size_t cdc_heap_size(cdc_heap_t *h)
{
        assert(h != NULL);

        return cdc_vector_size(h->vector);
}

bool cdc_heap_empty(cdc_heap_t *h)
{
        assert(h != NULL);

        return cdc_vector_size(h->vector) == 0;
}

void cdc_heap_swap (cdc_heap_t *a, cdc_heap_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(cdc_vector_t *,    a->vector, b->vector);
        CDC_SWAP(cdc_compar_func_t, a->compar, b->compar);
}
