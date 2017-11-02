#include "vector.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include "common.h"

#define CDS_VEC_MIN_CAPACITY     (4)
#define CDS_VEC_COPACITY_EXP     (2.0f)
#define CDS_VEC_SHRINK_THRESHOLD (1.0 / 4.0)
#define CDS_VEC_MAX_LEN          (SIZE_MAX)

struct cds_vector {
        size_t size;
        size_t capacity;
        float  cop_exp;
        void   **buffer;
};

static bool cds_vec_should_shrink(const struct cds_vector *v)
{
        assert(v != NULL);
        assert(v->size <= v->capacity);

        return v->size <= v->capacity * CDS_VEC_SHRINK_THRESHOLD;
}

static bool cds_vec_reach_limit_size(const struct cds_vector *v)
{
        assert(v != NULL);
        assert(v != NULL);

        return v->size == CDS_VEC_MAX_LEN;
}

static bool cds_vec_should_grow(const struct cds_vector *v)
{
        assert(v->size <= v->capacity);

        return v->size == v->capacity;
}

static enum cds_stat cds_vec_reallocate(struct cds_vector *v, size_t size)
{
        assert(v != NULL);

        void *tmp;

        if (size < CDS_VEC_MIN_CAPACITY) {
                if (v->capacity > CDS_VEC_MIN_CAPACITY)
                        size = CDS_VEC_MIN_CAPACITY;
                else
                        return CDS_STATUS_OK;
        }

        if (!(tmp = malloc(size * sizeof (void *))))
                return CDS_STATUS_BAD_ALLOC;

        memcpy(tmp, v->buffer, CDS_MIN(v->size, size) * sizeof(void *));
        free(v->buffer);

        v->capacity = size;
        v->buffer   = tmp;

        return CDS_STATUS_OK;
}

static enum cds_stat cds_vec_grow(struct cds_vector *v)
{
        return cds_vec_reallocate(v, v->capacity * v->cop_exp);
}

static enum cds_stat cds_vec_shrink(struct cds_vector *v)
{
        return cds_vec_reallocate(v, v->capacity / v->cop_exp);
}

static void cds_vec_move_left(struct cds_vector *v, size_t index)
{
        size_t count_bytes;

        count_bytes = (v->size - index) * sizeof(void *);
        memmove(v->buffer + index - 1, v->buffer + index, count_bytes);
}

static void cds_vec_move_right(struct cds_vector *v, size_t index)
{
        size_t count_bytes;

        count_bytes = (v->size - index) * sizeof(void *);
        memmove(v->buffer + index + 1, v->buffer + index, count_bytes);
}

enum cds_stat cds_vec_ctor(struct cds_vector **v)
{
        assert(v != NULL);

        struct cds_vector *tmp;
        enum cds_stat ret;

        if (!(tmp = (struct cds_vector *)malloc(sizeof(struct cds_vector))))
                return CDS_STATUS_BAD_ALLOC;

        tmp->size     = 0;
        tmp->capacity = 0;
        tmp->cop_exp  = CDS_VEC_COPACITY_EXP;
        tmp->buffer   = NULL;

        if ((ret = cds_vec_reserve(tmp, CDS_VEC_MIN_CAPACITY)) !=
                        CDS_STATUS_OK)
                return ret;

        *v = tmp;

        return CDS_STATUS_OK;
}

enum cds_stat cds_vec_ctor_list(struct cds_vector **v, ...)
{
        assert(v != NULL);

        enum cds_stat ret;
        va_list args;
        void *elem;

        if ((ret = cds_vec_ctor(v)) != CDS_STATUS_OK)
                return ret;

        va_start(args, v);
        while ((elem = va_arg(args, void *)) != NULL) {
                if ((ret = cds_vec_push_back(*v, elem)) != CDS_STATUS_OK) {
                        va_end(args);
                        return ret;
                }
        }

        va_end(args);

        return CDS_STATUS_OK;
}

void cds_vec_dtor(struct cds_vector *v)
{
        assert(v != NULL);

        free(v->buffer);
        free(v);
}

