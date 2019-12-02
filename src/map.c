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

#include "cdcontainers/data-info.h"

enum cdc_stat cdc_map_ctor(const struct cdc_map_table *table,
                           struct cdc_map **m, struct cdc_data_info *info)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_CMP(info));

  struct cdc_map *tmp = (struct cdc_map *)malloc(sizeof(struct cdc_map));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->table = table;
  enum cdc_stat stat = tmp->table->ctor(&tmp->container, info);
  if (stat != CDC_STATUS_OK) {
    free(tmp);
    return stat;
  }

  *m = tmp;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_map_ctorl(const struct cdc_map_table *table,
                            struct cdc_map **m, struct cdc_data_info *info, ...)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  enum cdc_stat stat = cdc_map_ctorv(table, m, info, args);
  va_end(args);
  return stat;
}

enum cdc_stat cdc_map_ctorv(const struct cdc_map_table *table,
                            struct cdc_map **m, struct cdc_data_info *info,
                            va_list args)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_CMP(info));

  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_CMP(info));

  struct cdc_map *tmp = (struct cdc_map *)malloc(sizeof(struct cdc_map));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->table = table;
  enum cdc_stat stat = tmp->table->ctorv(&tmp->container, info, args);
  if (stat != CDC_STATUS_OK) {
    free(tmp);
    return stat;
  }

  *m = tmp;
  return stat;
}

void cdc_map_dtor(struct cdc_map *m)
{
  assert(m != NULL);

  m->table->dtor(m->container);
  free(m);
}

enum cdc_stat cdc_map_iter_ctor(struct cdc_map *m, struct cdc_map_iter *it)
{
  assert(m != NULL);
  assert(it != NULL);

  it->table = m->table->iter_table;
  it->iter = it->table->ctor();
  if (!it->iter) {
    return CDC_STATUS_BAD_ALLOC;
  }

  return CDC_STATUS_OK;
}

void cdc_map_iter_dtor(struct cdc_map_iter *it)
{
  assert(it != NULL);

  it->table->dtor(it->iter);
}
