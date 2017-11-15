#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef struct cdc_deque cdc_deque_t;

enum cdc_stat cdc_deque_ctor      (cdc_deque_t **d, cdc_free_func_t func);
enum cdc_stat cdc_deque_ctorl     (cdc_deque_t **d, cdc_free_func_t func, ...);
enum cdc_stat cdc_deque_ctorv     (cdc_deque_t **d, cdc_free_func_t func, va_deque args);
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

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef cdc_deque_t deque_t;

#define deque_ctor(...)       cdc_deque_ctor(__VA_ARGS__)
#define deque_ctorl(...)      cdc_deque_ctorl(__VA_ARGS__)
#define deque_ctorv(...)      cdc_deque_ctorv(__VA_ARGS__)
#define deque_dtor(...)       cdc_deque_dtor(__VA_ARGS__)

// Element access
#define deque_at(...)         cdc_deque_at(__VA_ARGS__)
#define deque_front(...)      cdc_deque_front(__VA_ARGS__)
#define deque_back(...)       cdc_deque_back(__VA_ARGS__)

// Capacity
#define deque_empty(...)      cdc_deque_empty(__VA_ARGS__)
#define deque_size(...)       cdc_deque_size(__VA_ARGS__)

// Modifiers
#define deque_set(...)        cdc_deque_set(__VA_ARGS__)
#define deque_insert(...)     cdc_deque_insert(__VA_ARGS__)
#define deque_erase(...)      cdc_deque_erase(__VA_ARGS__)
#define deque_clear(...)      cdc_deque_clear(__VA_ARGS__)
#define deque_push_back(...)  cdc_deque_push_back(__VA_ARGS__)
#define deque_pop_back(...)   cdc_deque_pop_back(__VA_ARGS__)
#define deque_push_front(...) cdc_deque_push_front(__VA_ARGS__)
#define deque_pop_front(...)  cdc_deque_pop_back(__VA_ARGS__)
#define deque_swap(...)       cdc_deque_swap(__VA_ARGS__)

#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_DEQUE_H
