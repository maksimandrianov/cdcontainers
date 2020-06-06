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
#include "cdcontainers/splay-tree.h"

#include "cdcontainers/data-info.h"
#include "cdcontainers/tree-utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct node_pair {
  splay_tree_node_t *left;
  splay_tree_node_t *right;
};

CDC_MAKE_FIND_NODE_FN(splay_tree_node_t *)
CDC_MAKE_MIN_NODE_FN(splay_tree_node_t *)
CDC_MAKE_MAX_NODE_FN(splay_tree_node_t *)
CDC_MAKE_SUCCESSOR_FN(splay_tree_node_t *)
CDC_MAKE_PREDECESSOR_FN(splay_tree_node_t *)

static splay_tree_node_t *make_new_node(void *key, void *val)
{
  splay_tree_node_t *node = (splay_tree_node_t *)malloc(sizeof(splay_tree_node_t));
  if (node) {
    node->key = key;
    node->value = val;
    node->parent = node->left = node->right = NULL;
  }

  return node;
}

static void free_node(splay_tree_t *t, splay_tree_node_t *node)
{
  assert(t != NULL);

  if (CDC_HAS_DFREE(t->dinfo)) {
    pair_t pair = {node->key, node->value};
    t->dinfo->dfree(&pair);
  }

  free(node);
}

static void free_splay_tree(splay_tree_t *t, splay_tree_node_t *root)
{
  assert(t != NULL);

  if (root == NULL) {
    return;
  }

  free_splay_tree(t, root->left);
  free_splay_tree(t, root->right);
  free_node(t, root);
}

static void update_link(splay_tree_node_t *parent, splay_tree_node_t *old, splay_tree_node_t *node)
{
  if (!parent) {
    return;
  }

  if (parent->left == old) {
    parent->left = node;
  } else {
    parent->right = node;
  }
}

static splay_tree_node_t *zig_right(splay_tree_node_t *node)
{
  splay_tree_node_t *p = node->parent;
  update_link(p->parent, p, node);
  node->parent = p->parent;
  p->left = node->right;
  if (p->left) {
    p->left->parent = p;
  }

  node->right = p;
  if (node->right) {
    node->right->parent = node;
  }

  return node;
}

static splay_tree_node_t *zig_left(splay_tree_node_t *node)
{
  splay_tree_node_t *p = node->parent;
  update_link(p->parent, p, node);
  node->parent = p->parent;
  p->right = node->left;
  if (p->right) {
    p->right->parent = p;
  }

  node->left = p;
  if (node->left) {
    node->left->parent = node;
  }

  return node;
}

static splay_tree_node_t *zigzig_right(splay_tree_node_t *node)
{
  node = zig_right(node->parent);
  return zig_right(node->left);
}

static splay_tree_node_t *zigzig_left(splay_tree_node_t *node)
{
  node = zig_left(node->parent);
  return zig_left(node->right);
}

static splay_tree_node_t *zigzag_right(splay_tree_node_t *node)
{
  node = zig_left(node);
  return zig_right(node);
}

static splay_tree_node_t *zigzag_left(splay_tree_node_t *node)
{
  node = zig_right(node);
  return zig_left(node);
}

static splay_tree_node_t *splay(splay_tree_node_t *node)
{
  while (node->parent) {
    if (node->parent->parent == NULL) {
      if (node == node->parent->left) {
        node = zig_right(node);
      } else {
        node = zig_left(node);
      }
    } else if (node == node->parent->left) {
      if (node->parent == node->parent->parent->left) {
        node = zigzig_right(node);
      } else {
        node = zigzag_left(node);
      }
    } else {
      if (node->parent == node->parent->parent->right) {
        node = zigzig_left(node);
      } else {
        node = zigzag_right(node);
      }
    }
  }

  return node;
}

static splay_tree_node_t *find_hint(splay_tree_node_t *node, void *key, cdc_binary_pred_fn_t compar)
{
  splay_tree_node_t *tmp = node;
  while (node) {
    if (compar(key, node->key)) {
      if (node->left) {
        node = node->left;
      } else {
        break;
      }
    } else if (compar(node->key, key)) {
      if (node->right) {
        node = node->right;
      } else {
        break;
      }
    } else {
      break;
    }
  }

  return node ? node : cdc_max_tree_node(tmp);
}

static struct node_pair split(splay_tree_node_t *node, void *key, cdc_binary_pred_fn_t compar)
{
  struct node_pair ret;
  node = splay(node);
  if (compar(key, node->key)) {
    ret.left = node->left;
    ret.right = node;
    node->left = NULL;
    if (ret.left) {
      ret.left->parent = NULL;
    }
  } else {
    ret.left = node;
    ret.right = node->right;
    node->right = NULL;
    if (ret.right) {
      ret.right->parent = NULL;
    }
  }

  return ret;
}

static splay_tree_node_t *merge(splay_tree_node_t *a, splay_tree_node_t *b)
{
  if (!a) {
    return b;
  }

  if (!b) {
    return a;
  }

  a = cdc_max_tree_node(a);
  a = splay(a);
  a->right = b;
  b->parent = a;
  return a;
}

static splay_tree_node_t *sfind(splay_tree_t *t, void *key)
{
  splay_tree_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);

  if (!node) {
    return node;
  }

  node = splay(node);
  t->root = node;
  return node;
}

static splay_tree_node_t *insert_unique(splay_tree_t *t, splay_tree_node_t *node,
                                        splay_tree_node_t *nearest)
{
  struct node_pair pair;
  if (t->root == NULL) {
    t->root = node;
  } else {
    pair = split(nearest, node->key, t->dinfo->cmp);
    node->left = pair.left;
    if (node->left) {
      node->left->parent = node;
    }

    node->right = pair.right;
    if (node->right) {
      node->right->parent = node;
    }
    t->root = node;
  }

  ++t->size;
  return node;
}

