#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_STACK_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_STACK_H

#include <cdcontainers/stackl.h>
#include <cdcontainers/stackv.h>

typedef struct cdc_stackv_t cdc_stack_t;

#define cdc_stackv_ctor(...)   cdc_stackv_ctor(__VA_ARGS__)
#define cdc_stackv_ctorl(...)  cdc_stackv_ctorl(__VA_ARGS__)
#define cdc_stackv_ctorv(...)  cdc_stackv_ctorv(__VA_ARGS__)
#define cdc_stackv_dtor(...)   cdc_stackv_dtor(__VA_ARGS__)

// Element access
#define cdc_stackv_top(...)    cdc_stackv_top(__VA_ARGS__)

// Capacity
#define cdc_stackv_empty(...)  cdc_stackv_empty(__VA_ARGS__)
#define cdc_stackv_size(...)   cdc_stackv_size(__VA_ARGS__)

// Modifiers
#define cdc_stackv_push(...)   cdc_stackv_push(__VA_ARGS__)
#define cdc_stackv_pop(...)    cdc_stackv_pop(__VA_ARGS__)
#define cdc_stackv_swap(...)   cdc_stackv_swap(__VA_ARGS__)

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_stackv_t stack_t;

#define stack_ctor(...)   cdc_stack_ctor(__VA_ARGS__)
#define stack_ctorl(...)  cdc_stack_ctorl(__VA_ARGS__)
#define stack_ctorv(...)  cdc_stack_ctorv(__VA_ARGS__)
#define stack_dtor(...)   cdc_stack_dtor(__VA_ARGS__)

// Element access
#define stack_top(...)    cdc_stack_top(__VA_ARGS__)

// Capacity
#define stack_empty(...)  cdc_stack_empty(__VA_ARGS__)
#define stack_size(...)   cdc_stack_size(__VA_ARGS__)

// Modifiers
#define stack_push(...)   cdc_stack_push(__VA_ARGS__)
#define stack_pop(...)    cdc_stack_pop(__VA_ARGS__)
#define stack_swap(...)   cdc_stack_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_STACK_H
