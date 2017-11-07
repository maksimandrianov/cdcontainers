#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUEL_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUEL_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef struct cdc_queue_list cdc_queuel_t;

enum cdc_stat cdc_queuel_ctor (cdc_queuel_t **q, cdc_free_func_t func);
enum cdc_stat cdc_queuel_ctorl(cdc_queuel_t **q, cdc_free_func_t func, ...);
enum cdc_stat cdc_queuel_ctorv(cdc_queuel_t **q, cdc_free_func_t func, va_list args);
void          cdc_queuel_dtor (cdc_queuel_t *q);

// Element access
void *        cdc_queuel_front(cdc_queuel_t *q);
void *        cdc_queuel_back (cdc_queuel_t *q);

// Capacity
bool          cdc_queuel_empty(cdc_queuel_t *q);
size_t        cdc_queuel_size (cdc_queuel_t *q);

// Modifiers
enum cdc_stat cdc_queuel_push (cdc_queuel_t *q, void *elem);
enum cdc_stat cdc_queuel_pop  (cdc_queuel_t *q);
void          cdc_queuel_swap (cdc_queuel_t *a, cdc_queuel_t *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef cdc_queuel_t queuel_t;

#define queuel_ctor(...)   cdc_queuel_ctor(__VA_ARGS__)
#define queuel_ctorl(...)  cdc_queuel_ctorl(__VA_ARGS__)
#define queuel_ctorv(...)  cdc_queuel_ctorv(__VA_ARGS__)
#define queuel_dtor(...)   cdc_queuel_dtor(__VA_ARGS__)

// Element access
#define queuel_front(...)  cdc_queuel_front(__VA_ARGS__)
#define queuel_back(...)   cdc_queuel_back(__VA_ARGS__)

// Capacity
#define queuel_empty(...)  cdc_queuel_empty(__VA_ARGS__)
#define queuel_size(...)   cdc_queuel_size(__VA_ARGS__)

// Modifiers
#define queuel_push(...)   cdc_queuel_push(__VA_ARGS__)
#define queuel_pop(...)    cdc_queuel_pop(__VA_ARGS__)
#define queuel_swap(...)   cdc_queuel_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUEL_H
