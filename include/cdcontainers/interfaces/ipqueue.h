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
 * @brief The cdc_priority_queue_table is a priority queue interface
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_IPQUEUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_IPQUEUE_H

#include <cdcontainers/common.h>
#include <cdcontainers/status.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief The cdc_priority_queue_table struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_priority_queue_table {
  enum cdc_stat (*ctor)(void **cntr, struct cdc_data_info *info);
  enum cdc_stat (*ctorv)(void **cntr, struct cdc_data_info *info, va_list args);
  void (*dtor)(void *cntr);
  void *(*top)(void *cntr);
  bool (*empty)(void *cntr);
  size_t (*size)(void *cntr);
  enum cdc_stat (*push)(void *cntr, void *elem);
  void (*pop)(void *cntr);
};

extern const void *cdc_pq_heap;
extern const void *cdc_pq_binheap;
extern const void *cdc_pq_pheap;

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_INTERFACES_IPQUEUE_H
