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
#include "cdcontainers/treap.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "data-info.h"

struct node_pair
{
        struct cdc_treap_node *l, *r;
};

static inline int default_prior(void *value)
{
        return rand();
}

static inline struct cdc_treap_node *new_node(void *key, int prior,  void *val)
{
        struct cdc_treap_node *node;

        node = (struct cdc_treap_node *)malloc(sizeof(struct cdc_treap_node));
        if (node) {
                node->priority = prior;
                node->key = key;
                node->data = val;
        }

        return node;
}

static struct node_pair split(struct cdc_treap_node *root, void *key,
                              cdc_compar_fn_t compar)
{
        struct node_pair pair;

        if (root == NULL) {
                pair.l = pair.r = NULL;
                return pair;
        }

        if (compar(root->key, key)) {
                pair = split(root->right, key, compar);
                root->right = pair.l;
                if (pair.l)
                        pair.l->parent = root;

                if (pair.r)
                        pair.r->parent = NULL;

                pair.l = root;
                pair.r = pair.r;
                return pair;
        } else {
                pair = split(root->left, key, compar);
                root->left = pair.r;
                if (pair.l)
                        pair.l->parent = NULL;

                if (pair.r)
                        pair.r->parent = root;

                pair.l = pair.l;
                pair.r = root;
                return pair;
        }
}

static struct cdc_treap_node *merge(struct cdc_treap_node *l,
                                    struct cdc_treap_node *r)
{
        if (l == NULL)
                return r;

        if (r == NULL)
                return l;

        if (l->priority > r->priority) {
                l->right = merge(l->right, r);
                if (l->right)
                        l->right->parent = l;

                return l;
        } else {
                r->left = merge(l, r->left);
                if (r->left)
                        r->left->parent = r;

                return r;
        }
}

static inline struct cdc_treap_node *find_node(struct cdc_treap_node *node,
                                               void *key, cdc_compar_fn_t cmp)
{
        while (node != NULL && cdc_not_eq(cmp, node->key, key)) {
                if (cmp(key, node->key))
                        node = node->left;
                else
                        node = node->right;
        }

        return node;
}

static inline struct cdc_treap_node *min_node(struct cdc_treap_node *node)
{
        if (node == NULL)
                return NULL;

        while (node->left != NULL)
                node = node->left;

        return node;
}

static inline struct cdc_treap_node *max_node(struct cdc_treap_node *node)
{
        if (node == NULL)
                return NULL;

        while (node->right != NULL)
                node = node->right;

        return node;
}

static inline bool ifind(struct cdc_treap *t, void *key, int priority,
                         struct cdc_treap_node **sn, struct cdc_treap_node **pn)
{
        assert(t != NULL);

        struct cdc_treap_node *snode = t->root, *pnode = NULL;

        *sn = snode;
        *pn = pnode;
        while (snode != NULL) {
                if (cdc_eq(t->compar, key, snode->key))
                        return false;

                pnode = snode;
                if (t->compar(key, snode->key))
                        snode = snode->left;
                else
                        snode = snode->right;

                if (snode && snode->priority > priority) {
                        *sn = snode;
                        *pn = pnode;
                }
        }

        return true;
}

static inline void erase_node(struct cdc_treap *t, struct cdc_treap_node *snode)
{
        assert(t != NULL);

        struct cdc_treap_node *node;
        struct cdc_pair pair;

        node = merge(snode->left, snode->right);
        if (t->root == snode) {
                node->parent = NULL;
                t->root = node;
        } else {
                if (node)
                        node->parent = snode->parent;

                if (snode->parent->left && snode->parent->left == snode)
                        snode->parent->left = node;
                else
                        snode->parent->right = node;
        }

        if (CDC_HAS_DFREE(t)) {
                pair.first = snode->key;
                pair.second = snode->data;
                t->dinfo->dfree(&pair);
        }

        free(snode);
}

enum cdc_stat cdc_treap_ctor(struct cdc_treap **t, struct cdc_data_info *info,
                             cdc_compar_fn_t compar, cdc_priority_func_t prior)
{
        assert(t != NULL);

        struct cdc_treap *tmp;

        tmp = (struct cdc_treap *)calloc(sizeof(struct cdc_treap), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
                free(tmp);
                return CDC_STATUS_BAD_ALLOC;
        }

        tmp->prior = prior == NULL ? default_prior : prior;
        tmp->compar = compar;
        *t = tmp;
        return CDC_STATUS_OK;
}

enum cdc_stat cdc_treap_ctorl(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_compar_fn_t compar,
                              cdc_priority_func_t prior, ...)
{

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_treap_ctorv(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_compar_fn_t compar, cdc_priority_func_t prior,
                              va_list args)
{

        return CDC_STATUS_OK;
}

void cdc_treap_dtor(struct cdc_treap *t)
{

}

enum cdc_stat cdc_treap_insert(struct cdc_treap *t, void *key, void *value)
{
        assert(t != NULL);

        struct cdc_treap_node *node, *snode, *pnode;
        struct node_pair pair;
        int priority = t->prior(value);

        if (t->root == NULL) {
                node = new_node(key, priority, value);
                if (!node)
                        return CDC_STATUS_BAD_ALLOC;

                node->parent = node->left = node->right = NULL;
                t->root = node;
                ++t->size;
                return CDC_STATUS_OK;
        }

        if (!ifind(t, key, priority, &snode, &pnode))
                return CDC_STATUS_ALREADY_EXISTS;

        node = new_node(key, priority, value);
        if (!node)
                return CDC_STATUS_BAD_ALLOC;

        if (snode == NULL) {
                node->left = node->right = NULL;
                node->parent = pnode;
                if (t->compar(node->key, pnode->key))
                        pnode->left = node;
                else
                        pnode->right = node;
        } else {
                pair = split(snode, node->key, t->compar);
                node->left = pair.l;
                if (pair.l)
                        pair.l->parent = node;

                node->right = pair.r;
                if (pair.r)
                        pair.r->parent = node;

                if (pnode == NULL) {
                        node->parent = NULL;
                        t->root = node;
                } else if (pnode->left == snode) {
                        node->parent = pnode;
                        pnode->left = node;
                } else {
                        node->parent = pnode;
                        pnode->right = node;
                }
        }

        ++t->size;
        return CDC_STATUS_OK;
}

size_t cdc_treap_erase(struct cdc_treap *t, void *key)
{
        assert(t != NULL);

        struct cdc_treap_node *snode = find_node(t->root, key, t->compar);

        if (snode == NULL)
                return 0;

        erase_node(t, snode);
        --t->size;
        return 1;
}

void cdc_treap_clear(struct cdc_treap *t)
{

}

void cdc_treap_swap(struct cdc_treap *a, struct cdc_treap *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_treap_node *, a->root, b->root);
        CDC_SWAP(size_t, a->size, b->size);
        CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}
