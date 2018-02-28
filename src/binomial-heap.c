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
#include "cdcontainers/binomial-heap.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "data-info.h"

static inline void free_node(struct cdc_binomial_heap *h,
                             struct cdc_binomial_heap_node *node)
{
        assert(h);

        if (CDC_HAS_DFREE(h))
                h->dinfo->dfree(node->key);

        free(node);
}

static inline struct cdc_binomial_heap_node *prev_top(
                struct cdc_binomial_heap *h)
{
        assert(h);

        struct cdc_binomial_heap_node *m, *prev = NULL, *next = h->root;

        if (h->root == NULL)
                return NULL;

        m = h->root;
        while (next->sibling) {
                prev = next;
                if (h->compar(m->key, next->key))
                        m = next->key;
                next = next->sibling;
        }

        return prev;
}

static inline struct cdc_binomial_heap_node *get_prev_top(
                struct cdc_binomial_heap *h)
{
        assert(h);

        return h->prev_top ? h->prev_top : prev_top(h);
}

static inline struct cdc_binomial_heap_node *update_top(
                struct cdc_binomial_heap *h, struct cdc_binomial_heap_node *v)
{
        assert(h);

        struct cdc_binomial_heap_node *top =
                        h->prev_top ? h->prev_top->sibling->key : h->root->key;

        if (h->compar(v->key, top))
                h->prev_top = v;

        return h->prev_top;
}

static inline void link(struct cdc_binomial_heap_node *a,
                        struct cdc_binomial_heap_node *broot)
{

        a->parent = broot;
        a->sibling = broot->child;
        broot->child = a;
        ++broot->degree;
}

static inline struct cdc_binomial_heap_node *merge(
                struct cdc_binomial_heap_node *a,
                struct cdc_binomial_heap_node *b)
{
        struct cdc_binomial_heap_node *result = NULL, *next;

        if (a == NULL)
                return b;

        if (b == NULL)
                return a;

        while (1) {
                if (b == NULL || a == NULL)
                        break;

                if (a->degree < b->degree) {
                        if (result == NULL) {
                                result = next = a;
                        } else {
                                next->sibling = a;
                                next = a;
                        }

                        a = a->sibling;
                } else {
                        if (result == NULL) {
                                result = next = b;
                        } else {
                                next->sibling = b;
                                next = b;
                        }

                        b = b->sibling;
                }
        }

        if (a == NULL)
                next->sibling = b;
        else
                next->sibling = a;

        return result;
}

static inline struct cdc_binomial_heap_node *meld(
                struct cdc_binomial_heap_node *a,
                struct cdc_binomial_heap_node *b, cdc_compar_fn_t compare)
{
        assert(compare);

        struct cdc_binomial_heap_node *result, *prev, *curr, *next;

        result = merge(a, b);
        if (result == NULL)
                return result;

        prev = NULL;
        curr = result;
        next = result->sibling;
        while (next) {
                if (curr->degree != next->degree ||
                    (next->sibling && next->sibling->degree != curr->degree)) {
                        prev = curr;
                        curr = next;
                } else if (compare(curr->key, next->key)) {
                        curr->sibling = next->sibling;
                        link(next, curr);
                } else {
                        if (prev == NULL)
                                result = next;
                        else
                                prev->sibling = next;

                        link(curr, next);
                        next = curr;
                }

                next = curr->sibling;
        }

        return result;
}

static inline enum cdc_stat init_varg(struct cdc_binomial_heap *h, va_list args)
{
        assert(h != NULL);

        enum cdc_stat ret;
        void *elem;

        while ((elem = va_arg(args, void *)) != NULL) {
                ret = cdc_binomial_heap_insert(h, elem);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_binomial_heap_ctor(struct cdc_binomial_heap **h,
                                     struct cdc_data_info *info,
                                     cdc_compar_fn_t compar)
{
        assert(h !=  NULL);

        struct cdc_binomial_heap *tmp;

        tmp = (struct cdc_binomial_heap *)calloc(
                                sizeof(struct cdc_binomial_heap), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
                free(tmp);
                return CDC_STATUS_BAD_ALLOC;
        }
        tmp->compar = compar;
        *h = tmp;
        return CDC_STATUS_OK;
}

enum cdc_stat cdc_binomial_heap_ctorl(struct cdc_binomial_heap **h,
                                      struct cdc_data_info *info,
                                      cdc_compar_fn_t compar, ...)
{
        assert(h != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, compar);
        ret = cdc_binomial_heap_ctorv(h, info, compar, args);
        va_end(args);
        return ret;
}

enum cdc_stat cdc_binomial_heap_ctorv(struct cdc_binomial_heap **h,
                                      struct cdc_data_info *info,
                                      cdc_compar_fn_t compar, va_list args)
{
        assert(h != NULL);

        enum cdc_stat ret;

        ret = cdc_binomial_heap_ctor(h, info, compar);
        if (ret != CDC_STATUS_OK)
                return ret;

        return init_varg(*h, args);
}

void cdc_binomial_heap_dtor(struct cdc_binomial_heap *h)
{

}

enum cdc_stat cdc_binomial_heap_extract_top(struct cdc_binomial_heap *h)
{
        assert(h);

        struct cdc_binomial_heap_node *root, *tmp, *prev_top = get_prev_top(h);

        if (prev_top) {
                tmp = prev_top->sibling;
                prev_top->sibling = tmp->sibling;
        } else {
                tmp = h->root;
                h->root = tmp->sibling;
        }

        root = tmp->child;
        free_node(h, tmp);
        h->prev_top = NULL;
        tmp = root;
        while (tmp) {
                tmp->parent = NULL;
                tmp = tmp->sibling;
        }

        h->root = meld(h->root, root, h->compar);
        --h->size;
        h->prev_top = get_prev_top(h);
        return CDC_STATUS_OK;
}

enum cdc_stat cdc_binomial_heap_insert(struct cdc_binomial_heap *h, void *key)
{
        assert(h);

        struct cdc_binomial_heap_node *root;

        root = (struct cdc_binomial_heap_node *)calloc(
                                sizeof(struct cdc_binomial_heap_node), 1);
        if (!root)
                return CDC_STATUS_BAD_ALLOC;

        root->key = key;
        h->root = meld(h->root, root, h->compar);
        ++h->size;
//        update_top(h, root);
        h->prev_top = prev_top(h);
        return CDC_STATUS_OK;
}

void cdc_binomial_heap_increase_key(struct cdc_binomial_heap *h, size_t i,
                                    void *key)
{

}

void cdc_binomial_heap_swap(struct cdc_binomial_heap *a,
                            struct cdc_binomial_heap *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_binomial_heap_node *, a->root, b->root);
        CDC_SWAP(struct cdc_binomial_heap_node *, a->prev_top, b->prev_top);
        CDC_SWAP(size_t, a->size, b->size);
        CDC_SWAP(cdc_compar_fn_t, a->compar, b->compar);
        CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}
