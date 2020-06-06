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
#include "test-common.h"

#include "cdcontainers/casts.h"

#include <CUnit/Basic.h>

void test_ptr_float_cast()
{
#ifdef CDC_FLOAT_CAST
  float value = 123456.123456;

  CU_ASSERT_EQUAL(CDC_TO_FLOAT(CDC_FROM_FLOAT(value)), value);
#endif
}

void test_ptr_double_cast()
{
#ifdef CDC_DOUBLE_CAST
  double value = 123456.123456;

  CU_ASSERT_EQUAL(CDC_TO_DOUBLE(CDC_FROM_DOUBLE(value)), value);
#endif
}