void cds_vec_dtor_f(struct cds_vector *v, void (*f)(void *))
{
        assert(v != NULL);
        assert(f != NULL);

        size_t i;

        for (i = 0; i < v->size; ++i)
                (*f)(v->buffer[i]);

        free(v->buffer);
        free(v);
}

void * cds_vec_data(struct cds_vector *v)
{
        return v->buffer;
}

enum cds_stat cds_vec_insert(struct cds_vector *v, size_t index, void *elem)
{
        assert(v != NULL);
        assert(elem != NULL);
        assert(index < v->size);

        if (cds_vec_should_grow(v)) {
                enum cds_stat ret = cds_vec_grow(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        cds_vec_move_right(v, index);
        v->buffer[index] = elem;
        ++v->size;

        return CDS_STATUS_OK;
}

void cds_vec_clear(struct cds_vector *v)
{
        assert(v != NULL);

        v->size = 0;
}

void cds_vec_clear_f(struct cds_vector *v, void (*f)(void *))
{
        assert(v != NULL);
        assert(f != NULL);

        size_t i;

        for (i = 0; i < v->size; ++i)
                (*f)(v->buffer[i]);

        v->size = 0;
}

enum cds_stat cds_vec_erase(struct cds_vector *v, size_t index, void **elem)
{
        assert(v != NULL);
        assert(index < v->size);
        assert(elem != NULL);

        void *tmp = v->buffer[index];

        *elem = tmp;
        cds_vec_move_left(v, index);
        --v->size;

        if (cds_vec_should_shrink(v)) {
                enum cds_stat ret = cds_vec_shrink(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        return CDS_STATUS_OK;
}

enum cds_stat cds_vec_reserve(struct cds_vector *v, size_t size)
{
        assert(v != NULL);

        if (size > v->capacity)
                return cds_vec_reallocate(v, size);

        return CDS_STATUS_OK;
}

enum cds_stat cds_vec_push_back(struct cds_vector *v, void *elem)
{
        assert(v != NULL);
        assert(elem != NULL);

        if (cds_vec_should_grow(v)) {
                enum cds_stat ret = cds_vec_grow(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        v->buffer[v->size++] = (void *)elem;

        return CDS_STATUS_OK;
}

enum cds_stat cds_vec_pop_back(struct cds_vector *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        --v->size;

        if (cds_vec_should_shrink(v)) {
                enum cds_stat ret = cds_vec_shrink(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        return CDS_STATUS_OK;
}

void cds_vec_swap(struct cds_vector *a, struct cds_vector *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDS_SWAP(size_t,  a->size,     b->size);
        CDS_SWAP(size_t,  a->capacity, b->capacity);
        CDS_SWAP(float,   a->cop_exp,  b->cop_exp);
        CDS_SWAP(void **, a->buffer,   b->buffer);
}

void * cds_vec_get(struct cds_vector *v, size_t index)
{
        assert(v != NULL);
        assert(index < v->size);

        return v->buffer[index];
}

const void * cds_vec_const_get(struct cds_vector *v, size_t index)
{
        assert(v != NULL);
        assert(index < v->size);

        return cds_vec_get(v, index);
}

enum cds_stat cds_vec_at(struct cds_vector *v, size_t index, void *elem)
{
        assert(v != NULL);
        assert(index < v->size);

        if (index > v->size)
                return CDS_STATUS_OUT_OF_RANGE;

        elem = v->buffer[index];

        return CDS_STATUS_OK;
}


void * cds_vec_front(struct cds_vector *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        return cds_vec_get(v, 0);
}

void * cds_vec_back(struct cds_vector *v)
{
        assert(v != NULL);
        assert(v->size > 0);

        return cds_vec_get(v, v->size - 1);
}

bool cds_vec_empty(struct cds_vector *v)
{
        return v->size == 0;
}

size_t cds_vec_size(struct cds_vector *v)
{
        assert(v != NULL);

        return v->size;
}

size_t cds_vec_capacity(struct cds_vector *v)
{
        assert(v != NULL);

        return v->capacity;
}

