#ifndef CDSTRUCTURES_INCLUDE_VECTOR_H
#define CDSTRUCTURES_INCLUDE_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include "status.h"

#define CDS_VECTOR_MIN_CAPACITY     (4)
#define CDS_VECTOR_COPACITY_EXP     (2.0f)
#define CDS_VECTOR_SHRINK_THRESHOLD (1.0 / 4.0)
#define CDS_VECTOR_MAX_LEN          (SIZE_MAX)

typedef struct cds_vector cds_vector_t;

enum cds_stat cds_vector_ctor     (cds_vector_t **v, void (*fp_free)(void *));
enum cds_stat cds_vector_ctor_l   (cds_vector_t **v, void (*fp_free)(void *), ...);
void          cds_vector_dtor     (cds_vector_t *v);

enum cds_stat cds_vector_reserve  (cds_vector_t *v, size_t capacity);

void **       cds_vector_data     (cds_vector_t *v);

void *        cds_vector_get      (cds_vector_t *v, size_t index);
enum cds_stat cds_vector_at       (cds_vector_t *v, size_t index, void **elem);

void *        cds_vector_front    (cds_vector_t *v);
void *        cds_vector_back     (cds_vector_t *v);

enum cds_stat cds_vector_insert   (cds_vector_t *v, size_t index, void *elem);
enum cds_stat cds_vector_erase    (cds_vector_t *v, size_t index, void **elem);

void          cds_vector_clear    (cds_vector_t *v);

enum cds_stat cds_vector_push_back(cds_vector_t *v, void *elem);
enum cds_stat cds_vector_pop_back (cds_vector_t *v);

bool          cds_vector_empty    (cds_vector_t *v);
size_t        cds_vector_size     (cds_vector_t *v);
size_t        cds_vector_capacity (cds_vector_t *v);
float         cds_vector_cap_exp  (cds_vector_t *v);

void          cds_vector_swap     (cds_vector_t *a, cds_vector_t *b);

#endif  // CDSTRUCTURES_INCLUDE_VECTOR_H
