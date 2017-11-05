#include "vector.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include "common.h"

struct cdc_vector {
        size_t size;
        size_t capacity;
        float cop_exp;
        void **buffer;
        void (*fp_free)(void *);
};

static inline bool cdc_vector_should_shrink(cdc_vector_t *v)
{
        assert(v != NULL);
        assert(v->size <= v->capacity);

        return v->size <= v->capacity * CDC_VECTOR_SHRINK_THRESHOLD;
}

static inline bool cdc_vector_reach_limit_size(cdc_vector_t *v)
{
        assert(v != NULL);
        assert(v != NULL);

        return v->size == CDC_VECTOR_MAX_LEN;
}

static inline bool cdc_vector_should_grow(cdc_vector_t *v)
{
        assert(v != NULL);
        assert(v->size <= v->capacity);

        return v->size == v->capacity;
}

static inline enum cdc_stat cdc_vector_reallocate(cdc_vector_t *v,
                                                  size_t capacity)
{
        assert(v != NULL);

        void *tmp;

        if (capacity < CDC_VECTOR_MIN_CAPACITY) {
                if (v->capacity > CDC_VECTOR_MIN_CAPACITY)
                        capacity = CDC_VECTOR_MIN_CAPACITY;
                else
                        return cdc_STATUS_OK;
        }

        tmp = malloc(capacity * sizeof (void *));
        if (!tmp)
                return cdc_STATUS_BAD_ALLOC;

        memcpy(tmp, v->buffer, CDC_MIN(v->size, capacity) * sizeof(void *));
        free(v->buffer);

        v->capacity = capacity;
        v->buffer   = tmp;

        return cdc_STATUS_OK;
}

static inline enum cdc_stat cdc_vector_grow(cdc_vector_t *v)
{
        assert(v != NULL);

        return cdc_vector_reallocate(v, v->capacity * v->cop_exp);
}

static inline enum cdc_stat cdc_vector_shrink(cdc_vector_t *v)
{
        assert(v != NULL);

        return cdc_vector_reallocate(v, v->capacity / v->cop_exp);
}

static inline void cdc_vector_move_left(cdc_vector_t *v, size_t index)
{
        assert(v != NULL);

        size_t count_bytes = (v->size - index) * sizeof(void *);

        memmove(v->buffer + index, v->buffer + index + 1, count_bytes);
}

static inline void cdc_vector_move_right(cdc_vector_t *v, size_t index)
{
        assert(v != NULL);

        size_t count_bytes = (v->size - index) * sizeof(void *);

        memmove(v->buffer + index + 1, v->buffer + index, count_bytes);
}

static inline void cdc_vector_free_range(cdc_vector_t *v, size_t start,
                                         size_t end)
{
        assert(v);
        assert(start < v->size);
        assert(end <= v->size);

        size_t i;

        for (i = start; i < end; ++i)
                (*v->fp_free)(v->buffer[i]);
}

enum cdc_stat cdc_vector_ctor(cdc_vector_t **v, void (*fp_free)(void *))
{
        assert(v != NULL);

        cdc_vector_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_vector_t *)malloc(sizeof(cdc_vector_t));
        if (!tmp)
                return cdc_STATUS_BAD_ALLOC;

        tmp->size     = 0;
        tmp->capacity = 0;
        tmp->cop_exp  = CDC_VECTOR_COPACITY_EXP;
        tmp->buffer   = NULL;
        tmp->fp_free  = fp_free;

        ret = cdc_vector_reserve(tmp, CDC_VECTOR_MIN_CAPACITY);
        if (ret != cdc_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *v = tmp;

        return cdc_STATUS_OK;
}

enum cdc_stat cdc_vector_ctor_l(cdc_vector_t **v, void (*fp_free)(void *), ...)
{
        assert(v != NULL);

        enum cdc_stat ret;
        va_list args;
        void *elem;

        ret = cdc_vector_ctor(v, fp_free);
        if (ret != cdc_STATUS_OK)
                return ret;

        va_start(args, fp_free);
        while ((elem = va_arg(args, void *)) != NULL) {
                ret = cdc_vector_push_back(*v, elem);
                if (ret != cdc_STATUS_OK) {
                        va_end(args);
                        return ret;
                }
        }

        va_end(args);

        return cdc_STATUS_OK;
}

