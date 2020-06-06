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
#include "cdcontainers/treap.h"

#include "cdcontainers/data-info.h"
#include "cdcontainers/global.h"
#include "cdcontainers/tree-utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct node_pair {
  treap_node_t *left;
  treap_node_t *right;
};

CDC_MAKE_FIND_NODE_FN(treap_node_t *)
CDC_MAKE_MIN_NODE_FN(treap_node_t *)
CDC_MAKE_MAX_NODE_FN(treap_node_t *)
CDC_MAKE_SUCCESSOR_FN(treap_node_t *)
CDC_MAKE_PREDECESSOR_FN(treap_node_t *)

static int default_prior(void *value)
{
  CDC_UNUSED(value);

  return rand();
}

static treap_node_t *make_new_node(void *key, int prior, void *val)
{
  treap_node_t *node = (treap_node_t *)malloc(sizeof(treap_node_t));
  if (!node) return NULL;

  node->priority = prior;
  node->key = key;
  node->value = val;
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  return node;
}

static void free_node(treap_t *t, treap_node_t *node)
{
  if (CDC_HAS_DFREE(t->dinfo)) {
    pair_t pair = {node->key, node->value};
    t->dinfo->dfree(&pair);
  }

  free(node);
}

static void free_treap(treap_t *t, treap_node_t *root)
{
  if (root == NULL) {
    return;
  }

  free_treap(t, root->left);
  free_treap(t, root->right);
  free_node(t, root);
}

static struct node_pair split(treap_node_t *root, void *key, cdc_binary_pred_fn_t compar)
{
  struct node_pair pair;
  if (root == NULL) {
    pair.left = NULL;
    pair.right = NULL;
    return pair;
  }

  if (compar(root->key, key)) {
    pair = split(root->right, key, compar);
    root->right = pair.left;
    if (pair.left) {
      pair.left->parent = root;
    }

    if (pair.right) {
      pair.right->parent = NULL;
    }

    pair.left = root;
    pair.right = pair.right;
    return pair;
  } else {
    pair = split(root->left, key, compar);
    root->left = pair.right;
    if (pair.left) {
      pair.left->parent = NULL;
    }

    if (pair.right) {
      pair.right->parent = root;
    }

    pair.left = pair.left;
    pair.right = root;
    return pair;
  }
}

static treap_node_t *merge(treap_node_t *l, treap_node_t *r)
{
  if (l == NULL) {
    return r;
  }

  if (r == NULL) {
    return l;
  }

  if (l->priority > r->priority) {
    l->right = merge(l->right, r);
    if (l->right) {
      l->right->parent = l;
    }

    return l;
  } else {
    r->left = merge(l, r->left);
    if (r->left) {
      r->left->parent = r;
    }

    return r;
  }
}

static treap_node_t *find_nearest(treap_t *t, void *key, int priority)
{
  treap_node_t *node = t->root;
  while (node) {
    if (node->priority <= priority) {
      break;
    }

    if (t->dinfo->cmp(key, node->key)) {
      if (node->left) {
        node = node->left;
      } else {
        break;
      }
    } else {
      if (node->right) {
        node = node->right;
      } else {
        break;
      }
    }
  }

  return node;
}

static void erase_node(treap_t *t, treap_node_t *node)
{
  treap_node_t *tmp = merge(node->left, node->right);
  if (t->root == node) {
    if (tmp) {
      tmp->parent = NULL;
    }
    t->root = tmp;
  } else {
    if (tmp) {
      tmp->parent = node->parent;
    }

    if (node->parent->left && node->parent->left == node) {
      node->parent->left = tmp;
    } else {
      node->parent->right = tmp;
    }
  }

  --t->size;
  free_node(t, node);
}

static treap_node_t *insert_unique(treap_t *t, treap_node_t *node, treap_node_t *nearest)
{
  if (t->root) {
    if (nearest->priority > node->priority) {
      if (t->dinfo->cmp(node->key, nearest->key)) {
        nearest->left = node;
      } else {
        nearest->right = node;
      }

      node->parent = nearest;
    } else {
      treap_node_t *pnode = nearest->parent;
      struct node_pair pair = split(nearest, node->key, t->dinfo->cmp);
      node->left = pair.left;
      if (pair.left) {
        pair.left->parent = node;
      }

      node->right = pair.right;
      if (pair.right) {
        pair.right->parent = node;
      }

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
  } else {
    t->root = node;
  }

  ++t->size;
  return node;
}

static stat_t init_varg(treap_t *t, va_list args)
{
  pair_t *pair = NULL;
  while ((pair = va_arg(args, pair_t *)) != CDC_END) {
    stat_t stat = treap_insert(t, pair->first, pair->second, NULL);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  return CDC_STATUS_OK;
}

stat_t treap_ctor1(treap_t **t, data_info_t *info, cdc_priority_fn_t prior)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  treap_t *tmp = (treap_t *)calloc(sizeof(treap_t), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (info && !(tmp->dinfo = di_shared_ctorc(info))) {
    free(tmp);
    return CDC_STATUS_BAD_ALLOC;
  }

  tmp->prior = prior ? prior : default_prior;
  *t = tmp;
  return CDC_STATUS_OK;
}

stat_t treap_ctorl1(treap_t **t, data_info_t *info, cdc_priority_fn_t prior, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, prior);
  stat_t stat = treap_ctorv1(t, info, prior, args);
  va_end(args);
  return stat;
}

stat_t treap_ctorv1(treap_t **t, data_info_t *info, cdc_priority_fn_t prior, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  stat_t stat = treap_ctor1(t, info, prior);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  return init_varg(*t, args);
}

stat_t treap_ctor(treap_t **t, data_info_t *info)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  return treap_ctor1(t, info, NULL);
}

