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
/**
 * @file
 * @author Maksim Andrianov <maksimandrianov1@yandex.ru>
 * @brief The cdc_sequence_table is a sequence container interface
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_ISEQUENCE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_ISEQUENCE_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief The cdc_sequence_table struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_sequence_table {
  enum cdc_stat (*ctor)(void **cntr, struct cdc_data_info *info);
  enum cdc_stat (*ctorv)(void **cntr, struct cdc_data_info *info, va_list args);
  void (*dtor)(void *cntr);
  void *(*front)(void *cntr);
  void *(*back)(void *cntr);
  bool (*empty)(void *cntr);
  size_t (*size)(void *cntr);
  enum cdc_stat (*push_back)(void *cntr, void *elem);
  void (*pop_back)(void *cntr);
  enum cdc_stat (*push_front)(void *cntr, void *elem);
  void (*pop_front)(void *cntr);
  enum cdc_stat (*insert)(void *cntr, size_t index, void *value);
  void (*erase)(void *cntr, size_t index);
  void (*clear)(void *cntr);
  void *(*get)(void *cntr, size_t index);
  void (*set)(void *cntr, size_t index, void *value);
};

extern const struct cdc_sequence_table *cdc_seq_array;
extern const struct cdc_sequence_table *cdc_seq_carray;
extern const struct cdc_sequence_table *cdc_seq_list;

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_sequence_table sequence_table_t;
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_ISEQUENCE_H
