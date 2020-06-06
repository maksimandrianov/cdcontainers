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
#define CDC_USE_SHORT_NAMES
#include "cdcontainers/adapters/map.h"

#include "cdcontainers/data-info.h"

stat_t map_ctor(const map_table_t *table, map_t **m, data_info_t *info)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_CMP(info));

  map_t *tmp = (map_t *)malloc(sizeof(map_t));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->table = table;
  stat_t stat = tmp->table->ctor(&tmp->container, info);
  if (stat != CDC_STATUS_OK) {
    free(tmp);
    return stat;
  }

  *m = tmp;
  return CDC_STATUS_OK;
}

stat_t map_ctorl(const map_table_t *table, map_t **m, data_info_t *info, ...)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  stat_t stat = map_ctorv(table, m, info, args);
  va_end(args);
  return stat;
}

stat_t map_ctorv(const map_table_t *table, map_t **m, data_info_t *info, va_list args)
{
  assert(table != NULL);
  assert(m != NULL);
  assert(CDC_HAS_CMP(info));

  map_t *tmp = (map_t *)malloc(sizeof(map_t));
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->table = table;
  stat_t stat = tmp->table->ctorv(&tmp->container, info, args);
  if (stat != CDC_STATUS_OK) {
    free(tmp);
    return stat;
  }

  *m = tmp;
  return stat;
}

void map_dtor(map_t *m)
{
  assert(m != NULL);

  m->table->dtor(m->container);
  free(m);
}

stat_t map_iter_ctor(map_t *m, map_iter_t *it)
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
