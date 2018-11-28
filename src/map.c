// The MIT License (MIT)
// Copyright (c) 2018 Maksim Andrianov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
#include "cdcontainers/map.h"
#include "data-info.h"

enum cdc_stat cdc_map_ctor(const struct cdc_map_table *table, struct cdc_map **m,
                           struct cdc_data_info *info)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_LT(info));

  return cdc_map_ctor1(table, m, info, NULL);
}

enum cdc_stat cdc_map_ctorl(const struct cdc_map_table *table, struct cdc_map **m,
                            struct cdc_data_info *info, ...)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_LT(info));

  enum cdc_stat stat;
  va_list args;

  va_start(args, info);
  stat = cdc_map_ctorv(table, m, info, args);
  va_end(args);
  return stat;
}

enum cdc_stat cdc_map_ctorv(const struct cdc_map_table *table, struct cdc_map **m,
                            struct cdc_data_info *info, va_list args)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_LT(info));

  return cdc_map_ctorv1(table, m, info, NULL, args);
}

enum cdc_stat cdc_map_ctor1(const struct cdc_map_table *table, struct cdc_map **m,
                            struct cdc_data_info *info, cdc_binary_pred_fn_t compar)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  struct cdc_map *tmp;
  enum cdc_stat stat;

  tmp = (struct cdc_map *)malloc(sizeof(struct cdc_map));
  if (!tmp)
    return CDC_STATUS_BAD_ALLOC;

  tmp->table = table;
  stat = tmp->table->ctor(&tmp->container, info, compar);
  if (stat != CDC_STATUS_OK) {
    free(tmp);
    return stat;
  }

  *m = tmp;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_map_ctorl1(const struct cdc_map_table *table, struct cdc_map **m,
                             struct cdc_data_info *info, cdc_binary_pred_fn_t compar, ...)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  enum cdc_stat stat;
  va_list args;

  va_start(args, compar);
  stat = cdc_map_ctorv1(table, m, info, compar, args);
  va_end(args);
  return stat;
}

enum cdc_stat cdc_map_ctorv1(const struct cdc_map_table *table, struct cdc_map **m,
                             struct cdc_data_info *info, cdc_binary_pred_fn_t compar,
                             va_list args)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  struct cdc_map *tmp;
  enum cdc_stat stat;

  tmp = (struct cdc_map *)malloc(sizeof(struct cdc_map));
  if (!tmp)
    return CDC_STATUS_BAD_ALLOC;

  tmp->table = table;
  stat = tmp->table->ctorv(&tmp->container, info, compar, args);
  if (stat != CDC_STATUS_OK) {
    free(tmp);
    return stat;
  }

  *m = tmp;
  return stat;
}

enum cdc_stat cdc_mapa_ctor(struct cdc_map **m, struct cdc_data_info *info)
{
  assert(m != NULL);
  assert(CDC_HAS_LT(info));

  return cdc_mapa_ctor1(m, info, NULL);
}

enum cdc_stat cdc_mapa_ctorl(struct cdc_map **m, struct cdc_data_info *info, ...)
{
  assert(m != NULL);
  assert(CDC_HAS_LT(info));

  enum cdc_stat stat;
  va_list args;

  va_start(args, info);
  stat = cdc_mapa_ctorv(m, info, args);
  va_end(args);
  return stat;
}

enum cdc_stat cdc_mapa_ctorv(struct cdc_map **m, struct cdc_data_info *info,
                             va_list args)
{
  assert(m != NULL);
  assert(CDC_HAS_LT(info));

  return cdc_mapa_ctorv1(m, info, NULL, args);
}

enum cdc_stat cdc_mapa_ctor1(struct cdc_map **m, struct cdc_data_info *info,
                             cdc_binary_pred_fn_t compar)
{
  assert(m != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  return cdc_map_ctor1(cdc_map_avl, m, info, compar);
}

enum cdc_stat cdc_mapa_ctorl1(struct cdc_map **m, struct cdc_data_info *info,
                              cdc_binary_pred_fn_t compar, ...)
{
  assert(m != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  enum cdc_stat stat;
  va_list args;

  va_start(args, compar);
  stat = cdc_mapa_ctorv1(m, info, compar, args);
  va_end(args);
  return stat;
}


enum cdc_stat cdc_mapa_ctorv1(struct cdc_map **m, struct cdc_data_info *info,
                              cdc_binary_pred_fn_t compar, va_list args)
{
  assert(m != NULL);
  assert(CDC_HAS_LT(info) || compar != NULL);

  return cdc_map_ctorv1(cdc_map_avl, m, info, compar, args);
}

void cdc_map_dtor(struct cdc_map *m)
{
  assert(m != NULL);

  m->table->dtor(m->container);
  free(m);
}

enum cdc_stat cdc_map_iter_init(struct cdc_map *m, struct cdc_map_iter *it)
{
  assert(m != NULL);
  assert(it != NULL);

  it->table = m->table->iter_table;
  it->iter = it->table->alloc();
  if (!it->iter)
    return CDC_STATUS_BAD_ALLOC;

  return CDC_STATUS_OK;
}

void cdc_map_iter_free(struct cdc_map_iter *it)
{
  assert(it != NULL);

  it->table->free(it->iter);
}
