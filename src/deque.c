#include "cdcontainers/deque.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stddef.h>

struct cdc_deque {
        void **buffer;
        size_t head;
        size_t tail;
        size_t size;
        size_t capacity;
        cdc_free_func_t fp_free;
};

static inline bool cdc_deque_should_shrink(cdc_deque_t *d)
{
        assert(d != NULL);
        assert(d->size <= d->capacity);

        return d->size <= d->capacity * CDC_DEQUE_SHRINK_THRESHOLD;
}

static inline bool cdc_deque_should_grow(cdc_deque_t *d)
{
        assert(d != NULL);
        assert(d->size <= d->capacity);

        return d->size == d->capacity;
}

static inline bool cdc_deque_reach_limit_size(cdc_deque_t *d)
{
        assert(d != NULL);
        assert(d != NULL);

        return d->size == CDC_DEQUE_MAX_LEN;
}

static inline enum cdc_stat cdc_deque_reallocate(cdc_deque_t *d,
                                                 size_t capacity)
{
        assert(d != NULL);

        void **tmp;
        size_t i, head;

        if (capacity < CDC_DEQUE_MIN_CAPACITY) {
                if (d->capacity > CDC_DEQUE_MIN_CAPACITY)
                        capacity = CDC_DEQUE_MIN_CAPACITY;
                else
                        return CDC_STATUS_OK;
        }

        tmp = (void **)malloc(capacity * sizeof (void *));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        for (i = 0, head = d->head; i < d->size; ++i, head = ++head % d->size)
                tmp[i] = d->buffer[head];

        free(d->buffer);

        d->tail     = d->size;
        d->head     = 0;
        d->capacity = capacity;
        d->buffer   = tmp;

        return CDC_STATUS_OK;
}

static inline enum cdc_stat cdc_deque_grow(cdc_deque_t *d)
{
        assert(d != NULL);

        return cdc_deque_reallocate(d, d->capacity * CDC_DEQUE_COPACITY_EXP);
}

static inline enum cdc_stat cdc_deque_shrink(cdc_deque_t *d)
{
        assert(d != NULL);

        return cdc_deque_reallocate(d, d->capacity / CDC_DEQUE_COPACITY_EXP);
}

