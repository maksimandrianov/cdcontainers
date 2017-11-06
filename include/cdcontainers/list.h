#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef struct cdc_list cdc_list_t;

enum cdc_stat cdc_list_ctor      (cdc_list_t **l, cdc_free_func_t func);
enum cdc_stat cdc_list_ctorl     (cdc_list_t **l, cdc_free_func_t func, ...);
enum cdc_stat cdc_list_ctorv     (cdc_list_t **l, cdc_free_func_t func, va_list args);
void          cdc_list_dtor      (cdc_list_t *l);

// Element access
enum cdc_stat cdc_list_at        (cdc_list_t *l, size_t index, void **elem);
void *        cdc_list_front     (cdc_list_t *l);
void *        cdc_list_back      (cdc_list_t *l);

// Capacity
size_t        cdc_list_size      (cdc_list_t *l);
bool          cdc_list_empty     (cdc_list_t *l);

// Modifiers
enum cdc_stat cdc_list_push_back (cdc_list_t *l, void *elem);
enum cdc_stat cdc_list_pop_back  (cdc_list_t *l);
enum cdc_stat cdc_list_push_front(cdc_list_t *l, void *elem);
enum cdc_stat cdc_list_pop_front (cdc_list_t *l);
enum cdc_stat cdc_list_insert    (cdc_list_t *l, size_t index, void *elem);
enum cdc_stat cdc_list_erase     (cdc_list_t *l, size_t index, void **elem);
void          cdc_list_clear     (cdc_list_t *l);
void          cdc_list_swap      (cdc_list_t *a, cdc_list_t *b);

void          cdc_list_foreach   (cdc_list_t *l, void (*cb)(void *));


#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_LIST_H
