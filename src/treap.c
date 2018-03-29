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

static int default_prior(void *value)
{
        return rand();
}

static void free_node(struct cdc_treap *t, struct cdc_treap_node *node)
{
        struct cdc_pair pair;

        if (CDC_HAS_DFREE(t)) {
                pair.first = node->key;
                pair.second = node->value;
                t->dinfo->dfree(&pair);
        }

        free(node);
}

static void free_treap(struct cdc_treap *t, struct cdc_treap_node *root)
{
        if (root == NULL)
                return;

        free_treap(t, root->left);
        free_treap(t, root->right);
        free_node(t, root);
}

static struct cdc_treap_node *new_node(void *key, int prior,  void *val)
{
        struct cdc_treap_node *node;

        node = (struct cdc_treap_node *)malloc(sizeof(struct cdc_treap_node));
        if (node) {
                node->priority = prior;
                node->key = key;
                node->value = val;
                node->parent = node->left = node->right = NULL;
        }

        return node;
}

static struct node_pair split(struct cdc_treap_node *root, void *key,
                              cdc_binary_pred_fn_t compar)
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

static struct cdc_treap_node *find_node(struct cdc_treap_node *node, void *key,
                                        cdc_binary_pred_fn_t cmp)
{
        while (node != NULL && cdc_not_eq(cmp, node->key, key)) {
                if (cmp(key, node->key))
                        node = node->left;
                else
                        node = node->right;
        }

        return node;
}

static struct cdc_treap_node *min_node(struct cdc_treap_node *node)
{
        if (node == NULL)
                return NULL;

        while (node->left != NULL)
                node = node->left;

        return node;
}

static struct cdc_treap_node *max_node(struct cdc_treap_node *node)
{
        if (node == NULL)
                return NULL;

        while (node->right != NULL)
                node = node->right;

        return node;
}

static struct cdc_treap_node *successor(struct cdc_treap_node *node)
{
        struct cdc_treap_node *p;

        if (node->right)
                return min_node(node->right);

        p = node->parent;
        while (p && node == p->right) {
                node = p;
                p = p->parent;
        }

        return p;
}

static struct cdc_treap_node *predecessor(struct cdc_treap_node *node)
{
        struct cdc_treap_node *p;

        if (node->left)
                return max_node(node->left);

        p = node->parent;
        while (p && node == p->left) {
                node = p;
                p = p->parent;
        }

        return p;
}

static struct cdc_treap_node *find_nearest(struct cdc_treap *t, void *key,
                                           int priority)
{
        struct cdc_treap_node *node = t->root, *nearest = node;

        while (node != NULL) {
                if (t->compar(key, node->key))
                        node = node->left;
                else
                        node = node->right;

                if (node && node->priority <= priority)
                        return nearest;
        }

        return nearest;
}

static void erase_node(struct cdc_treap *t, struct cdc_treap_node *node)
{
        struct cdc_treap_node *tmp;

        tmp = merge(node->left, node->right);
        if (t->root == node) {
                if (tmp)
                        tmp->parent = NULL;
                t->root = tmp;
        } else {
                if (tmp)
                        tmp->parent = node->parent;

                if (node->parent->left && node->parent->left == node)
                        node->parent->left = tmp;
                else
                        node->parent->right = tmp;
        }

        --t->size;
        free_node(t, node);
}

static struct cdc_treap_node *insert_unique(struct cdc_treap *t,
                                            struct cdc_treap_node *node,
                                            struct cdc_treap_node *nearest)
{
        struct cdc_treap_node *pnode;
        struct node_pair pair;

        if (t->root == NULL) {
                t->root = node;
        } else {
                pnode = nearest->parent;
                pair = split(nearest, node->key, t->compar);
                node->left = pair.l;
                if (pair.l)
                        pair.l->parent = node;

                node->right = pair.r;
                if (pair.r)
                        pair.r->parent = node;


                if (pnode == NULL) {
                        node->parent = NULL;
                        t->root = node;
                } else if (pnode->left == nearest) {
                        node->parent = pnode;
                        pnode->left = node;
                } else {
                        node->parent = pnode;
                        pnode->right = node;
                }
        }

        ++t->size;
        return node;
}

static struct cdc_treap_node *make_and_insert_unique(struct cdc_treap *t,
                                                     void *key, void *value)
{
        struct cdc_treap_node *node = new_node(key, t->prior(value), value);
        struct cdc_treap_node *nearest;

        if (!node)
                return node;

        nearest = find_nearest(t ,node->key, node->priority);
        return insert_unique(t, node, nearest);

}

static enum cdc_stat init_varg(struct cdc_treap *t, va_list args)
{
        enum cdc_stat stat;
        struct cdc_pair *pair;

        while ((pair = va_arg(args, struct cdc_pair *)) != NULL) {
                stat = cdc_treap_insert(t, pair->first, pair->second, NULL);
                if (stat != CDC_STATUS_OK)
                        return stat;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_treap_ctor(struct cdc_treap **t, struct cdc_data_info *info,
                             cdc_binary_pred_fn_t compar, cdc_priority_fn_t prior)
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

        tmp->prior = prior ? prior : default_prior;
        tmp->compar = compar;
        *t = tmp;
        return CDC_STATUS_OK;
}

enum cdc_stat cdc_treap_ctorl(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_binary_pred_fn_t compar, cdc_priority_fn_t prior, ...)
{
        assert(t != NULL);
        assert(compar != NULL);

