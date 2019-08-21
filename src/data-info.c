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
#include "data-info.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct cdc_data_info *cdc_di_shared_ctorc(struct cdc_data_info *other)
{
  assert(other != NULL);

  if (other->__cnt++ != 0) {
    return other;
  }

  struct cdc_data_info *result =
      (struct cdc_data_info *)malloc(sizeof(struct cdc_data_info));
  if (result) {
    memcpy(result, other, sizeof(struct cdc_data_info));
  }

  return result;
}

void cdc_di_shared_dtor(struct cdc_data_info *info)
{
  if (info && --info->__cnt == 0) {
    free(info);
  }
}