void cdc_vector_dtor(cdc_vector_t *v)
{
        assert(v != NULL);

        if (v->fp_free)
                cdc_vector_free_range(v, 0, v->size);

        free(v->buffer);
        free(v);
}

void **cdc_vector_data(cdc_vector_t *v)
{
        assert(v != NULL);

        return v->buffer;
}

enum cdc_stat cdc_vector_insert(cdc_vector_t *v, size_t index, void *elem)
{
        assert(v != NULL);
        assert(elem != NULL);
        assert(index <= v->size);

        if (cdc_vector_should_grow(v)) {
                enum cdc_stat ret = cdc_vector_grow(v);
                if (ret != cdc_STATUS_OK)
                        return ret;
        }

        if (v->size == index) {
                cdc_vector_push_back(v,elem);
                return cdc_STATUS_OK;
        }

        cdc_vector_move_right(v, index);

        v->buffer[index] = elem;
        ++v->size;

        return cdc_STATUS_OK;
}

void cdc_vector_clear(cdc_vector_t *v)
{
        assert(v != NULL);

        if (v->fp_free)
                cdc_vector_free_range(v, 0, v->size);

        v->size = 0;
}

enum cdc_stat cdc_vector_erase(cdc_vector_t *v, size_t index, void **elem)
{
        assert(v != NULL);
        assert(elem != NULL);
        assert(index < v->size);

        *elem = v->buffer[index];

        if (v->size - 1 == index)
                return cdc_vector_pop_back(v);

        cdc_vector_move_left(v, index);
        --v->size;

        if (cdc_vector_should_shrink(v)) {
                enum cdc_stat ret = cdc_vector_shrink(v);
                if (ret != cdc_STATUS_OK)
                        return ret;
        }

        return cdc_STATUS_OK;
}

enum cdc_stat cdc_vector_reserve(cdc_vector_t *v, size_t capacity)
{
        assert(v != NULL);

        if (capacity > v->capacity)
                return cdc_vector_reallocate(v, capacity);

        return cdc_STATUS_OK;
}

enum cdc_stat cdc_vector_push_back(cdc_vector_t *v, void *elem)
{
        assert(v != NULL);

        if (cdc_vector_should_grow(v)) {
                enum cdc_stat ret = cdc_vector_grow(v);
                if (ret != cdc_STATUS_OK)
                        return ret;
        }

        v->buffer[v->size++] = elem;

        return cdc_STATUS_OK;
}

enum cdc_stat cdc_vector_pop_back(cdc_vector_t *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        --v->size;

        if (cdc_vector_should_shrink(v)) {
                enum cdc_stat ret = cdc_vector_shrink(v);
                if (ret != cdc_STATUS_OK)
                        return ret;
        }

        return cdc_STATUS_OK;
}

void cdc_vector_swap(cdc_vector_t *a, cdc_vector_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(size_t,  a->size,     b->size);
        CDC_SWAP(size_t,  a->capacity, b->capacity);
        CDC_SWAP(float,   a->cop_exp,  b->cop_exp);
        CDC_SWAP(void **, a->buffer,   b->buffer);
        CDC_SWAP(void *,  a->fp_free,  b->fp_free);
}

void *cdc_vector_get(cdc_vector_t *v, size_t index)
{
        assert(v != NULL);
        assert(index < v->size);

        return v->buffer[index];
}

enum cdc_stat cdc_vector_at(cdc_vector_t *v, size_t index, void **elem)
{
        assert(v != NULL);
        assert(elem != NULL);

        if (index > v->size)
                return cdc_STATUS_OUT_OF_RANGE;

        *elem = v->buffer[index];

        return cdc_STATUS_OK;
}


void *cdc_vector_front(cdc_vector_t *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        return cdc_vector_get(v, 0);
}

void *cdc_vector_back(cdc_vector_t *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        return cdc_vector_get(v, v->size - 1);
}

bool cdc_vector_empty(cdc_vector_t *v)
{
        return v->size == 0;
}

size_t cdc_vector_size(cdc_vector_t *v)
{
        assert(v != NULL);

        return v->size;
}

size_t cdc_vector_capacity(cdc_vector_t *v)
{
        assert(v != NULL);

        return v->capacity;
}

float cdc_vector_cap_exp(cdc_vector_t *v)
{
        assert(v != NULL);

        return v->cop_exp;
}
