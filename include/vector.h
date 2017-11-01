#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include "status.h"

struct cds_vector;

enum cds_stat cds_vec_ctor     (struct cds_vector *v);
void          cds_vec_dtor     (struct cds_vector *v);

enum cds_stat cds_vec_reserve  (struct cds_vector *v, size_t size);

void *        cds_vec_get      (struct cds_vector *v, size_t index);
const void *  cds_vec_const_get(struct cds_vector *v, size_t index);
enum cds_stat cds_vec_at       (struct cds_vector *v, size_t index, void *elem);

void *        cds_vec_front    (struct cds_vector *v);
void *        cds_vec_back     (struct cds_vector *v);

enum cds_stat cds_vec_push_back(struct cds_vector *v, const void *elem);
enum cds_stat cds_vec_pop_back (struct cds_vector *v);


size_t        cds_vec_size     (struct cds_vector *v);
size_t        cds_vec_capacity (struct cds_vector *v);

#endif // VECTOR_H
