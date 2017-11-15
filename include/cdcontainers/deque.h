#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

#define CDC_DEQUE_MIN_CAPACITY     (4)
#define CDC_DEQUE_COPACITY_EXP     (2.0f)
#define CDC_DEQUE_SHRINK_THRESHOLD (1.0 / 4.0)
#define CDC_DEQUE_MAX_LEN          (SIZE_MAX)

typedef struct cdc_deque cdc_deque_t;

enum cdc_stat cdc_deque_ctor      (cdc_deque_t **d, cdc_free_func_t func);
enum cdc_stat cdc_deque_ctorl     (cdc_deque_t **d, cdc_free_func_t func, ...);
enum cdc_stat cdc_deque_ctorv     (cdc_deque_t **d, cdc_free_func_t func, va_list args);
void          cdc_deque_dtor      (cdc_deque_t *d);

// Element access
void *        cdc_deque_get       (cdc_deque_t *d, size_t index);
void          cdc_deque_set       (cdc_deque_t *d, size_t index, void *elem);
enum cdc_stat cdc_deque_at        (cdc_deque_t *d, size_t index, void **elem);
void *        cdc_deque_front     (cdc_deque_t *d);
void *        cdc_deque_back      (cdc_deque_t *d);

// Capacity
bool          cdc_deque_empty     (cdc_deque_t *d);
size_t        cdc_deque_size      (cdc_deque_t *d);

// Modifiers
enum cdc_stat cdc_deque_insert    (cdc_deque_t *d, size_t index, void *elem);
enum cdc_stat cdc_deque_erase     (cdc_deque_t *d, size_t index, void **elem);
void          cdc_deque_clear     (cdc_deque_t *d);
enum cdc_stat cdc_deque_push_back (cdc_deque_t *d, void *elem);
enum cdc_stat cdc_deque_pop_back  (cdc_deque_t *d);
enum cdc_stat cdc_deque_push_front(cdc_deque_t *d, void *elem);
enum cdc_stat cdc_deque_pop_front (cdc_deque_t *d);
void          cdc_deque_swap      (cdc_deque_t *a, cdc_deque_t *b);

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H
