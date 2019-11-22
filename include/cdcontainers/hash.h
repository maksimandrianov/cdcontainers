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
 * @brief This file contains different utilities for hashing.
 * The ideas of algorithms were borrowed from the boost library.
 * http://www.boost.org/doc/libs/1_64_0/boost/functional/hash/hash.hpp
 * http://www.boost.org/doc/libs/1_64_0/boost/functional/hash/detail/hash_float.hpp
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_HASH_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_HASH_H

#include <cdcontainers/casts.h>
#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

typedef size_t (*cdc_hash_fn_t)(void *);

#define CDC_DIGITS_CHAR (CHAR_BIT - (CHAR_MIN < 0))
#define CDC_DIGITS_SCHAR (CHAR_BIT - 1)
#define CDC_DIGITS_UCHAR (CHAR_BIT)
#define CDC_DIGITS_SHORT (CHAR_BIT * sizeof(short) - 1)
#define CDC_DIGITS_USHORT (CHAR_BIT * sizeof(short))
#define CDC_DIGITS_INT (CHAR_BIT * sizeof(int) - 1)
#define CDC_DIGITS_UINT (CHAR_BIT * sizeof(int))
#define CDC_DIGITS_LONG (CHAR_BIT * sizeof(long) - 1)
#define CDC_DIGITS_ULONG (CHAR_BIT * sizeof(long))
#define CDC_DIGITS_SIZE (CHAR_BIT * sizeof(size_t))

#define MAKE_SIGNED_HASH(T, DIGITS, NAME)                          \
  static inline size_t cdc_hash_##NAME(T val)                      \
  {                                                                \
    const unsigned int size_t_bits = CDC_DIGITS_SIZE;              \
    const int length = (DIGITS - 1) / (int)(size_t_bits);          \
    unsigned int i;                                                \
    size_t seed = 0;                                               \
    T positive = val < 0 ? -1 - val : val;                         \
    for (i = length * size_t_bits; i > 0; i -= size_t_bits)        \
      seed ^= (size_t)(positive >> i) + (seed << 6) + (seed >> 2); \
    seed ^= (size_t)val + (seed << 6) + (seed >> 2);               \
    return seed;                                                   \
  }

#define MAKE_UNSIGNED_HASH(T, DIGITS, NAME)                    \
  static inline size_t cdc_hash_##NAME(T val)                  \
  {                                                            \
    const unsigned int size_t_bits = CDC_DIGITS_SIZE;          \
    const int length = (DIGITS - 1) / (int)(size_t_bits);      \
    unsigned int i;                                            \
    size_t seed = 0;                                           \
    for (i = length * size_t_bits; i > 0; i -= size_t_bits)    \
      seed ^= (size_t)(seed >> i) + (seed << 6) + (seed >> 2); \
    seed ^= (size_t)val + (seed << 6) + (seed >> 2);           \
    return seed;                                               \
  }

static inline size_t cdc_hash_float_combine(size_t seed, size_t value)
{
  seed ^= value + (seed << 6) + (seed >> 2);
  return seed;
}

static inline size_t cdc_hash_binary(char *ptr, size_t length)
{
  size_t seed = 0;

  if (length >= sizeof(size_t)) {
    memcpy(&seed, ptr, sizeof(size_t));
    length -= sizeof(size_t);
    ptr += sizeof(size_t);

    while (length >= sizeof(size_t)) {
      size_t buffer = 0;
      memcpy(&buffer, ptr, sizeof(size_t));
      seed = cdc_hash_float_combine(seed, buffer);
      length -= sizeof(size_t);
      ptr += sizeof(size_t);
    }
  }

  if (length > 0) {
    size_t buffer = 0;
    memcpy(&buffer, ptr, length);
    seed = cdc_hash_float_combine(seed, buffer);
  }

  return seed;
}

#define MAKE_FLOAT_HASH(T, NAME)                     \
  static inline size_t cdc_hash_##NAME(T val)        \
  {                                                  \
    return cdc_hash_binary((char *)&val, sizeof(T)); \
  }

MAKE_SIGNED_HASH(signed char, CDC_DIGITS_SCHAR, schar)
MAKE_SIGNED_HASH(short, CDC_DIGITS_SHORT, short)
MAKE_SIGNED_HASH(int, CDC_DIGITS_INT, int)
MAKE_SIGNED_HASH(long, CDC_DIGITS_LONG, long)

MAKE_FLOAT_HASH(float, float)
MAKE_FLOAT_HASH(double, double)
MAKE_FLOAT_HASH(long double, ldouble)

MAKE_UNSIGNED_HASH(unsigned char, CDC_DIGITS_UCHAR, uchar)
MAKE_UNSIGNED_HASH(unsigned short, CDC_DIGITS_USHORT, ushort)
MAKE_UNSIGNED_HASH(unsigned int, CDC_DIGITS_UINT, uint)
MAKE_UNSIGNED_HASH(unsigned long, CDC_DIGITS_ULONG, ulong)

#if CHAR_MIN < 0
MAKE_SIGNED_HASH(char, CDC_DIGITS_CHAR, char)
#else
MAKE_UNSIGNED_HASH(char, CDC_DIGITS_CHAR, char)
#endif

#define MAKE_POINTER_DATA_HASH(T, NAME, CAST_FUNC)  \
  static inline size_t cdc_pdhash_##NAME(void *val) \
  {                                                 \
    T t = CAST_FUNC(val);                           \
    return cdc_hash_##NAME(t);                      \
  }

MAKE_POINTER_DATA_HASH(char, char, CDC_TO_CHAR)
MAKE_POINTER_DATA_HASH(signed char, schar, CDC_TO_SCHAR)
MAKE_POINTER_DATA_HASH(unsigned char, uchar, CDC_TO_UCHAR)
MAKE_POINTER_DATA_HASH(short, short, CDC_TO_SHORT)
MAKE_POINTER_DATA_HASH(unsigned short, ushort, CDC_TO_USHORT)
MAKE_POINTER_DATA_HASH(int, int, CDC_TO_INT)
MAKE_POINTER_DATA_HASH(unsigned int, uint, CDC_TO_UINT)

#ifdef CDC_LONG_CAST
MAKE_POINTER_DATA_HASH(long, long, CDC_TO_LONG)
MAKE_POINTER_DATA_HASH(unsigned long, ulong, CDC_TO_ULONG)
#endif
#ifdef CDC_FLOAT_CAST
MAKE_POINTER_DATA_HASH(float, float, CDC_TO_FLOAT)
#endif
#ifdef CDC_DOUBLE_CAST
MAKE_POINTER_DATA_HASH(double, double, CDC_TO_DOUBLE)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_HASH_H
