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
 * @brief The cdc_map_table is a map interface
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_IMAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_IMAP_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief The cdc_map_iter_table struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_map_iter_table {
  void *(*alloc)();
  void (*free)(void *it);
  void (*next)(void *it);
  void (*prev)(void *it);
  bool (*has_next)(void *it);
  bool (*has_prev)(void *it);
  void *(*key)(void *it);
  void *(*value)(void *it);
  struct cdc_pair (*key_value)(void *it);
  bool (*eq)(void *it1, void *it2);
};

/**
 * @brief The cdc_map_table struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_map_table {
  enum cdc_stat (*ctor)(void **cntr, struct cdc_data_info *info,
                        cdc_binary_pred_fn_t compar);
  enum cdc_stat (*ctorv)(void **cntr, struct cdc_data_info *info,
                         cdc_binary_pred_fn_t compar, va_list args);
  void (*dtor)(void *cntr);
  enum cdc_stat (*get)(void *cntr, void *key, void **value);
  size_t (*count)(void *cntr, void *key);
  void (*find)(void *cntr, void *key, void *it);
  size_t (*size)(void *cntr);
  bool (*empty)(void *cntr);
  void (*clear)(void *cntr);
  enum cdc_stat (*insert)(void *cntr, void *key, void *value, void *it,
                          bool *inserted);
  enum cdc_stat (*insert_or_assign)(void *cntr, void *key, void *value,
                                    void *it, bool *inserted);
  size_t (*erase)(void *cntr, void *key);
  void (*swap)(void *a, void *b);
  void (*begin)(void *cntr, void *it);
  void (*end)(void *cntr, void *it);
  const struct cdc_map_iter_table *iter_table;
};

extern const void *cdc_map_avl;
extern const void *cdc_map_splay;
extern const void *cdc_map_treap;

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_IMAP_H
