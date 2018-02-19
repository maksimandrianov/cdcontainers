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

#include <stdbool.h>
#include <stddef.h>

#define CDC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CDC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CDC_SWAP(T, x, y) do \
{ \
        T tmp = x;  \
        x = y; \
        y = tmp; \
        } while (0)
#define CDC_INIT_STRUCT {0,}

struct cdc_data_info {
        void (*dfree)(void *);
        bool (*lt)(const void *, const void *);
        size_t size;
        size_t __cnt;
};

typedef int (*cdc_compar_func_t)(const void *, const void *);


#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H
