#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef struct cdc_heap cdc_heap_t;

enum cdc_stat cdc_heap_ctor        (cdc_heap_t **h, cdc_free_func_t func, cdc_compar_func_t compar);
enum cdc_stat cdc_heap_ctorl       (cdc_heap_t **h, cdc_free_func_t func, cdc_compar_func_t compar, ...);
enum cdc_stat cdc_heap_ctorv       (cdc_heap_t **h, cdc_free_func_t func, cdc_compar_func_t compar, va_list args);
void          cdc_heap_dtor        (cdc_heap_t *h);

// Element access
void *        cdc_heap_top         (cdc_heap_t *h);

// Capacity
size_t        cdc_heap_size        (cdc_heap_t *h);
bool          cdc_heap_empty       (cdc_heap_t *h);

// Modifiers
enum cdc_stat cdc_heap_extract_top (cdc_heap_t *h);
enum cdc_stat cdc_heap_insert      (cdc_heap_t *h, void *key);
void          cdc_heap_increase_key(cdc_heap_t *h, size_t i, void *key);
void          cdc_heap_swap        (cdc_heap_t *a, cdc_heap_t *b);


#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H
