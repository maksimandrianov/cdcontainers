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
#ifndef CDCONTAINERS_SRC_DATA_INFO_H
#define CDCONTAINERS_SRC_DATA_INFO_H

#include "cdcontainers/common.h"

struct cdc_data_info *cdc_di_shared_ctorc(struct cdc_data_info *other);
void cdc_di_shared_dtor(struct cdc_data_info *info);

#define CDC_HAS_DFREE(dinfo) (dinfo && dinfo->dfree)
#define CDC_HAS_CMP(dinfo) (dinfo && dinfo->cmp)
#define CDC_HAS_EQ(dinfo) (dinfo && dinfo->eq)
#define CDC_HAS_CP(dinfo) (dinfo && dinfo->cp)
#define CDC_HAS_SIZE(dinfo) (dinfo && dinfo->size)

static inline int cdc_eq(int (*pred)(const void *, const void *), const void *l,
                         const void *r)
{
  return !(pred(l, r) || pred(r, l));
}

static inline int cdc_not_eq(int (*pred)(const void *, const void *),
                             const void *l, const void *r)
{
  return pred(l, r) || pred(r, l);
}

static inline int cdc_gt(int (*pred)(const void *, const void *), const void *l,
                         const void *r)
{
  return pred(r, l);
}

static inline int cdc_gte(int (*pred)(const void *, const void *),
                          const void *l, const void *r)
{
  return !pred(l, r);
}

static inline int cdc_lte(int (*pred)(const void *, const void *),
                          const void *l, const void *r)
{
  return !pred(r, l);
}

#endif  // CDCONTAINERS_SRC_DATA_INFO_Hs
