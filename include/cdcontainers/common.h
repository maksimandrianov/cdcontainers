// The MIT License (MIT)
// Copyright (c) 2017 Maksim Andrianov
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
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H

#include <cdcontainers/casts.h>
#include <cdcontainers/hash.h>

#include <stdbool.h>
#include <stddef.h>

#define CDC_END CDC_FROM_INT(UINTPTR_MAX)

#define CDC_MAX(a, b) ((a) > (b) ? (a) : (b))

#define CDC_MIN(a, b) ((a) < (b) ? (a) : (b))

#define CDC_ABS(x) ((x < 0) ? -(x) : x)

#define CDC_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define CDC_SWAP(T, x, y) \
  do {                    \
    T tmp = x;            \
    x = y;                \
    y = tmp;              \
  } while (0)

#define CDC_INIT_STRUCT \
  {                     \
    0,                  \
  }

#define CDC_STATIC_ASSERT(COND, MSG) typedef char cdc_static_assertion_##MSG[(COND) ? 1 : -1]

typedef void (*cdc_free_fn_t)(void *);
typedef int (*cdc_unary_pred_fn_t)(const void *);
typedef int (*cdc_binary_pred_fn_t)(const void *, const void *);
typedef void (*cdc_copy_fn_t)(void *, const void *);

struct cdc_pair {
  void *first;
  void *second;
};

/**
 * @brief The cdc_data_info struct used to initialize contaners.
 *
 * This gives more data about the data stored in the container.
 *
 */
struct cdc_data_info {
  /**
   * @brief dfree - callback free data.
   *
   * If this function is not NULL, then it will be automatically applied to free
   * memory for cotainer items.
   */
  cdc_free_fn_t dfree;
  /**
   * @brief cmp - callback less or greater.
   *
   * Used for containers where check for less or greater is required.
   * For example, for a cdc_map.
   */
  cdc_binary_pred_fn_t cmp;
  /**
   * @brief eq - callback equil.
   *
   * Used for containers where equality check is required. For example, for a
   * cdc_hash_table.
   */
  cdc_binary_pred_fn_t eq;
  cdc_hash_fn_t hash;
  cdc_copy_fn_t cp;
  size_t size;
  /**
   * @brief __cnt
   *
   * To avoid problems, do not change this field in the code.
   */
  size_t __cnt;
};

static inline size_t cdc_up_to_pow2(size_t x)
{
  x = x - 1;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x + 1;
}

enum cdc_iterator_type {
  // Forward iterator category.
  CDC_FWD_ITERATOR,
  // Bidirectional iterator category.
  CDC_BIDIR_ITERATOR,
  // Random-access iterator category.
  CDC_RA_ITERATOR
};

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef enum cdc_iterator_type iterator_type_t;

typedef struct cdc_pair pair_t;
typedef struct cdc_data_info data_info_t;
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H
