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
  * @brief The cdc_treap is a struct and functions that provide a cartesion tree
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_TREAP_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_TREAP_H

#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

typedef int (*cdc_priority_func_t)(void *);

struct cdc_treap_node {
        struct cdc_treap_node *parent;
        struct cdc_treap_node *left;
        struct cdc_treap_node *right;
        void *key;
        void *data;
        int priority;
};


struct cdc_treap {
        struct cdc_treap_node *root;
        size_t size;
        cdc_priority_func_t prior;
        cdc_compar_func_t compar;
        struct cdc_data_info *dinfo;
};

enum cdc_stat cdc_treap_ctor(struct cdc_treap **t, struct cdc_data_info *info,
                             cdc_compar_func_t compar, cdc_priority_func_t prior);

enum cdc_stat cdc_treap_ctorl(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_compar_func_t compar,
                              cdc_priority_func_t prior, ...);

enum cdc_stat cdc_treap_ctorv(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_compar_func_t compar, cdc_priority_func_t prior,
                              va_list args);

void cdc_treap_dtor(struct cdc_treap *t);

static inline size_t cdc_treap_size(struct cdc_treap *t)
{
        assert(t != NULL);

        return t->size;
}

static inline bool cdc_treap_empty(struct cdc_treap *t)
{
        assert(t != NULL);

        return t->size == 0;
}

enum cdc_stat cdc_treap_insert(struct cdc_treap *t, void *key, void *value);

size_t cdc_treap_erase(struct cdc_treap *t, void *key);

void cdc_treap_clear(struct cdc_treap *t);

void cdc_treap_swap(struct cdc_treap *a, struct cdc_treap *b);

#endif  // CDSTRUCTURES_INCLUDE_CDCONTAINERS_VECTOR_H