static inline enum cdc_stat cdc_deque_pop_back_f(cdc_deque_t *d,
                                                 bool must_free)
{
        assert(d != NULL);
        assert(d->size > 0);

        d->tail = (d->tail - 1 + d->capacity) % d->capacity;

        if (must_free && d->fp_free)
                (*d->fp_free)(d->buffer[d->tail]);

        --d->size;

        if (cdc_deque_should_shrink(d)) {
                enum cdc_stat ret = cdc_deque_shrink(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

static inline enum cdc_stat cdc_deque_pop_front_f(cdc_deque_t *d,
                                                  bool must_free)
{
        assert(d != NULL);
        assert(d->size > 0);

        if (must_free && d->fp_free)
                (*d->fp_free)(d->buffer[d->head]);

        d->head = (d->head + 1) % d->capacity;
        --d->size;

        if (cdc_deque_should_shrink(d)) {
                enum cdc_stat ret = cdc_deque_shrink(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

static inline void cdc_deque_move_left(cdc_deque_t *d, size_t index)
{
        assert(d != NULL);

}

static inline void cdc_deque_move_right(cdc_deque_t *d, size_t index,
                                        size_t count)
{
        assert(d != NULL);

        size_t idx, end = (d->tail - 1) % d->capacity;

        while (count--) {
                idx = (end + 1) % d->capacity;
                d->buffer[idx] = d->buffer[end];
                end = --end % d->capacity;
        }
}

static inline void cdc_deque_free_range(cdc_deque_t *d, size_t start,
                                        size_t end)
{
        assert(d != NULL);
        assert(start < d->size);
        assert(end <= d->size);

        size_t nstart = (d->head + start) % d->capacity;
        size_t count = end - start;

        while (count--) {
                (*d->fp_free)(d->buffer[nstart]);
                nstart = ++nstart % d->capacity;
        }
}

static inline enum cdc_stat cdc_deque_init_varg(cdc_deque_t *d, va_list args)
{
        assert(d != NULL);

        enum cdc_stat ret;
        void *elem;

        while ((elem = va_arg(args, void *)) != NULL) {
                ret = cdc_deque_push_back(d, elem);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_ctor(cdc_deque_t **d, cdc_free_func_t func)
{
        assert(d != NULL);

        cdc_deque_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_deque_t *)malloc(sizeof(cdc_deque_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->size     = 0;
        tmp->capacity = 0;
        tmp->buffer   = NULL;
        tmp->fp_free  = func;

        ret = cdc_deque_reallocate(tmp, CDC_DEQUE_MIN_CAPACITY);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *d = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_ctorl(cdc_deque_t **d, cdc_free_func_t func, ...)
{
        assert(d != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, func);
        ret = cdc_deque_ctorv(d, func, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_deque_ctorv(cdc_deque_t **d, cdc_free_func_t func,
                              va_list args)
{
        assert(d != NULL);

        enum cdc_stat ret;

        ret = cdc_deque_ctor(d, func);
        if (ret != CDC_STATUS_OK)
                return ret;

        return cdc_deque_init_varg(*d, args);
}

void cdc_deque_dtor(cdc_deque_t *d)
{
        assert(d != NULL);

        if (d->fp_free)
                cdc_deque_free_range(d, 0, d->size);

        free(d->buffer);
        free(d);
}

void *cdc_deque_get(cdc_deque_t *d, size_t index)
{
        assert(d != NULL);
        assert(index < d->size);

        size_t idx = (d->head + index) % d->capacity;

        return d->buffer[idx];
}

void cdc_deque_set(cdc_deque_t *d, size_t index, void *elem)
{

}

enum cdc_stat cdc_deque_at(cdc_deque_t *d, size_t index, void **elem)
{
        assert(d != NULL);
        assert(elem != NULL);

        size_t idx;

        if (index > d->size)
                return CDC_STATUS_OUT_OF_RANGE;

        idx = (d->head + index) % d->capacity;
        *elem = d->buffer[idx];

        return CDC_STATUS_OK;
}

void *cdc_deque_front(cdc_deque_t *d)
{
        assert(d != NULL);
        assert(d->size > 0);

        return d->buffer[d->head];
}

void *cdc_deque_back(cdc_deque_t *d)
{
        assert(d != NULL);
        assert(d->size > 0);

        ssize_t idx = (d->tail - 1 + d->capacity) % d->capacity;

        return d->buffer[idx];
}

bool cdc_deque_empty(cdc_deque_t *d)
{
        assert(d != NULL);

        return d->size == 0;
}

size_t cdc_deque_size(cdc_deque_t *d)
{
        assert(d != NULL);

        return d->size;
}

enum cdc_stat cdc_deque_insert(cdc_deque_t *d, size_t index, void *elem)
{
        assert(d != NULL);
        assert(index <= d->size);

        size_t idx;

        if (index == 0)
                return cdc_deque_push_front(d, elem);

        if (index == d->size)
                return cdc_deque_push_back(d, elem);

        if (cdc_deque_should_grow(d)) {
                enum cdc_stat ret = cdc_deque_grow(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        cdc_deque_move_right(d, index, d->size - index);

        idx = (d->head + index) % d->capacity;
        d->buffer[idx] = elem;
        d->tail = (d->tail + 1) % d->capacity;
        ++d->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_erase(cdc_deque_t *d, size_t index, void **elem)
{

}

void cdc_deque_clear(cdc_deque_t *d)
{
        assert(d != NULL);

        if (d->fp_free)
                cdc_deque_free_range(d, 0, d->size);

        d->tail = 0;
        d->head = 0;
        d->size = 0;
}

enum cdc_stat cdc_deque_push_back(cdc_deque_t *d, void *elem)
{
        assert(d != NULL);

        if (cdc_deque_should_grow(d)) {
                enum cdc_stat ret = cdc_deque_grow(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        d->buffer[d->tail] = elem;
        d->tail = (d->tail + 1) % d->capacity;
        ++d->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_pop_back(cdc_deque_t *d)
{
        assert(d != NULL);
        assert(d->size > 0);

        return cdc_deque_pop_back_f(d, true);
}

enum cdc_stat cdc_deque_push_front(cdc_deque_t *d, void *elem)
{
        assert(d != NULL);

        if (cdc_deque_should_grow(d)) {
                enum cdc_stat ret = cdc_deque_grow(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        d->head = (d->head - 1 + d->capacity) % d->capacity;
        d->buffer[d->head] = elem;
        ++d->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_pop_front(cdc_deque_t *d)
{
        assert(d != NULL);
        assert(d->size > 0);

        return cdc_deque_pop_front_f(d, true);
}

void cdc_deque_swap(cdc_deque_t *a, cdc_deque_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(size_t,          a->size,     b->size);
        CDC_SWAP(size_t,          a->capacity, b->capacity);
        CDC_SWAP(size_t,          a->head,     b->head);
        CDC_SWAP(size_t,          a->tail,     b->tail);
        CDC_SWAP(void **,         a->buffer,   b->buffer);
        CDC_SWAP(cdc_free_func_t, a->fp_free,  b->fp_free);
}
