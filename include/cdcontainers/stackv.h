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

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef cdc_stackv_t stackv_t;

#define stackv_ctor(...)   cdc_stackv_ctor(__VA_ARGS__)
#define stackv_ctorl(...)  cdc_stackv_ctorl(__VA_ARGS__)
#define stackv_ctorv(...)  cdc_stackv_ctorv(__VA_ARGS__)
#define stackv_dtor(...)   cdc_stackv_dtor(__VA_ARGS__)

// Element access
#define stackv_top(...)    cdc_stackv_top(__VA_ARGS__)

// Capacity
#define stackv_empty(...)  cdc_stackv_empty(__VA_ARGS__)
#define stackv_size(...)   cdc_stackv_size(__VA_ARGS__)

// Modifiers
#define stackv_push(...)   cdc_stackv_push(__VA_ARGS__)
#define stackv_pop(...)    cdc_stackv_pop(__VA_ARGS__)
#define stackv_swap(...)   cdc_stackv_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKV_H
