#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUED_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUED_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef struct cdc_queue_deque cdc_queued_t;

enum cdc_stat cdc_queued_ctor (cdc_queued_t **q, cdc_free_func_t func);
enum cdc_stat cdc_queued_ctorl(cdc_queued_t **q, cdc_free_func_t func, ...);
enum cdc_stat cdc_queued_ctorv(cdc_queued_t **q, cdc_free_func_t func, va_list args);
void          cdc_queued_dtor (cdc_queued_t *q);

// Element access
void *        cdc_queued_front(cdc_queued_t *q);
void *        cdc_queued_back (cdc_queued_t *q);

// Capacity
bool          cdc_queued_empty(cdc_queued_t *q);
size_t        cdc_queued_size (cdc_queued_t *q);

// Modifiers
enum cdc_stat cdc_queued_push (cdc_queued_t *q, void *elem);
enum cdc_stat cdc_queued_pop  (cdc_queued_t *q);
void          cdc_queued_swap (cdc_queued_t *a, cdc_queued_t *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef cdc_queued_t queued_t;

#define queued_ctor(...)   cdc_queued_ctor(__VA_ARGS__)
#define queued_ctorl(...)  cdc_queued_ctorl(__VA_ARGS__)
#define queued_ctorv(...)  cdc_queued_ctorv(__VA_ARGS__)
#define queued_dtor(...)   cdc_queued_dtor(__VA_ARGS__)

// Element access
#define queued_front(...)  cdc_queued_front(__VA_ARGS__)
#define queued_back(...)   cdc_queued_back(__VA_ARGS__)

// Capacity
#define queued_empty(...)  cdc_queued_empty(__VA_ARGS__)
#define queued_size(...)   cdc_queued_size(__VA_ARGS__)

// Modifiers
#define queued_push(...)   cdc_queued_push(__VA_ARGS__)
#define queued_pop(...)    cdc_queued_pop(__VA_ARGS__)
#define queued_swap(...)   cdc_queued_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUED_H
