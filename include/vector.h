#ifndef CDSTRUCTURES_INCLUDE_VECTOR_H
#define CDSTRUCTURES_INCLUDE_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include "status.h"

struct cds_vector;

enum cds_stat cds_vec_ctor     (struct cds_vector **v);
enum cds_stat cds_vec_ctor_list(struct cds_vector **v, ...);
void          cds_vec_dtor     (struct cds_vector *v);
void          cds_vec_dtor_f   (struct cds_vector *v, void (*f)(void *));

enum cds_stat cds_vec_reserve  (struct cds_vector *v, size_t size);

void *        cds_vec_data     (struct cds_vector *v);

void *        cds_vec_get      (struct cds_vector *v, size_t index);
const void *  cds_vec_const_get(struct cds_vector *v, size_t index);
enum cds_stat cds_vec_at       (struct cds_vector *v, size_t index, void *elem);

void *        cds_vec_front    (struct cds_vector *v);
void *        cds_vec_back     (struct cds_vector *v);

enum cds_stat cds_vec_insert   (struct cds_vector *v, size_t index, void *elem);
enum cds_stat cds_vec_erase    (struct cds_vector *v, size_t index, void **elem);

void          cds_vec_clear    (struct cds_vector *v);
void          cds_vec_clear_f  (struct cds_vector *v, void (*f)(void *));

enum cds_stat cds_vec_push_back(struct cds_vector *v, void *elem);
enum cds_stat cds_vec_pop_back (struct cds_vector *v);

bool          cds_vec_empty    (struct cds_vector *v);
size_t        cds_vec_size     (struct cds_vector *v);
size_t        cds_vec_capacity (struct cds_vector *v);

void          cds_vec_swap     (struct cds_vector *a, struct cds_vector *b);

#endif // CDSTRUCTURES_INCLUDE_VECTOR_H
