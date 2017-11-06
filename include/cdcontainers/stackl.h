#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKL_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKL_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef struct cdc_stack_list cdc_stackl_t;

enum cdc_stat cdc_stackl_ctor (cdc_stackl_t **s, cdc_free_func_t func);
enum cdc_stat cdc_stackl_ctorl(cdc_stackl_t **s, cdc_free_func_t func, ...);
enum cdc_stat cdc_stackl_ctorv(cdc_stackl_t **s, cdc_free_func_t func, va_list args);
void          cdc_stackl_dtor (cdc_stackl_t *s);

// Element access
void *        cdc_stackl_top  (cdc_stackl_t *s);

// Capacity
bool          cdc_stackl_empty(cdc_stackl_t *s);
size_t        cdc_stackl_size (cdc_stackl_t *s);

// Modifiers
enum cdc_stat cdc_stackl_push (cdc_stackl_t *s, void *elem);
enum cdc_stat cdc_stackl_pop  (cdc_stackl_t *s);
void          cdc_stackl_swap (cdc_stackl_t *a, cdc_stackl_t *b);

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKL_H
