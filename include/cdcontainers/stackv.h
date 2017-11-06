#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKV_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKV_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef struct cdc_stack_vector cdc_stackv_t;

enum cdc_stat cdc_stackv_ctor (cdc_stackv_t **s, cdc_free_func_t func);
enum cdc_stat cdc_stackv_ctorl(cdc_stackv_t **s, cdc_free_func_t func, ...);
enum cdc_stat cdc_stackv_ctorv(cdc_stackv_t **s, cdc_free_func_t func, va_list args);
void          cdc_stackv_dtor (cdc_stackv_t *s);

// Element access
void *        cdc_stackv_top  (cdc_stackv_t *s);

// Capacity
bool          cdc_stackv_empty(cdc_stackv_t *s);
size_t        cdc_stackv_size (cdc_stackv_t *s);

// Modifiers
enum cdc_stat cdc_stackv_push (cdc_stackv_t *s, void *elem);
enum cdc_stat cdc_stackv_pop  (cdc_stackv_t *s);
void          cdc_stackv_swap (cdc_stackv_t *a, cdc_stackv_t *b);

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKV_H
