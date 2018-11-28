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
#include "cdcontainers/status.h"

#include <stdio.h>

void cdc_print_stat(enum cdc_stat s)
{
  static const char *descriptions[] = {
    "CDC_STATUS_OK",
    "CDC_STATUS_BAD_ALLOC",
    "CDC_STATUS_OUT_OF_RANGE",
    "CDC_STATUS_OVERFLOW",
    "CDC_STATUS_ALREADY_EXISTS",
    "CDC_STATUS_NOT_FOUND",

    "CDC_STATUS_UNKN"
  };

  if (s < CDC_STATUS_OK && s > CDC_STATUS_UNKN)
    s = CDC_STATUS_UNKN;

  printf("%s", descriptions[s]);
}
