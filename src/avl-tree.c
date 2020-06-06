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
#include "cdcontainers/avl-tree.h"

#include "cdcontainers/data-info.h"
#include "cdcontainers/tree-utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

CDC_MAKE_FIND_NODE_FN(avl_tree_node_t *)
CDC_MAKE_MIN_NODE_FN(avl_tree_node_t *)
CDC_MAKE_MAX_NODE_FN(avl_tree_node_t *)
CDC_MAKE_SUCCESSOR_FN(avl_tree_node_t *)
CDC_MAKE_PREDECESSOR_FN(avl_tree_node_t *)

static avl_tree_node_t *make_new_node(void *key, void *val)
{
  avl_tree_node_t *node = (avl_tree_node_t *)malloc(sizeof(avl_tree_node_t));
  if (!node) {
    return NULL;
  }

  node->key = key;
  node->value = val;
  node->height = 1;
  node->parent = NULL;
  node->left = NULL;
  node->right = NULL;
  return node;
}

static void free_node(avl_tree_t *t, avl_tree_node_t *node)
{
  assert(t != NULL);

  if (CDC_HAS_DFREE(t->dinfo)) {
    pair_t pair = {node->key, node->value};
    t->dinfo->dfree(&pair);
  }

  free(node);
}

static void free_avl_tree(avl_tree_t *t, avl_tree_node_t *root)
{
  assert(t != NULL);

  if (root == NULL) {
    return;
  }

  free_avl_tree(t, root->left);
  free_avl_tree(t, root->right);
  free_node(t, root);
}

static unsigned char height(avl_tree_node_t *node)
{
  return node ? node->height : 0;
}

static int height_diff(avl_tree_node_t *node)
{
  return height(node->right) - height(node->left);
}

static void update_height(avl_tree_node_t *node)
{
  unsigned char lhs = height(node->left);
  unsigned char rhs = height(node->right);
  node->height = CDC_MAX(lhs, rhs) + 1;
}

static void update_link(avl_tree_node_t *parent, avl_tree_node_t *old_child,
                        avl_tree_node_t *new_child)
{
  new_child->parent = parent;
  if (!parent) {
    return;
  }

  if (parent->left == old_child) {
    parent->left = new_child;
  } else {
    parent->right = new_child;
  }
}

static void unlink_list(avl_tree_node_t *parent, avl_tree_node_t *child)
{
  if (!parent) {
    return;
  }

  if (parent->left == child) {
    parent->left = NULL;
  } else {
    parent->right = NULL;
  }
}

static avl_tree_node_t *rotate_right(avl_tree_node_t *node)
{
  avl_tree_node_t *q = node->left;
  update_link(node->parent, node, q);
  node->left = q->right;
  if (node->left) {
    node->left->parent = node;
  }

  q->right = node;
  if (q->right) {
    q->right->parent = q;
  }

  update_height(node);
  update_height(q);
  return q;
}

static avl_tree_node_t *rotate_left(avl_tree_node_t *node)
{
  avl_tree_node_t *q = node->right;
  update_link(node->parent, node, q);
  node->right = q->left;
  if (node->right) {
    node->right->parent = node;
  }

  q->left = node;
  if (q->left) {
    q->left->parent = q;
  }

  update_height(node);
  update_height(q);
  return q;
}

static avl_tree_node_t *balance(avl_tree_node_t *node)
{
  while (node) {
    update_height(node);
    int diff = height_diff(node);
    if (diff == 2) {
      if (height_diff(node->right) < 0) {
        node->right = rotate_right(node->right);
      }
      node = rotate_left(node);
    } else if (diff == -2) {
      if (height_diff(node->left) > 0) {
        node->left = rotate_left(node->left);
      }
      node = rotate_right(node);
    } else {
      if (node->parent) {
        node = node->parent;
      } else {
        break;
      }
    }
  }

  return node;
}

static avl_tree_node_t *insert_unique(avl_tree_t *t, avl_tree_node_t *node,
                                      avl_tree_node_t *nearest)
{
  if (t->root != NULL) {
    if (t->dinfo->cmp(node->key, nearest->key)) {
      nearest->left = node;
    } else {
      nearest->right = node;
    }

    node->parent = nearest;
    t->root = balance(nearest);
  } else {
    t->root = node;
  }

  ++t->size;
  return node;
}

static avl_tree_node_t *find_hint(avl_tree_node_t *node, void *key, cdc_binary_pred_fn_t compar)
{
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

  return node;
}

static avl_tree_node_t *erase_node(avl_tree_t *t, avl_tree_node_t *node)
{
  avl_tree_node_t *parent = NULL;
  if (node->left == NULL && node->right == NULL) {
    parent = node->parent;
    unlink_list(parent, node);
  } else if (node->right == NULL) {
    parent = node->parent;
    if (parent) {
      if (parent->left == node) {
        parent->left = node->left;
        parent->left->parent = parent;
      } else {
        parent->right = node->left;
        parent->right->parent = parent;
      }
    } else {
      parent = node->left;
      parent->parent = NULL;
    }
  } else if (node->left == NULL) {
    parent = node->parent;
    if (parent) {
      if (parent->right == node) {
        parent->right = node->right;
        parent->right->parent = parent;
      } else {
        parent->left = node->right;
        parent->left->parent = parent;
      }
    } else {
      parent = node->right;
      parent->parent = NULL;
    }
  } else {
    avl_tree_node_t *mnode = cdc_min_tree_node(node->right);
    CDC_SWAP(void *, node->value, mnode->value);
    CDC_SWAP(void *, node->key, mnode->key);
    parent = mnode->parent;
    if (parent == node) {
      parent->right = NULL;
    } else if (parent) {
      parent->left = mnode->right;
      if (mnode->right) {
        mnode->right->parent = parent;
      }
    }

    node = mnode;
  }

  free_node(t, node);
  return balance(parent);
}

