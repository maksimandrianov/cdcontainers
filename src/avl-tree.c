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
#include "cdcontainers/avl-tree.h"

#include "cdcontainers/data-info.h"
#include "cdcontainers/tree-utils.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

CDC_MAKE_FIND_NODE_FN(struct cdc_avl_tree_node *)
CDC_MAKE_MIN_NODE_FN(struct cdc_avl_tree_node *)
CDC_MAKE_MAX_NODE_FN(struct cdc_avl_tree_node *)
CDC_MAKE_SUCCESSOR_FN(struct cdc_avl_tree_node *)
CDC_MAKE_PREDECESSOR_FN(struct cdc_avl_tree_node *)

static struct cdc_avl_tree_node *new_node(void *key, void *val)
{
  struct cdc_avl_tree_node *node =
      (struct cdc_avl_tree_node *)malloc(sizeof(struct cdc_avl_tree_node));
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

static void free_node(struct cdc_avl_tree *t, struct cdc_avl_tree_node *node)
{
  assert(t != NULL);

  if (CDC_HAS_DFREE(t->dinfo)) {
    struct cdc_pair pair = {.first = node->key, .second = node->value};
    t->dinfo->dfree(&pair);
  }

  free(node);
}

static void free_avl_tree(struct cdc_avl_tree *t,
                          struct cdc_avl_tree_node *root)
{
  assert(t != NULL);

  if (root == NULL) {
    return;
  }

  free_avl_tree(t, root->left);
  free_avl_tree(t, root->right);
  free_node(t, root);
}

static unsigned char height(struct cdc_avl_tree_node *node)
{
  return node ? node->height : 0;
}

static int height_diff(struct cdc_avl_tree_node *node)
{
  return height(node->right) - height(node->left);
}

static void update_height(struct cdc_avl_tree_node *node)
{
  unsigned char hl = height(node->left), hr = height(node->right);
  node->height = CDC_MAX(hl, hr) + 1;
}

static void update_link(struct cdc_avl_tree_node *parent,
                        struct cdc_avl_tree_node *old,
                        struct cdc_avl_tree_node *node)
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

static void unlink_list(struct cdc_avl_tree_node *p,
                        struct cdc_avl_tree_node *node)
{
  if (!p) {
    return;
  }

