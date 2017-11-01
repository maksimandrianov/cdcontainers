#include "vector.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define CDS_VEC_MIN_CAPACITY     (4)
#define CDS_VEC_COPACITY_EXP     (2.0f)
#define CDS_VEC_SHRINK_THRESHOLD (1.0 / 4.0)

#define CDS_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CDS_MIN(a, b) ((a) < (b) ? (a) : (b))

struct cds_vector {
        size_t size;
        size_t capacity;
        void   **buffer;
        float  cop_exp;
};

static bool cds_vec_should_shrink(const struct cds_vector *v)
{
        assert(v->size <= v->capacity);

        return v->size == v->capacity * CDS_VEC_SHRINK_THRESHOLD;
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

        memcpy(tmp, v->buffer, size * sizeof(void *));
        free(v->buffer);

        v->capacity = size;
        v->buffer   = tmp;

        return CDS_STATUS_OK;
}

static enum cds_stat cds_vec_adjust_capacity(struct cds_vector *v)
{
        return cds_vec_reallocate(v, CDS_MAX(1, v->capacity * v->cop_exp));
}

enum cds_stat cds_vec_ctor(struct cds_vector *v)
{
        assert(v != NULL);

        struct cds_vector *tmp;

        if (!(tmp = (struct cds_vector *)calloc(sizeof(struct cds_vector), 0)))
                return CDS_STATUS_BAD_ALLOC;

        tmp->cop_exp = CDS_VEC_COPACITY_EXP;
        cds_vec_reserve(tmp, CDS_VEC_MIN_CAPACITY);
        v = tmp;

        return CDS_STATUS_OK;
}

void cds_vec_dtor(struct cds_vector *v)
{
        assert(v != NULL);

        free(v->buffer);
        free(v);
}

enum cds_stat cds_vec_reserve(struct cds_vector *v, size_t size)
{
        assert(v != NULL);

        if (size > v->capacity)
                return cds_vec_reallocate(v, size);

        return CDS_STATUS_OK;
}

enum cds_stat cds_vec_push_back(struct cds_vector *v, const void *elem)
{
        assert(v != NULL);
        assert(elem != NULL);

        if (cds_vec_should_grow(v)) {
                enum cds_stat ret = cds_vec_adjust_capacity(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        v->buffer[v->size++] = (void *)elem;

        return CDS_STATUS_OK;
}

enum cds_stat cds_vec_pop_back(struct cds_vector *v)
{
        assert(v != NULL);

        --v->size;

        if (cds_vec_should_shrink(v)) {
                enum cds_stat ret = cds_vec_adjust_capacity(v);
                if (ret != CDS_STATUS_OK)
                        return ret;
        }

        return CDS_STATUS_OK;
}

void *cds_vec_get(struct cds_vector *v, size_t index)
{
        assert(v != NULL);
        assert(index >= 0 && index < v->size);

        return v->buffer[index];
}

const void * cds_vec_const_get(struct cds_vector *v, size_t index)
{
        assert(v != NULL);
        assert(index >= 0 && index < v->size);

        return cds_vec_get(v, index);
}

enum cds_stat cds_vec_at(struct cds_vector *v, size_t index, void *elem)
{
        assert(v != NULL);
        assert(index >= 0 && index < v->size);

        if (index < 0 && index > v->size)
                return CDS_STATUS_OUT_OF_RANGE;

        elem = v->buffer[index];

        return CDS_STATUS_OK;
}


void * cds_vec_front(struct cds_vector *v)
{
        assert(v != NULL);

        return cds_vec_get(v, 0);
}

void * cds_vec_back(struct cds_vector *v)
{
        assert(v != NULL);

        return cds_vec_get(v, v->size - 1);
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

