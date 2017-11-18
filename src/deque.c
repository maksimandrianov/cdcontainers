#include "cdcontainers/deque.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define CDC_DEQUE_MIN_CAPACITY     4
#define CDC_DEQUE_COPACITY_EXP     2.0f
#define CDC_DEQUE_SHRINK_THRESHOLD 4.0f
#define CDC_DEQUE_MAX_LEN          SIZE_MAX

static inline bool should_shrink(struct cdc_deque *d)
{
        assert(d != NULL);
        assert(d->size <= d->capacity);

        return d->size * CDC_DEQUE_SHRINK_THRESHOLD <= d->capacity;
}

static inline bool should_grow(struct cdc_deque *d)
{
        assert(d != NULL);
        assert(d->size <= d->capacity);

        return d->size == d->capacity;
}

static inline bool reach_limit_size(struct cdc_deque *d)
{
        assert(d != NULL);
        assert(d != NULL);

        return d->size == CDC_DEQUE_MAX_LEN;
}

static inline enum cdc_stat reallocate(struct cdc_deque *d, size_t capacity)
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

static inline enum cdc_stat grow(struct cdc_deque *d)
{
        assert(d != NULL);

        return reallocate(d, d->capacity * CDC_DEQUE_COPACITY_EXP);
}

static inline enum cdc_stat shrink(struct cdc_deque *d)
{
        assert(d != NULL);

        return reallocate(d, d->capacity / CDC_DEQUE_COPACITY_EXP);
}

