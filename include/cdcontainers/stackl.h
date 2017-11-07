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

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef cdc_stackl_t stackl_t;

#define stackl_ctor(...)   cdc_stackl_ctor(__VA_ARGS__)
#define stackl_ctorl(...)  cdc_stackl_ctorl(__VA_ARGS__)
#define stackl_ctorv(...)  cdc_stackl_ctorv(__VA_ARGS__)
#define stackl_dtor(...)   cdc_stackl_dtor(__VA_ARGS__)

// Element access
#define stackl_top(...)    cdc_stackl_top(__VA_ARGS__)

// Capacity
#define stackl_empty(...)  cdc_stackl_empty(__VA_ARGS__)
#define stackl_size(...)   cdc_stackl_size(__VA_ARGS__)

// Modifiers
#define stackl_push(...)   cdc_stackl_push(__VA_ARGS__)
#define stackl_pop(...)    cdc_stackl_pop(__VA_ARGS__)
#define stackl_swap(...)   cdc_stackl_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKL_H
