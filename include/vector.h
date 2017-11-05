#ifndef CDCONTAINERS_INCLUDE_VECTOR_H
#define CDCONTAINERS_INCLUDE_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include "status.h"

#define CDC_VECTOR_MIN_CAPACITY     (4)
#define CDC_VECTOR_COPACITY_EXP     (2.0f)
#define CDC_VECTOR_SHRINK_THRESHOLD (1.0 / 4.0)
#define CDC_VECTOR_MAX_LEN          (SIZE_MAX)

typedef struct cdc_vector cdc_vector_t;

enum cdc_stat cdc_vector_ctor     (cdc_vector_t **v, void (*fp_free)(void *));
enum cdc_stat cdc_vector_ctor_l   (cdc_vector_t **v, void (*fp_free)(void *), ...);
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

#endif  // CDSTRUCTURES_INCLUDE_VECTOR_H