  if (p->left == node) {
    p->left = NULL;
  } else {
    p->right = NULL;
  }
}

static struct cdc_avl_tree_node *rotate_right(struct cdc_avl_tree_node *node)
{
  struct cdc_avl_tree_node *q = node->left;
  update_link(node->parent, node, q);
  q->parent = node->parent;
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

static struct cdc_avl_tree_node *rotate_left(struct cdc_avl_tree_node *node)
{
  struct cdc_avl_tree_node *q = node->right;
  update_link(node->parent, node, q);
  q->parent = node->parent;
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

static struct cdc_avl_tree_node *balance(struct cdc_avl_tree_node *node)
{
  while (node) {
    update_height(node);
    if (height_diff(node) == 2) {
      if (height_diff(node->right) < 0) {
        node->right = rotate_right(node->right);
      }
      node = rotate_left(node);
    } else if (height_diff(node) == -2) {
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

static struct cdc_avl_tree_node *insert_unique(
    struct cdc_avl_tree *t, struct cdc_avl_tree_node *node,
    struct cdc_avl_tree_node *nearest)
{
  if (t->root == NULL) {
    t->root = node;
  } else {
    if (t->dinfo->cmp(node->key, nearest->key)) {
      nearest->left = node;
    } else {
      nearest->right = node;
    }

    node->parent = nearest;
    t->root = balance(nearest);
  }

  ++t->size;
  return node;
}

static struct cdc_avl_tree_node *find_hint(struct cdc_avl_tree_node *node,
                                           void *key,
                                           cdc_binary_pred_fn_t compar)
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

static struct cdc_avl_tree_node *make_and_insert_unique(
    struct cdc_avl_tree *t, void *key, void *value,
    struct cdc_avl_tree_node *nearest)
{
  struct cdc_avl_tree_node *node = new_node(key, value);
  if (!node) {
    return node;
  }

  return insert_unique(t, node, nearest);
}

static struct cdc_avl_tree_node *erase_node(struct cdc_avl_tree *t,
                                            struct cdc_avl_tree_node *node)
{
  struct cdc_avl_tree_node *mnode = NULL;
  struct cdc_avl_tree_node *parent = NULL;
  if (node->left == NULL && node->right == NULL) {
    parent = node->parent;
    unlink_list(parent, node);
  } else if (node->right == NULL) {
    parent = node->parent;
    if (parent->left == node) {
      parent->left = node->left;
      parent->left->parent = parent;
    } else {
      parent->right = node->left;
      parent->right->parent = parent;
    }
  } else {
    mnode = cdc_min_tree_node(node->right);
    parent = mnode->parent;
    unlink_list(parent, mnode);
    if (parent == node) {
      parent = node->parent;
    }

    update_link(parent, node, mnode);
    mnode->left = node->left;
    mnode->right = node->right;
    mnode->parent = node->parent;
    if (mnode->left) {
      mnode->left->parent = mnode;
    }

    if (mnode->right) {
      mnode->right->parent = mnode;
    }
  }

  free_node(t, node);
  return balance(parent ? parent : mnode);
}

static enum cdc_stat init_varg(struct cdc_avl_tree *t, va_list args)
{
  struct cdc_pair *pair = NULL;
  while ((pair = va_arg(args, struct cdc_pair *)) != CDC_END) {
    enum cdc_stat stat =
        cdc_avl_tree_insert(t, pair->first, pair->second, NULL);
    if (stat != CDC_STATUS_OK) {
      return stat;
    }
  }

  return CDC_STATUS_OK;
}

enum cdc_stat cdc_avl_tree_ctor(struct cdc_avl_tree **t,
                                struct cdc_data_info *info)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  struct cdc_avl_tree *tmp =
      (struct cdc_avl_tree *)calloc(sizeof(struct cdc_avl_tree), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
    free(tmp);
    return CDC_STATUS_BAD_ALLOC;
  }

  *t = tmp;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_avl_tree_ctorl(struct cdc_avl_tree **t,
                                 struct cdc_data_info *info, ...)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  enum cdc_stat stat = cdc_avl_tree_ctorv(t, info, args);
  va_end(args);
  return stat;
}

enum cdc_stat cdc_avl_tree_ctorv(struct cdc_avl_tree **t,
                                 struct cdc_data_info *info, va_list args)
{
  assert(t != NULL);
  assert(CDC_HAS_CMP(info));

  enum cdc_stat stat = cdc_avl_tree_ctor(t, info);
  if (stat != CDC_STATUS_OK) {
    return stat;
  }

  return init_varg(*t, args);
}

void cdc_avl_tree_dtor(struct cdc_avl_tree *t)
{
  assert(t != NULL);

  free_avl_tree(t, t->root);
  cdc_di_shared_dtor(t->dinfo);
  free(t);
}

enum cdc_stat cdc_avl_tree_get(struct cdc_avl_tree *t, void *key, void **value)
{
  assert(t != NULL);

  struct cdc_avl_tree_node *node =
      cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (node) {
    *value = node->value;
    return CDC_STATUS_OK;
  }

  return CDC_STATUS_NOT_FOUND;
}

size_t cdc_avl_tree_count(struct cdc_avl_tree *t, void *key)
{
  assert(t != NULL);

  return (size_t)(cdc_find_tree_node(t->root, key, t->dinfo->cmp) != NULL);
}

void cdc_avl_tree_find(struct cdc_avl_tree *t, void *key,
                       struct cdc_avl_tree_iter *it)
{
  assert(t != NULL);
  assert(it != NULL);

  struct cdc_avl_tree_node *node =
      cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (!node) {
    cdc_avl_tree_end(t, it);
    return;
  }

  it->container = t;
  it->current = node;
  it->prev = cdc_tree_predecessor(node);
}

enum cdc_stat cdc_avl_tree_insert(struct cdc_avl_tree *t, void *key,
                                  void *value,
                                  struct cdc_pair_avl_tree_iter_bool *ret)
{
  assert(t != NULL);

  struct cdc_avl_tree_iter *it = NULL;
  bool *inserted = NULL;

  if (ret) {
    it = &ret->first;
    inserted = &ret->second;
  }

  return cdc_avl_tree_insert1(t, key, value, it, inserted);
}

enum cdc_stat cdc_avl_tree_insert1(struct cdc_avl_tree *t, void *key,
                                   void *value, struct cdc_avl_tree_iter *it,
                                   bool *inserted)
{
  assert(t != NULL);

  struct cdc_avl_tree_node *node = find_hint(t->root, key, t->dinfo->cmp);
  bool finded = node && cdc_eq(t->dinfo->cmp, node->key, key);
  if (!finded) {
    node = make_and_insert_unique(t, key, value, node);
    if (!node) {
      return CDC_STATUS_BAD_ALLOC;
    }
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

enum cdc_stat cdc_avl_tree_insert_or_assign(
    struct cdc_avl_tree *t, void *key, void *value,
    struct cdc_pair_avl_tree_iter_bool *ret)
{
  assert(t != NULL);

  struct cdc_avl_tree_iter *it = NULL;
  bool *inserted = NULL;

  if (ret) {
    it = &ret->first;
    inserted = &ret->second;
  }

  return cdc_avl_tree_insert_or_assign1(t, key, value, it, inserted);
}

enum cdc_stat cdc_avl_tree_insert_or_assign1(struct cdc_avl_tree *t, void *key,
                                             void *value,
                                             struct cdc_avl_tree_iter *it,
                                             bool *inserted)
{
  assert(t != NULL);

  struct cdc_avl_tree_node *node = find_hint(t->root, key, t->dinfo->cmp);
  bool finded = node && cdc_eq(t->dinfo->cmp, node->key, key);
  if (!finded) {
    node = make_and_insert_unique(t, key, value, node);
    if (!node) {
      return CDC_STATUS_BAD_ALLOC;
    }
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

size_t cdc_avl_tree_erase(struct cdc_avl_tree *t, void *key)
{
  assert(t != NULL);

  struct cdc_avl_tree_node *node =
      cdc_find_tree_node(t->root, key, t->dinfo->cmp);
  if (!node) {
    return 0;
  }

  t->root = erase_node(t, node);
  --t->size;
  return 1;
}

void cdc_avl_tree_clear(struct cdc_avl_tree *t)
{
  assert(t != NULL);

  free_avl_tree(t, t->root);
  t->size = 0;
  t->root = NULL;
}

void cdc_avl_tree_swap(struct cdc_avl_tree *a, struct cdc_avl_tree *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(struct cdc_avl_tree_node *, a->root, b->root);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}

void cdc_avl_tree_begin(struct cdc_avl_tree *t, struct cdc_avl_tree_iter *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = cdc_min_tree_node(t->root);
  it->prev = NULL;
}

void cdc_avl_tree_end(struct cdc_avl_tree *t, struct cdc_avl_tree_iter *it)
{
  assert(t != NULL);
  assert(it != NULL);

  it->container = t;
  it->current = NULL;
  it->prev = cdc_max_tree_node(t->root);
}

void cdc_avl_tree_iter_next(struct cdc_avl_tree_iter *it)
{
  assert(it != NULL);

  it->prev = it->current;
  it->current = cdc_tree_successor(it->current);
}

void cdc_avl_tree_iter_prev(struct cdc_avl_tree_iter *it)
{
  assert(it != NULL);

  it->current = it->prev;
  it->prev = cdc_tree_predecessor(it->current);
}
