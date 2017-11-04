#include "vector.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include "common.h"

struct cds_vector {
        size_t size;
        size_t capacity;
        float  cop_exp;
        void   **buffer;
        void   (*fp_free)(void *);
};

static bool cds_vector_should_shrink(cds_vector_t *v)
{
        assert(v != NULL);
        assert(v->size <= v->capacity);

        return v->size <= v->capacity * CDS_VECTOR_SHRINK_THRESHOLD;
}

static bool cds_vector_reach_limit_size(cds_vector_t *v)
{
        assert(v != NULL);
        assert(v != NULL);

        return v->size == CDS_VECTOR_MAX_LEN;
}

static bool cds_vector_should_grow(cds_vector_t *v)
{
        assert(v != NULL);
        assert(v->size <= v->capacity);

        return v->size == v->capacity;
}

static enum cds_stat cds_vector_reallocate(cds_vector_t *v, size_t capacity)
{
        assert(v != NULL);

        void *tmp;

        if (capacity < CDS_VECTOR_MIN_CAPACITY) {
                if (v->capacity > CDS_VECTOR_MIN_CAPACITY)
                        capacity = CDS_VECTOR_MIN_CAPACITY;
                else
                        return CDS_STATUS_OK;
        }

        if (!(tmp = malloc(capacity * sizeof (void *))))
                return CDS_STATUS_BAD_ALLOC;

        memcpy(tmp, v->buffer, CDS_MIN(v->size, capacity) * sizeof(void *));
        free(v->buffer);

        v->capacity = capacity;
        v->buffer   = tmp;

        return CDS_STATUS_OK;
}

static enum cds_stat cds_vector_grow(cds_vector_t *v)
{
        assert(v != NULL);

        return cds_vector_reallocate(v, v->capacity * v->cop_exp);
}

static enum cds_stat cds_vector_shrink(cds_vector_t *v)
{
        assert(v != NULL);

        return cds_vector_reallocate(v, v->capacity / v->cop_exp);
}

static void cds_vector_move_left(cds_vector_t *v, size_t index)
{
        assert(v != NULL);

        size_t count_bytes;

        count_bytes = (v->size - index) * sizeof(void *);
        memmove(v->buffer + index, v->buffer + index + 1, count_bytes);
}

static void cds_vector_move_right(cds_vector_t *v, size_t index)
{
        assert(v != NULL);

        size_t count_bytes;

        count_bytes = (v->size - index) * sizeof(void *);
        memmove(v->buffer + index + 1, v->buffer + index, count_bytes);
}

static void cds_vector_free_range(cds_vector_t *v, size_t start, size_t end)
{
        assert(v);
        assert(start < v->size);
        assert(end <= v->size);

        size_t i;

        for (i = start; i < end; ++i)
                (*v->fp_free)(v->buffer[i]);
}

enum cds_stat cds_vector_ctor(cds_vector_t **v, void (*fp_free)(void *))
{
        assert(v != NULL);

        cds_vector_t *tmp;
        enum cds_stat ret;

        if (!(tmp = (cds_vector_t *)malloc(sizeof( cds_vector_t))))
                return CDS_STATUS_BAD_ALLOC;

        tmp->size     = 0;
        tmp->capacity = 0;
        tmp->cop_exp  = CDS_VECTOR_COPACITY_EXP;
        tmp->buffer   = NULL;
        tmp->fp_free  = fp_free;

        if ((ret = cds_vector_reserve(tmp, CDS_VECTOR_MIN_CAPACITY)) !=
            CDS_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *v = tmp;

        return CDS_STATUS_OK;
}

enum cds_stat cds_vector_ctor_l(cds_vector_t **v, void (*fp_free)(void *), ...)
{
        assert(v != NULL);

        enum cds_stat ret;
        va_list args;
        void *elem;

        if ((ret = cds_vector_ctor(v, fp_free)) != CDS_STATUS_OK)
                return ret;

        va_start(args, fp_free);
        while ((elem = va_arg(args, void *)) != NULL) {
                if ((ret = cds_vector_push_back(*v, elem)) != CDS_STATUS_OK) {
                        va_end(args);
                        return ret;
                }
        }