stat_t treap_ctorl(treap_t **t, data_info_t *info, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  stat_t stat = treap_ctorv(t, info, args);
  va_end(args);
  return stat;
}

stat_t treap_ctorv(treap_t **t, data_info_t *info, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  return treap_ctorv1(t, info, NULL, args);
}

void treap_dtor(treap_t *t)
{
  assert(t != NULL);

  free_treap(t, t->root);
  di_shared_dtor(t->dinfo);
  free(t);
}

stat_t treap_get(treap_t *t, void *key, void **value)
{
  assert(t != NULL);

  treap_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (node) {
    *value = node->value;
    return CDC_STATUS_OK;
  }

  return CDC_STATUS_NOT_FOUND;
}

size_t treap_count(treap_t *t, void *key)
{
  assert(t != NULL);

  return (size_t)(cdc_find_tree_node(t->root, key, t->dinfo->cmp) != NULL);
}

void treap_find(treap_t *t, void *key, treap_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  treap_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (!node) {
    treap_end(t, it);
    return;
  }

  it->container = t;
  it->current = node;
  it->prev = cdc_tree_predecessor(node);
}

stat_t treap_insert(treap_t *t, void *key, void *value, pair_treap_iter_bool_t *ret)
{
  assert(t != NULL);

  treap_iter_t *it = NULL;
  bool *inserted = NULL;
  if (ret) {
    it = &ret->first;
    inserted = &ret->second;
  }

  return treap_insert1(t, key, value, it, inserted);
}

stat_t treap_insert1(treap_t *t, void *key, void *value, treap_iter_t *it, bool *inserted)
{
  assert(t != NULL);

  treap_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  bool finded = node;
  if (!node) {
    node = make_new_node(key, t->prior(value), value);
    if (!node) {
      return CDC_STATUS_BAD_ALLOC;
    }

    treap_node_t *nearest = find_nearest(t, node->key, node->priority);
    node = insert_unique(t, node, nearest);
  }

  if (it) {
    it->container = t;
    it->current = node;
    it->prev = cdc_tree_predecessor(node);
  }

  if (inserted) {
    *inserted = !finded;
  }

  return CDC_STATUS_OK;
}

stat_t treap_insert_or_assign(treap_t *t, void *key, void *value, pair_treap_iter_bool_t *ret)
{
  assert(t != NULL);

  treap_iter_t *it = NULL;
  bool *inserted = NULL;
  if (ret) {
    it = &ret->first;
    inserted = &ret->second;
  }

  return treap_insert_or_assign1(t, key, value, it, inserted);
}

stat_t treap_insert_or_assign1(treap_t *t, void *key, void *value, treap_iter_t *it, bool *inserted)
{
  assert(t != NULL);

  treap_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  bool finded = node;
  if (!node) {
    node = make_new_node(key, t->prior(value), value);
    if (!node) {
      return CDC_STATUS_BAD_ALLOC;
    }

    treap_node_t *nearest = find_nearest(t, node->key, node->priority);
    node = insert_unique(t, node, nearest);
  } else {
    node->value = value;
  }

  if (it) {
    it->container = t;
    it->current = node;
    it->prev = cdc_tree_predecessor(node);
  }

  if (inserted) {
    *inserted = !finded;
  }

  return CDC_STATUS_OK;
}

size_t treap_erase(treap_t *t, void *key)
{
  assert(t != NULL);

  treap_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (node == NULL) {
    return 0;
  }

  erase_node(t, node);
  return 1;
}

void treap_clear(treap_t *t)
{
  assert(t != NULL);

  free_treap(t, t->root);
  t->size = 0;
  t->root = NULL;
}

void treap_swap(treap_t *a, treap_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(treap_node_t *, a->root, b->root);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(cdc_priority_fn_t, a->prior, b->prior);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}

void treap_begin(treap_t *t, treap_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = cdc_min_tree_node(t->root);
  it->prev = NULL;
}

void treap_end(treap_t *t, treap_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = NULL;
  it->prev = cdc_max_tree_node(t->root);
}

void treap_iter_next(treap_iter_t *it)
{
  assert(it != NULL);

  it->prev = it->current;
  it->current = cdc_tree_successor(it->current);
}

void treap_iter_prev(treap_iter_t *it)
{
  assert(it != NULL);

  it->current = it->prev;
  it->prev = cdc_tree_predecessor(it->current);
}
