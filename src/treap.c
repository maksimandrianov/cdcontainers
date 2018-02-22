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

struct cdc_treap_node_pair
{
        struct cdc_treap_node *l, *r;
};

static struct cdc_treap_node_pair split(struct cdc_treap_node *root, void *key,
                                        cdc_compar_func_t compar)
{
        struct cdc_treap_node_pair pair;

        if (root == NULL) {
                pair.l = pair.r = NULL;
                return pair;
        }

        if (compar(root->key, key)) {
                pair = split(root->rigth, key, compar);
                root->rigth = pair.l;
                pair.l = root;
                pair.r = pair.r;
                return pair;
        } else {
                pair = split(root->left, key, compar);
                root->left = pair.r;
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
                l->rigth = merge(l->rigth, r);
                return l;
        } else {
                l->left = merge(l, r->left);
                return r;
        }
}


enum cdc_stat cdc_treap_ctor(struct cdc_treap **t, struct cdc_data_info *info,
                             cdc_compar_func_t compar)
{
        assert(t !=  NULL);

        struct cdc_treap *tmp;

        tmp = (struct cdc_treap *)calloc(sizeof(struct cdc_treap), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
                free(tmp);
                return CDC_STATUS_BAD_ALLOC;
        }

        tmp->compar = compar;
        *t = tmp;
        return CDC_STATUS_OK;
}

enum cdc_stat cdc_treap_ctorl(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_compar_func_t compar, ...)
{

}

enum cdc_stat cdc_treap_ctorv(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_compar_func_t compar, va_list args)
{

}

void cdc_treap_dtor(struct cdc_treap *t)
{

}

enum cdc_stat cdc_treap_insert(struct cdc_treap *t, void *key, void *value)
{
        struct cdc_treap_node *node, *snode, *pnode = NULL;
        struct cdc_treap_node_pair pair;

        node = (struct cdc_treap_node *)malloc(sizeof(struct cdc_treap_node));
        if (!node)
                return CDC_STATUS_BAD_ALLOC;

        node->priority = rand();
        node->key = key;
        node->data = value;

        if (t->root == NULL) {
                node->left = node->rigth = NULL;
                t->root = node;
        } else {
                snode = t->root;
                while (snode != NULL && snode->priority > node->priority) {
                        pnode  = snode;
                        if (t->compar(node->key, snode->key))
                                snode = snode->left;
                        else
                                snode = snode->rigth;
                }

                if (snode == NULL) {
                        node->left = node->rigth = NULL;
                        if (t->compar(node->key, pnode->key))
                                pnode->left = node;
                        else
                                pnode->rigth = node;
                } else {
                        pair = split(snode, node->key, t->compar);
                        node->left = pair.l;
                        node->rigth = pair.r;

                        if (pnode == NULL)
                                t->root = node;
                        else if (pnode->left == snode)
                                pnode->left = node;
                        else
                                pnode->rigth = node;
                }
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_treap_erase(struct cdc_treap *t, void *key, void **elem)
{

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
