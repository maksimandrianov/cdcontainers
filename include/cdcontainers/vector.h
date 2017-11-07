#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_VECTOR_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

#define CDC_VECTOR_MIN_CAPACITY     (4)
#define CDC_VECTOR_COPACITY_EXP     (2.0f)
#define CDC_VECTOR_SHRINK_THRESHOLD (1.0 / 4.0)
#define CDC_VECTOR_MAX_LEN          (SIZE_MAX)

typedef struct cdc_vector cdc_vector_t;

enum cdc_stat cdc_vector_ctor     (cdc_vector_t **v, cdc_free_func_t func);
enum cdc_stat cdc_vector_ctorl    (cdc_vector_t **v, cdc_free_func_t func, ...);
enum cdc_stat cdc_vector_ctorv    (cdc_vector_t **v, cdc_free_func_t func, va_list args);
void          cdc_vector_dtor     (cdc_vector_t *v);

// Element access
void *        cdc_vector_get      (cdc_vector_t *v, size_t index);
enum cdc_stat cdc_vector_at       (cdc_vector_t *v, size_t index, void **elem);
void *        cdc_vector_front    (cdc_vector_t *v);
void *        cdc_vector_back     (cdc_vector_t *v);
void **       cdc_vector_data     (cdc_vector_t *v);

// Capacity
enum cdc_stat cdc_vector_reserve  (cdc_vector_t *v, size_t capacity);
bool          cdc_vector_empty    (cdc_vector_t *v);
size_t        cdc_vector_size     (cdc_vector_t *v);
size_t        cdc_vector_capacity (cdc_vector_t *v);
float         cdc_vector_cap_exp  (cdc_vector_t *v);

// Modifiers
enum cdc_stat cdc_vector_insert   (cdc_vector_t *v, size_t index, void *elem);
enum cdc_stat cdc_vector_erase    (cdc_vector_t *v, size_t index, void **elem);
void          cdc_vector_clear    (cdc_vector_t *v);
enum cdc_stat cdc_vector_push_back(cdc_vector_t *v, void *elem);
enum cdc_stat cdc_vector_pop_back (cdc_vector_t *v);
void          cdc_vector_swap     (cdc_vector_t *a, cdc_vector_t *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef cdc_vector_t vector_t;

#define vector_ctor(...)      cdc_vector_ctor(__VA_ARGS__)
#define vector_ctorl(...)     cdc_vector_ctorl(__VA_ARGS__)
#define vector_ctorv(...)     cdc_vector_ctorv(__VA_ARGS__)
#define vector_dtor(...)      cdc_vector_dtor(__VA_ARGS__)

// Element access
#define vector_get(...)       cdc_vector_get(__VA_ARGS__)
#define vector_at(...)        cdc_vector_at(__VA_ARGS__)
#define vector_front(...)     cdc_vector_front(__VA_ARGS__)
#define vector_back(...)      cdc_vector_back(__VA_ARGS__)
#define vector_data(...)      cdc_vector_data(__VA_ARGS__)

// Capacity
#define vector_reserve(...)   cdc_vector_reserve(__VA_ARGS__)
#define vector_empty(...)     cdc_vector_empty(__VA_ARGS__)
#define vector_size(...)      cdc_vector_size(__VA_ARGS__)
#define vector_capacity(...)  cdc_vector_capacity(__VA_ARGS__)
#define vector_cap_exp(...)   cdc_vector_cap_exp(__VA_ARGS__)

// Modifiers
#define vector_insert(...)    cdc_vector_insert(__VA_ARGS__)
#define vector_erase(...)     cdc_vector_erase(__VA_ARGS__)
#define vector_clear(...)     cdc_vector_clear(__VA_ARGS__)
#define vector_push_back(...) cdc_vector_push_back(__VA_ARGS__)
#define vector_pop_back(...)  cdc_vector_pop_back(__VA_ARGS__)
#define vector_swap(...)      cdc_vector_swap(__VA_ARGS__)
#endif

#endif  // CDSTRUCTURES_INCLUDE_CDCONTAINERS_VECTOR_H