static inline enum cdc_stat pop_back_f(struct cdc_deque *d,
                                       bool must_free)
{
        assert(d != NULL);
        assert(d->size > 0);

        d->tail = (d->tail - 1 + d->capacity) % d->capacity;

        if (must_free && CDC_HAS_FREE(d))
                d->dinfo->free(d->buffer[d->tail]);

        --d->size;

        if (should_shrink(d)) {
                enum cdc_stat ret = shrink(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

static inline enum cdc_stat pop_front_f(struct cdc_deque *d,
                                        bool must_free)
{
        assert(d != NULL);
        assert(d->size > 0);

        if (must_free && CDC_HAS_FREE(d))
                d->dinfo->free(d->buffer[d->head]);

        d->head = (d->head + 1) % d->capacity;
        --d->size;

        if (should_shrink(d)) {
                enum cdc_stat ret = shrink(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

static inline void move_left(struct cdc_deque *d, size_t index,
                             size_t count)
{
        assert(d != NULL);

        size_t i, from;
        size_t to = (d->head + index - count - 1 + d->capacity) % d->capacity;

        for (i = 0; i < count; ++i) {
                from = (to + 1) % d->capacity;
                d->buffer[to] = d->buffer[from];
                to = (to + 1) % d->capacity;
        }
}

static inline void move_right(struct cdc_deque *d, size_t index,
                              size_t count)
{
        assert(d != NULL);

        size_t i, from, to = (d->head + index + count) % d->capacity;

        for (i = 0; i < count; ++i) {
                from = (to - 1 + d->capacity) % d->capacity;
                d->buffer[to] = d->buffer[from];
                to = (to - 1  + d->capacity) % d->capacity;
        }
}

static inline void free_range(struct cdc_deque *d, size_t start,
                              size_t end)
{
        assert(d != NULL);
        assert(start < d->size);
        assert(end <= d->size);

        size_t nstart = (d->head + start) % d->capacity;
        size_t count = end - start;

        while (count--) {
                d->dinfo->free(d->buffer[nstart]);
                nstart = (nstart + 1) % d->capacity;
        }
}

static inline enum cdc_stat init_varg(struct cdc_deque *d, va_list args)
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

enum cdc_stat cdc_deque_ctor(struct cdc_deque **d, struct cdc_data_info *info)
{
        assert(d != NULL);

        struct cdc_deque *tmp;
        enum cdc_stat ret;

        tmp = (struct cdc_deque *)malloc(sizeof(struct cdc_deque));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->size     = 0;
        tmp->capacity = 0;
        tmp->buffer   = NULL;
        tmp->dinfo    = info ? cdc_data_info_dcopy(info) : NULL;;

        ret = reallocate(tmp, CDC_DEQUE_MIN_CAPACITY);
        if (ret != CDC_STATUS_OK) {
                free(tmp);
                return ret;
        }

        *d = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_ctorl(struct cdc_deque **d,
                              struct cdc_data_info *info, ...)
{
        assert(d != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = cdc_deque_ctorv(d, info, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_deque_ctorv(struct cdc_deque **d,
                              struct cdc_data_info *info, va_list args)
{
        assert(d != NULL);

        enum cdc_stat ret;

        ret = cdc_deque_ctor(d, info);
        if (ret != CDC_STATUS_OK)
                return ret;

        return init_varg(*d, args);
}

void cdc_deque_dtor(struct cdc_deque *d)
{
        assert(d != NULL);

        if (CDC_HAS_FREE(d))
                free_range(d, 0, d->size);

        free(d->buffer);
        free(d);
}

enum cdc_stat cdc_deque_at(struct cdc_deque *d, size_t index, void **elem)
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

enum cdc_stat cdc_deque_insert(struct cdc_deque *d, size_t index, void *value)
{
        assert(d != NULL);
        assert(index <= d->size);

        size_t idx;

        if (index == 0)
                return cdc_deque_push_front(d, value);

        if (index == d->size)
                return cdc_deque_push_back(d, value);

        if (should_grow(d)) {
                enum cdc_stat ret = grow(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        if (index > d->size / 2) {
                move_right(d, index, d->size - index);
                d->tail = (d->tail + 1) % d->capacity;
        } else {
                move_left(d, index, index);
                d->head = (d->head - 1 + d->capacity) % d->capacity;
        }

        idx  = (d->head + index) % d->capacity;
        d->buffer[idx] = value;
        ++d->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_erase(struct cdc_deque *d, size_t index, void **elem)
{
        assert(d != NULL);
        assert(elem != NULL);
        assert(index < d->size);

        size_t idx = (d->head + index) % d->capacity;

        *elem = d->buffer[idx];

        if (index == d->size - 1)
                return pop_back_f(d, false);

        if (index == 0)
                return pop_front_f(d, false);

        if (index < d->size / 2) {
                move_right(d, 0, index);
                d->head = (d->head + 1) % d->capacity;
        } else {
                move_left(d, d->size + 1, d->size - index);
                d->tail = (d->tail + 1) % d->capacity;
        }

        --d->size;

        if (should_shrink(d)) {
                enum cdc_stat ret = shrink(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

void cdc_deque_clear(struct cdc_deque *d)
{
        assert(d != NULL);

        if (CDC_HAS_FREE(d))
                free_range(d, 0, d->size);

        d->tail = 0;
        d->head = 0;
        d->size = 0;
}

enum cdc_stat cdc_deque_push_back(struct cdc_deque *d, void *value)
{
        assert(d != NULL);

        if (should_grow(d)) {
                enum cdc_stat ret = grow(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        d->buffer[d->tail] = value;
        d->tail = (d->tail + 1) % d->capacity;
        ++d->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_pop_back(struct cdc_deque *d)
{
        assert(d != NULL);
        assert(d->size > 0);

        return pop_back_f(d, true);
}

enum cdc_stat cdc_deque_push_front(struct cdc_deque *d, void *value)
{
        assert(d != NULL);

        if (should_grow(d)) {
                enum cdc_stat ret = grow(d);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        d->head = (d->head - 1 + d->capacity) % d->capacity;
        d->buffer[d->head] = value;
        ++d->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_deque_pop_front(struct cdc_deque *d)
{
        assert(d != NULL);
        assert(d->size > 0);

        return pop_front_f(d, true);
}

void cdc_deque_swap(struct cdc_deque *a, struct cdc_deque *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(size_t, a->size, b->size);
        CDC_SWAP(size_t, a->capacity, b->capacity);
        CDC_SWAP(size_t, a->head, b->head);
        CDC_SWAP(size_t, a->tail, b->tail);
        CDC_SWAP(void **, a->buffer, b->buffer);
        CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}