static stat_t init_varg(splay_tree_t *t, va_list args)
{
  pair_t *pair = NULL;
  while ((pair = va_arg(args, pair_t *)) != CDC_END) {
    stat_t stat = splay_tree_insert(t, pair->first, pair->second, NULL);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  return CDC_STATUS_OK;
}

stat_t splay_tree_ctor(splay_tree_t **t, data_info_t *info)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  splay_tree_t *tmp = (splay_tree_t *)calloc(sizeof(splay_tree_t), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (info && !(tmp->dinfo = di_shared_ctorc(info))) {
    free(tmp);
    return CDC_STATUS_BAD_ALLOC;
  }

  *t = tmp;
  return CDC_STATUS_OK;
}

stat_t splay_tree_ctorl(splay_tree_t **t, data_info_t *info, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  stat_t stat = splay_tree_ctorv(t, info, args);
  va_end(args);
  return stat;
}

stat_t splay_tree_ctorv(splay_tree_t **t, data_info_t *info, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  stat_t stat = splay_tree_ctor(t, info);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  return init_varg(*t, args);
}

void splay_tree_dtor(splay_tree_t *t)
{
  assert(t != NULL);

  free_splay_tree(t, t->root);
  di_shared_dtor(t->dinfo);
  free(t);
}

stat_t splay_tree_get(splay_tree_t *t, void *key, void **value)
{
  assert(t != NULL);

  splay_tree_node_t *node = sfind(t, key);
  if (node) {
    *value = node->value;
  }

  return node ? CDC_STATUS_OK : CDC_STATUS_NOT_FOUND;
}

size_t splay_tree_count(splay_tree_t *t, void *key)
{
  assert(t != NULL);

  return (size_t)(sfind(t, key) != NULL);
}

void splay_tree_find(splay_tree_t *t, void *key, splay_tree_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  splay_tree_node_t *node = sfind(t, key);
  if (!node) {
    splay_tree_end(t, it);
    return;
  }

  it->container = t;
  it->current = node;
  it->prev = cdc_tree_predecessor(node);
}

stat_t splay_tree_insert(splay_tree_t *t, void *key, void *value, pair_splay_tree_iter_bool_t *ret)
{
  assert(t != NULL);

  splay_tree_iter_t *it = NULL;
  bool *inserted = NULL;
  if (ret) {
    it = &ret->first;
    inserted = &ret->second;
  }

  return splay_tree_insert1(t, key, value, it, inserted);
}

stat_t splay_tree_insert1(splay_tree_t *t, void *key, void *value, splay_tree_iter_t *it,
                          bool *inserted)
{
  assert(t != NULL);

  splay_tree_node_t *node = find_hint(t->root, key, t->dinfo->cmp);
  bool finded = node && cdc_eq(t->dinfo->cmp, node->key, key);
  if (!finded) {
    splay_tree_node_t *new_node = make_new_node(key, value);
    if (!new_node) {
      return CDC_STATUS_BAD_ALLOC;
    }

    node = insert_unique(t, new_node, node);
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

stat_t splay_tree_insert_or_assign(splay_tree_t *t, void *key, void *value,
                                   pair_splay_tree_iter_bool_t *ret)
{
  assert(t != NULL);

  splay_tree_iter_t *it = NULL;
  bool *inserted = NULL;
  if (ret) {
    it = &ret->first;
    inserted = &ret->second;
  }

  return splay_tree_insert_or_assign1(t, key, value, it, inserted);
}

stat_t splay_tree_insert_or_assign1(splay_tree_t *t, void *key, void *value, splay_tree_iter_t *it,
                                    bool *inserted)
{
  assert(t != NULL);

  splay_tree_node_t *node = find_hint(t->root, key, t->dinfo->cmp);
  bool finded = node && cdc_eq(t->dinfo->cmp, node->key, key);
  if (!finded) {
    splay_tree_node_t *new_node = make_new_node(key, value);
    if (!new_node) {
      return CDC_STATUS_BAD_ALLOC;
    }

    node = insert_unique(t, new_node, node);
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

size_t splay_tree_erase(splay_tree_t *t, void *key)
{
  assert(t != NULL);

  splay_tree_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (!node) {
    return 0;
  }

  node = splay(node);
  if (node->left) {
    node->left->parent = NULL;
  }

  if (node->right) {
    node->right->parent = NULL;
  }

  t->root = merge(node->left, node->right);
  free_node(t, node);
  --t->size;
  return 1;
}

void splay_tree_clear(splay_tree_t *t)
{
  assert(t != NULL);

  free_splay_tree(t, t->root);
  t->size = 0;
  t->root = NULL;
}

void splay_tree_swap(splay_tree_t *a, splay_tree_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(splay_tree_node_t *, a->root, b->root);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}

void splay_tree_begin(splay_tree_t *t, splay_tree_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = cdc_min_tree_node(t->root);
  it->prev = NULL;
}

void splay_tree_end(splay_tree_t *t, splay_tree_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = NULL;
  it->prev = cdc_max_tree_node(t->root);
}

void splay_tree_iter_next(splay_tree_iter_t *it)
{
  assert(it != NULL);

  it->prev = it->current;
  it->current = cdc_tree_successor(it->current);
}

void splay_tree_iter_prev(splay_tree_iter_t *it)
{
  assert(it != NULL);

  it->current = it->prev;
  it->prev = cdc_tree_predecessor(it->current);
}
