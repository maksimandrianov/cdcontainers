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

static inline void free_tree(struct cdc_binomial_heap *h,
                             struct cdc_binomial_heap_node *root)
{
        assert(h);

        struct cdc_binomial_heap_node *l, *curr, *next;

        if (root == NULL)
                return;

        l = root;
        while (l->child)
                l = l->child;

        do {
                curr = l;
                l = l->parent;
                while (curr) {
                        next = curr->sibling;
                        free_node(h, curr);
                        curr = next;
                }
        } while (l);
}

static inline bool is_heap_tree(struct cdc_binomial_heap_node *root,
                                cdc_compar_fn_t compar)
{
        struct cdc_binomial_heap_node *l, *curr, *next;

        if (root == NULL)
                return true;

        l = root;
        while (l->child)
                l = l->child;

        do {
                curr = l;
                l = l->parent;
                while (curr) {
                        next = curr->sibling;
                        if (curr->parent && compar(curr->key, curr->parent->key))
                                return false;

                        curr = next;
                }
        } while (l);

        return true;
}

static inline void free_trees(struct cdc_binomial_heap *h)
{
        assert(h);

        struct cdc_binomial_heap_node *curr, *next;

        curr = h->root;
        while (curr) {
                next = curr->sibling;
                curr->sibling = NULL;
                free_tree(h, curr);
                curr = next;
        }
}

static inline struct cdc_binomial_heap_node *find_prev_top(
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

static inline struct cdc_binomial_heap_node *find_top(
                struct cdc_binomial_heap *h)
{
        assert(h);

        struct cdc_binomial_heap_node *node;

        node = find_prev_top(h);
        return node ? node->sibling : h->root;
}

static inline void update_top(struct cdc_binomial_heap *h,
                              struct cdc_binomial_heap_node *v)
{
        assert(h);

        if (h->top == NULL)
                h->top = v;
        else if (v && h->compar(v->key, h->top->key))
                h->top = v;
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
                struct cdc_binomial_heap_node *b,
                cdc_compar_fn_t compare)
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

static inline struct cdc_binomial_heap_node *decrease_key(
                struct cdc_binomial_heap *h, struct cdc_binomial_heap_iter pos,
                void *key)
{
        assert(h);
        assert(h == pos.container);

        struct cdc_binomial_heap_node *node = pos.current, *p = node->parent;

        if (CDC_HAS_DFREE(h))
                h->dinfo->dfree(node->key);

        node->key = key;
        while (p != NULL && h->compar(node->key, p->key)) {
                CDC_SWAP(void *, node->key, p->key);
                node = p;
                p = node->parent;
        }

        return node;
}

static inline void increase_key(struct cdc_binomial_heap *h,
                                struct cdc_binomial_heap_iter pos, void *key)
{
        assert(h);
        assert(h == pos.container);

        struct cdc_binomial_heap_node *curr = pos.current, *ch = curr->child, *t;

        if (CDC_HAS_DFREE(h))
                h->dinfo->dfree(curr->key);

        curr->key = key;
        while (ch != NULL) {
                if (h->compar(ch->key, curr->key)) {
                        t = ch;
                        while (ch->sibling != NULL) {
                                if (h->compar(ch->sibling->key, t->key))
                                        t = ch->sibling;

                                ch = ch->sibling;
                        }

                        CDC_SWAP(void *, t->key, curr->key);
                        curr = t;
                        ch = curr->child;
                } else {
                        ch = ch->sibling;
                }
        }
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
        assert(h);

        free_trees(h);
}

enum cdc_stat cdc_binomial_heap_extract_top(struct cdc_binomial_heap *h)
{
        assert(h);

        struct cdc_binomial_heap_node *root, *tmp, *prev_top = find_prev_top(h);

        if (prev_top) {
                tmp = prev_top->sibling;
                prev_top->sibling = tmp->sibling;
        } else {
                tmp = h->root;
                h->root = tmp->sibling;
        }

        root = tmp->child;
        free_node(h, tmp);
        tmp = root;
        while (tmp) {
                tmp->parent = NULL;
                tmp = tmp->sibling;
        }

        h->root = meld(h->root, root, h->compar);
        h->top = find_top(h);
        --h->size;
        return CDC_STATUS_OK;
}

enum cdc_stat cdc_binomial_heap_riinsert(struct cdc_binomial_heap *h, void *key,
                                         struct cdc_binomial_heap_iter *ret)
{
        assert(h);

        struct cdc_binomial_heap_node *root;

        root = (struct cdc_binomial_heap_node *)calloc(
                                sizeof(struct cdc_binomial_heap_node), 1);
        if (!root)
                return CDC_STATUS_BAD_ALLOC;

        root->key = key;
        update_top(h, root);
        h->root = meld(h->root, root, h->compar);
        ++h->size;
        if (ret) {
                ret->container = h;
                ret->current = root;
        }

        return CDC_STATUS_OK;
}

void cdc_binomial_heap_change_key(struct cdc_binomial_heap *h,
                                  struct cdc_binomial_heap_iter pos, void *key)
{
        assert(h);

        struct cdc_binomial_heap_node *node;

        if (h->compar(key, pos.current->key)) {
                node = decrease_key(h, pos, key);
                update_top(h, node);

        } else {
                increase_key(h, pos, key);
                if (pos.current == h->top)
                        h->top = find_top(h);
        }
}

void cdc_binomial_heap_clear(struct cdc_binomial_heap *h)
{
        assert(h);

        free_trees(h);
        h->size = 0;
        h->root = NULL;
        h->top = NULL;
}

void cdc_binomial_heap_swap(struct cdc_binomial_heap *a,
                            struct cdc_binomial_heap *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_binomial_heap_node *, a->root, b->root);
        CDC_SWAP(struct cdc_binomial_heap_node *, a->top, b->top);
        CDC_SWAP(size_t, a->size, b->size);
        CDC_SWAP(cdc_compar_fn_t, a->compar, b->compar);
        CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}

void cdc_binomial_heap_merge(struct cdc_binomial_heap *h,
                             struct cdc_binomial_heap *other)
{
        assert(h);
        assert(other);

        h->root = meld(h->root, other->root, h->compar);
        update_top(h, other->top);
        h->size += other->size;
        other->size = 0;
        other->root = NULL;
        other->top = NULL;
}

bool cdc_binomial_heap_is_heap(struct cdc_binomial_heap *h)
{
        assert(h);

        struct cdc_binomial_heap_node *curr, *next;

        curr = h->root;
        while (curr) {
                next = curr->sibling;
                if (!is_heap_tree(curr, h->compar))
                        return false;

                curr = next;
        }

        return true;
}
