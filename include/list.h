#ifndef CDSTRUCTURES_INCLUDE_LIST_H
#define CDSTRUCTURES_INCLUDE_LIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "status.h"

typedef struct cds_list cds_list_t;

enum cds_stat cds_list_ctor      (cds_list_t **l, void (*fp_free)(void *));
enum cds_stat cds_list_ctor_l    (cds_list_t **l, void (*fp_free)(void *), ...);
void          cds_list_dtor      (cds_list_t *l);

enum cds_stat cds_list_at        (cds_list_t *l, size_t index, void **elem);

enum cds_stat cds_list_push_back (cds_list_t *l, void *elem);
enum cds_stat cds_list_pop_back  (cds_list_t *l);

enum cds_stat cds_list_push_front(cds_list_t *l, void *elem);
enum cds_stat cds_list_pop_front (cds_list_t *l);

size_t        cds_list_size      (cds_list_t *l);
bool          cds_list_empty     (cds_list_t *l);

void          cds_list_foreach   (cds_list_t *l, void (*cb)(void *, size_t));


#endif  // CDSTRUCTURES_INCLUDE_LIST_H
