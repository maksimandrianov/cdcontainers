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
#include "cdcontainers/circular-array.h"
#include "cdcontainers/interfaces/isequence.h"

#include <assert.h>

static enum cdc_stat ctor(void **cntr, struct cdc_data_info *info)
{
  assert(cntr != NULL);

  struct cdc_circular_array **circular_array =
      (struct cdc_circular_array **)cntr;
  return cdc_circular_array_ctor(circular_array, info);
}

static enum cdc_stat ctorv(void **cntr, struct cdc_data_info *info,
                           va_list args)
{
  assert(cntr != NULL);

  struct cdc_circular_array **circular_array =
      (struct cdc_circular_array **)cntr;
  return cdc_circular_array_ctorv(circular_array, info, args);
}

static void dtor(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  cdc_circular_array_dtor(circular_array);
}

static void *front(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  return cdc_circular_array_front(circular_array);
}

static void *back(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  return cdc_circular_array_back(circular_array);
}

static bool empty(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  return cdc_circular_array_empty(circular_array);
}

static size_t size(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  return cdc_circular_array_size(circular_array);
}

static enum cdc_stat push_back(void *cntr, void *elem)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  return cdc_circular_array_push_back(circular_array, elem);
}

static void pop_back(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  cdc_circular_array_pop_back(circular_array);
}

static enum cdc_stat push_front(void *cntr, void *elem)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  return cdc_circular_array_push_front(circular_array, elem);
}

static void pop_front(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  cdc_circular_array_pop_front(circular_array);
}

static enum cdc_stat insert(void *cntr, size_t index, void *value)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  return cdc_circular_array_insert(circular_array, index, value);
}

static void erase(void *cntr, size_t index)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  cdc_circular_array_erase(circular_array, index);
}

static void clear(void *cntr)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  cdc_circular_array_clear(circular_array);
}

static void *get(void *cntr, size_t index)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  return cdc_circular_array_get(circular_array, index);
}

static void set(void *cntr, size_t index, void *value)
{
  assert(cntr != NULL);

  struct cdc_circular_array *circular_array = (struct cdc_circular_array *)cntr;
  cdc_circular_array_set(circular_array, index, value);
}

static const struct cdc_sequence_table _table = {.ctor = ctor,
                                                 .ctorv = ctorv,
                                                 .dtor = dtor,
                                                 .front = front,
                                                 .back = back,
                                                 .empty = empty,
                                                 .size = size,
                                                 .push_back = push_back,
                                                 .pop_back = pop_back,
                                                 .push_front = push_front,
                                                 .pop_front = pop_front,
                                                 .insert = insert,
                                                 .erase = erase,
                                                 .clear = clear,
                                                 .get = get,
                                                 .set = set};

const struct cdc_sequence_table *cdc_seq_carray = &_table;