static stat_t init_varg(avl_tree_t *t, va_list args)
{
  pair_t *pair = NULL;
  while ((pair = va_arg(args, pair_t *)) != CDC_END) {
    stat_t stat = avl_tree_insert(t, pair->first, pair->second, NULL);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  return CDC_STATUS_OK;
}

stat_t avl_tree_ctor(avl_tree_t **t, data_info_t *info)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  avl_tree_t *tmp = (avl_tree_t *)calloc(sizeof(avl_tree_t), 1);
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

stat_t avl_tree_ctorl(avl_tree_t **t, data_info_t *info, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  stat_t stat = avl_tree_ctorv(t, info, args);
  va_end(args);
  return stat;
}

stat_t avl_tree_ctorv(avl_tree_t **t, data_info_t *info, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  stat_t stat = avl_tree_ctor(t, info);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  return init_varg(*t, args);
}

void avl_tree_dtor(avl_tree_t *t)
{
  assert(t != NULL);

  free_avl_tree(t, t->root);
  di_shared_dtor(t->dinfo);
  free(t);
}

stat_t avl_tree_get(avl_tree_t *t, void *key, void **value)
{
  assert(t != NULL);

  avl_tree_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (node) {
    *value = node->value;
    return CDC_STATUS_OK;
  }

  return CDC_STATUS_NOT_FOUND;
}

size_t avl_tree_count(avl_tree_t *t, void *key)
{
  assert(t != NULL);

  return (size_t)(cdc_find_tree_node(t->root, key, t->dinfo->cmp) != NULL);
}

void avl_tree_find(avl_tree_t *t, void *key, avl_tree_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  avl_tree_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (!node) {
    avl_tree_end(t, it);
    return;
  }

  it->container = t;
  it->current = node;
  it->prev = cdc_tree_predecessor(node);
}

stat_t avl_tree_insert(avl_tree_t *t, void *key, void *value, pair_avl_tree_iter_bool_t *ret)
{
  assert(t != NULL);

  avl_tree_iter_t *it = NULL;
  bool *inserted = NULL;
  if (ret) {
    it = &ret->first;
    inserted = &ret->second;
  }

  return avl_tree_insert1(t, key, value, it, inserted);
}

stat_t avl_tree_insert1(avl_tree_t *t, void *key, void *value, avl_tree_iter_t *it, bool *inserted)
{
  assert(t != NULL);

  avl_tree_node_t *node = find_hint(t->root, key, t->dinfo->cmp);
  bool finded = node && cdc_eq(t->dinfo->cmp, node->key, key);
  if (!finded) {
    avl_tree_node_t *new_node = make_new_node(key, value);
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

stat_t avl_tree_insert_or_assign(avl_tree_t *t, void *key, void *value,
                                 pair_avl_tree_iter_bool_t *ret)
{
  assert(t != NULL);

  avl_tree_iter_t *it = NULL;
  bool *inserted = NULL;
  if (ret) {
    it = &ret->first;
    inserted = &ret->second;
  }

  return avl_tree_insert_or_assign1(t, key, value, it, inserted);
}

stat_t avl_tree_insert_or_assign1(avl_tree_t *t, void *key, void *value, avl_tree_iter_t *it,
                                  bool *inserted)
{
  assert(t != NULL);

  avl_tree_node_t *node = find_hint(t->root, key, t->dinfo->cmp);
  bool finded = node && cdc_eq(t->dinfo->cmp, node->key, key);
  if (!finded) {
    avl_tree_node_t *new_node = make_new_node(key, value);
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

size_t avl_tree_erase(avl_tree_t *t, void *key)
{
  assert(t != NULL);

  avl_tree_node_t *node = cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (!node) {
    return 0;
  }

  t->root = erase_node(t, node);
  --t->size;
  return 1;
}

void avl_tree_clear(avl_tree_t *t)
{
  assert(t != NULL);

  free_avl_tree(t, t->root);
  t->size = 0;
  t->root = NULL;
}

void avl_tree_swap(avl_tree_t *a, avl_tree_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(avl_tree_node_t *, a->root, b->root);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}

void avl_tree_begin(avl_tree_t *t, avl_tree_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = cdc_min_tree_node(t->root);
  it->prev = NULL;
}

void avl_tree_end(avl_tree_t *t, avl_tree_iter_t *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = NULL;
  it->prev = cdc_max_tree_node(t->root);
}

void avl_tree_iter_next(avl_tree_iter_t *it)
{
  assert(it != NULL);

  it->prev = it->current;
  it->current = cdc_tree_successor(it->current);
}

void avl_tree_iter_prev(avl_tree_iter_t *it)
{
  assert(it != NULL);

  it->current = it->prev;
  it->prev = cdc_tree_predecessor(it->current);
}
