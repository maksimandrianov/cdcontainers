#ifndef CDSTRUCTURES_INCLUDE_VECTOR_H
#define CDSTRUCTURES_INCLUDE_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include "status.h"

typedef struct cds_vector cds_vector_t;

enum cds_stat cds_vector_ctor     (cds_vector_t **v);
enum cds_stat cds_vector_ctor_list(cds_vector_t **v, ...);
void          cds_vector_dtor     (cds_vector_t *v);
void          cds_vector_dtor_f   (cds_vector_t *v, void (*f)(void *));

enum cds_stat cds_vector_reserve  (cds_vector_t *v, size_t size);

void *        cds_vector_data     (cds_vector_t *v);

void *        cds_vector_get      (cds_vector_t *v, size_t index);
const void *  cds_vector_const_get(cds_vector_t *v, size_t index);
enum cds_stat cds_vector_at       (cds_vector_t *v, size_t index, void **elem);

void *        cds_vector_front    (cds_vector_t *v);
void *        cds_vector_back     (cds_vector_t *v);

enum cds_stat cds_vector_insert   (cds_vector_t *v, size_t index, void *elem);
enum cds_stat cds_vector_erase    (cds_vector_t *v, size_t index, void **elem);

void          cds_vector_clear    (cds_vector_t *v);
void          cds_vector_clear_f  (cds_vector_t *v, void (*f)(void *));

enum cds_stat cds_vector_push_back(cds_vector_t *v, void *elem);
enum cds_stat cds_vector_pop_back (cds_vector_t *v);

bool          cds_vector_empty    (cds_vector_t *v);
size_t        cds_vector_size     (cds_vector_t *v);
size_t        cds_vector_capacity (cds_vector_t *v);

void          cds_vector_swap     (cds_vector_t *a, cds_vector_t *b);

#endif // CDSTRUCTURES_INCLUDE_VECTOR_H
