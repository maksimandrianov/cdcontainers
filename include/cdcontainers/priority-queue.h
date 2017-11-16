#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef struct cdc_priority_queue cdc_priority_queue_t;

enum cdc_stat cdc_priority_queue_ctor (cdc_priority_queue_t **q, cdc_free_func_t func, cdc_compar_func_t compar);
enum cdc_stat cdc_priority_queue_ctorl(cdc_priority_queue_t **q, cdc_free_func_t func, cdc_compar_func_t compar, ...);
enum cdc_stat cdc_priority_queue_ctorv(cdc_priority_queue_t **q, cdc_free_func_t func, cdc_compar_func_t compar, va_list args);
void          cdc_priority_queue_dtor (cdc_priority_queue_t *q);

// Element access
void *        cdc_priority_queue_top  (cdc_priority_queue_t *q);

// Capacity
bool          cdc_priority_queue_empty(cdc_priority_queue_t *q);
size_t        cdc_priority_queue_size (cdc_priority_queue_t *q);

// Modifiers
enum cdc_stat cdc_priority_queue_push (cdc_priority_queue_t *q, void *elem);
enum cdc_stat cdc_priority_queue_pop  (cdc_priority_queue_t *q);
void          cdc_priority_queue_swap (cdc_priority_queue_t *a, cdc_priority_queue_t *b);

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_PRIORITY_QUEUE_H
