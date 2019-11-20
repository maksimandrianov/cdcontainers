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
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_CASTS_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_CASTS_H

#include <cdcontainers/common.h>

#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define CDC_PTR_TO_CHAR(p) ((char)(intptr_t)(p))
#define CDC_CHAR_TO_PTR(s) ((void *)(intptr_t)(char)(s))

#define CDC_PTR_TO_SCHAR(p) ((signed char)(intptr_t)(p))
#define CDC_SCHAR_TO_PTR(s) ((void *)(intptr_t)(signed char)(s))

#define CDC_PTR_TO_UCHAR(p) ((unsigned char)(uintptr_t)(p))
#define CDC_UCHAR_TO_PTR(s) ((void *)(uintptr_t)(unsigned char)(s))

#define CDC_PTR_TO_SHORT(p) ((short)(intptr_t)(p))
#define CDC_SHORT_TO_PTR(s) ((void *)(intptr_t)(short)(s))

#define CDC_PTR_TO_USHORT(p) ((unsigned short)(uintptr_t)(p))
#define CDC_USHORT_TO_PTR(s) ((void *)(uintptr_t)(unsigned short)(s))

#define CDC_PTR_TO_INT(p) ((int)(intptr_t)(p))
#define CDC_INT_TO_PTR(s) ((void *)(intptr_t)(int)(s))

#define CDC_PTR_TO_UINT(p) ((unsigned int)(uintptr_t)(p))
#define CDC_UINT_TO_PTR(s) ((void *)(uintptr_t)(unsigned int)(s))

#define CDC_PTR_TO_SIZE(p) ((size_t)(uintptr_t)(p))
#define CDC_SIZE_TO_PTR(s) ((void *)(uintptr_t)(size_t)(s))

#if UINTPTR_MAX >= ULONG_MAX
#define CDC_PTR_TO_LONG(p) ((long)(intptr_t)(p))
#define CDC_LONG_TO_PTR(s) ((void *)(intptr_t)(long)(s))
#define CDC_PTR_LONG_CAST

#define CDC_PTR_TO_ULONG(p) ((unsigned long)(uintptr_t)(p))
#define CDC_ULONG_TO_PTR(s) ((void *)(uintptr_t)(unsigned long)(s))
#define CDC_PTR_ULONG_CAST
#endif

#if (defined(__GNUC__)) && defined(__SIZEOF_POINTER__)
#if defined(__SIZEOF_FLOAT__) && (__SIZEOF_POINTER__ >= __SIZEOF_FLOAT__)
union cdc_union_float_ptr {
  void *ptr;
  float flt;
};

static inline void *cdc_float_to_ptr(float flt)
{
  union cdc_union_float_ptr u;
  u.flt = flt;
  return u.ptr;
}

static inline float cdc_ptr_to_float(void *ptr)
{
  union cdc_union_float_ptr u;
  u.ptr = ptr;
  return u.flt;
}

#define CDC_PTR_TO_FLOAT(p) cdc_ptr_to_float(p)
#define CDC_FLOAT_TO_PTR(s) cdc_float_to_ptr(s)
#define CDC_PTR_FLOAT_CAST
#endif

#if defined(__SIZEOF_DOUBLE__) && (__SIZEOF_POINTER__ >= __SIZEOF_DOUBLE__)
union cdc_union_double_ptr {
  void *ptr;
  double dbl;
};

static inline void *cdc_double_to_ptr(double dbl)
{
  union cdc_union_double_ptr u;
  u.dbl = dbl;
  return u.ptr;
}

static inline double cdc_ptr_to_double(void *ptr)
{
  union cdc_union_double_ptr u;
  u.ptr = ptr;
  return u.dbl;
}

#define CDC_PTR_TO_DOUBLE(p) cdc_ptr_to_double(p)
#define CDC_DOUBLE_TO_PTR(s) cdc_double_to_ptr(s)
#define CDC_PTR_DOUBLE_CAST
#endif
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_CASTS_H