        va_end(args);

        return CDS_STATUS_OK;
}

void cds_vector_dtor(cds_vector_t *v)
{
        assert(v != NULL);

        if (v->fp_free)
                cds_vector_free_range(v, 0, v->size);

        free(v->buffer);
        free(v);
}

void **cds_vector_data(cds_vector_t *v)
{
        assert(v != NULL);

        return v->buffer;
}

enum cds_stat cds_vector_insert(cds_vector_t *v, size_t index, void *elem)
{
        assert(v != NULL);
        assert(elem != NULL);
        assert(index <= v->size);

        if (cds_vector_should_grow(v)) {
                enum cds_stat ret = cds_vector_grow(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        if (v->size == index) {
                cds_vector_push_back(v,elem);
                return CDS_STATUS_OK;
        }

        cds_vector_move_right(v, index);

        v->buffer[index] = elem;
        ++v->size;

        return CDS_STATUS_OK;
}

void cds_vector_clear(cds_vector_t *v)
{
        assert(v != NULL);

        if (v->fp_free)
                cds_vector_free_range(v, 0, v->size);

        v->size = 0;
}

enum cds_stat cds_vector_erase(cds_vector_t *v, size_t index, void **elem)
{
        assert(v != NULL);
        assert(elem != NULL);
        assert(index < v->size);

        *elem = v->buffer[index];

        if (v->size - 1 == index)
                return cds_vector_pop_back(v);

        cds_vector_move_left(v, index);
        --v->size;

        if (cds_vector_should_shrink(v)) {
                enum cds_stat ret = cds_vector_shrink(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        return CDS_STATUS_OK;
}

enum cds_stat cds_vector_reserve(cds_vector_t *v, size_t capacity)
{
        assert(v != NULL);

        if (capacity > v->capacity)
                return cds_vector_reallocate(v, capacity);

        return CDS_STATUS_OK;
}

enum cds_stat cds_vector_push_back(cds_vector_t *v, void *elem)
{
        assert(v != NULL);
        assert(elem != NULL);

        if (cds_vector_should_grow(v)) {
                enum cds_stat ret = cds_vector_grow(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        v->buffer[v->size++] = elem;

        return CDS_STATUS_OK;
}

enum cds_stat cds_vector_pop_back(cds_vector_t *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        --v->size;

        if (cds_vector_should_shrink(v)) {
                enum cds_stat ret = cds_vector_shrink(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        return CDS_STATUS_OK;
}

void cds_vector_swap(cds_vector_t *a, cds_vector_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDS_SWAP(size_t,  a->size,     b->size);
        CDS_SWAP(size_t,  a->capacity, b->capacity);
        CDS_SWAP(float,   a->cop_exp,  b->cop_exp);
        CDS_SWAP(void **, a->buffer,   b->buffer);
}

void *cds_vector_get(cds_vector_t *v, size_t index)
{
        assert(v != NULL);
        assert(index < v->size);

        return v->buffer[index];
}

enum cds_stat cds_vector_at(cds_vector_t *v, size_t index, void **elem)
{
        assert(v != NULL);
        assert(elem != NULL);

        if (index > v->size)
                return CDS_STATUS_OUT_OF_RANGE;

        *elem = v->buffer[index];

        return CDS_STATUS_OK;
}


void *cds_vector_front(cds_vector_t *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        return cds_vector_get(v, 0);
}

void *cds_vector_back(cds_vector_t *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        return cds_vector_get(v, v->size - 1);
}

bool cds_vector_empty(cds_vector_t *v)
{
        return v->size == 0;
}

size_t cds_vector_size(cds_vector_t *v)
{
        assert(v != NULL);

        return v->size;
}

size_t cds_vector_capacity(cds_vector_t *v)
{
        assert(v != NULL);

        return v->capacity;
}

float cds_vector_cap_exp(cds_vector_t *v)
{
        assert(v != NULL);

        return v->cop_exp;
}
