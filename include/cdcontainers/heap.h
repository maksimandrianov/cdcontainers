#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

struct cdc_heap {
        struct cdc_vector *vector;
        cdc_compar_func_t compar;
        struct cdc_data_info *dinfo;
};

enum cdc_stat cdc_heap_ctor(struct cdc_heap **h, struct cdc_data_info *info,
                            cdc_compar_func_t compar);
enum cdc_stat cdc_heap_ctorl(struct cdc_heap **h, struct cdc_data_info *info,
                             cdc_compar_func_t compar, ...);
enum cdc_stat cdc_heap_ctorv(struct cdc_heap **h, struct cdc_data_info *info,
                             cdc_compar_func_t compar, va_list args);
void cdc_heap_dtor(struct cdc_heap *h);

// Element access
void *cdc_heap_top(struct cdc_heap *h);

// Capacity
size_t cdc_heap_size(struct cdc_heap *h);
bool cdc_heap_empty(struct cdc_heap *h);

// Modifiers
enum cdc_stat cdc_heap_extract_top(struct cdc_heap *h);
enum cdc_stat cdc_heap_insert(struct cdc_heap *h, void *key);
void cdc_heap_increase_key(struct cdc_heap *h, size_t i, void *key);
void cdc_heap_swap(struct cdc_heap *a, struct cdc_heap *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_heap heap_t;

#define heap_ctor(...)         cdc_heap_ctor(__VA_ARGS__)
#define heap_ctorl(...)        cdc_heap_ctorl(__VA_ARGS__)
#define heap_ctorv(...)        cdc_heap_ctorv(__VA_ARGS__)
#define heap_dtor(...)         cdc_heap_dtor(__VA_ARGS__)

// Element access
#define heap_top(...)          cdc_heap_top(__VA_ARGS__)

// Capacity
#define heap_empty(...)        cdc_heap_empty(__VA_ARGS__)
#define heap_size(...)         cdc_heap_size(__VA_ARGS__)

// Modifiers
#define heap_extract_top(...)  cdc_heap_extract_top(__VA_ARGS__)
#define heap_insert(...)       cdc_heap_insert(__VA_ARGS__)
#define heap_increase_key(...) cdc_heap_increase_key(__VA_ARGS__)
#define heap_swap(...)         cdc_heap_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_HEAP_H