        enum cdc_stat stat;
        va_list args;

        va_start(args, prior);
        stat = cdc_treap_ctorv(t, info, compar, prior, args);
        va_end(args);

        return stat;
}

enum cdc_stat cdc_treap_ctorv(struct cdc_treap **t, struct cdc_data_info *info,
                              cdc_binary_pred_fn_t compar, cdc_priority_fn_t prior,
                              va_list args)
{
        assert(t != NULL);
        assert(compar != NULL);

        enum cdc_stat stat;

        stat = cdc_treap_ctor(t, info, compar, prior);
        if (stat != CDC_STATUS_OK)
                return stat;

        return init_varg(*t, args);
}

void cdc_treap_dtor(struct cdc_treap *t)
{
        assert(t != NULL);

        free_treap(t, t->root);
        cdc_di_shared_dtor(t->dinfo);
        free(t);
}

enum cdc_stat cdc_treap_get(struct cdc_treap *t, void *key, void **value)
{
        assert(t != NULL);

        struct cdc_treap_node *node = find_node(t->root, key, t->compar);

        if (node)
                *value = node->value;

        return node ? CDC_STATUS_OK : CDC_STATUS_NOT_FOUND;
}

size_t cdc_treap_count(struct cdc_treap *t, void *key)
{
        assert(t != NULL);

        return (size_t)(find_node(t->root, key, t->compar) != NULL);
}

void cdc_treap_find(struct cdc_treap *t, void *key, struct cdc_treap_iter *it)
{
        assert(t != NULL);
        assert(it != NULL);

        struct cdc_treap_node *node = find_node(t->root, key, t->compar);

        if (!node) {
                cdc_treap_end(t, it);
                return;
        }

        it->container = t;
        it->current = node;
        it->prev = predecessor(node);
}

void cdc_treap_equal_range(struct cdc_treap *t, void *key,
                           struct cdc_pair_treap_iter *pair)
{
        assert(t != NULL);
        assert(pair != NULL);

        cdc_treap_find(t, key, &pair->first);
        cdc_treap_end(t, &pair->second);

        if (cdc_treap_iter_is_eq(&pair->first, &pair->second)) {
                cdc_treap_end(t, &pair->first);
        } else {
                pair->second = pair->first;
                cdc_treap_iter_next(&pair->second);
        }
}

enum cdc_stat cdc_treap_insert(struct cdc_treap *t, void *key, void *value,
                               struct cdc_pair_treap_iter_bool *ret)
{
        assert(t != NULL);

        struct cdc_treap_node *node = find_node(t->root, key, t->compar);
        bool finded = node;

        if (!node) {
                node = make_and_insert_unique(t, key, value);
                if (!node)
                        return CDC_STATUS_BAD_ALLOC;
        }

        if (ret) {
                (*ret).first.container = t;
                (*ret).first.current = node;
                (*ret).first.prev = predecessor(node);
                (*ret).second = !finded;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_treap_insert_or_assign(struct cdc_treap *t, void *key, void *value,
                                         struct cdc_pair_treap_iter_bool *ret)
{
        assert(t != NULL);

        struct cdc_treap_node *node = find_node(t->root, key, t->compar);
        bool finded = node;

        if (!node) {
                node = make_and_insert_unique(t, key, value);
                if (!node)
                        return CDC_STATUS_BAD_ALLOC;
        } else {
                node->value = value;
        }

        if (ret) {
                (*ret).first.container = t;
                (*ret).first.current = node;
                (*ret).first.prev = predecessor(node);
                (*ret).second = !finded;
        }

        return CDC_STATUS_OK;
}

size_t cdc_treap_erase(struct cdc_treap *t, void *key)
{
        assert(t != NULL);

        struct cdc_treap_node *node = find_node(t->root, key, t->compar);

        if (node == NULL)
                return 0;

        erase_node(t, node);
        return 1;
}

void cdc_treap_clear(struct cdc_treap *t)
{
        assert(t != NULL);

        free_treap(t, t->root);
        t->size = 0;
        t->root = NULL;
}

void cdc_treap_swap(struct cdc_treap *a, struct cdc_treap *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct cdc_treap_node *, a->root, b->root);
        CDC_SWAP(size_t, a->size, b->size);
        CDC_SWAP(cdc_priority_fn_t, a->prior, b->prior);
        CDC_SWAP(cdc_binary_pred_fn_t, a->compar, b->compar);
        CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}

void cdc_treap_begin(struct cdc_treap *t, struct cdc_treap_iter *it)
{
        assert(t != NULL);
        assert(it != NULL);

        it->container = t;
        it->current = min_node(t->root);
        it->prev = NULL;
}

void cdc_treap_end(struct cdc_treap *t, struct cdc_treap_iter *it)
{
        assert(t != NULL);
        assert(it != NULL);

        it->container = t;
        it->current = NULL;
        it->prev = max_node(t->root);
}

void cdc_treap_iter_next(struct cdc_treap_iter *it)
{
        assert(it != NULL);

        it->prev = it->current;
        it->current = successor(it->current);
}

void cdc_treap_iter_prev(struct cdc_treap_iter *it)
{
        assert(it != NULL);

        it->current = it->prev;
        it->prev = predecessor(it->current);
}

