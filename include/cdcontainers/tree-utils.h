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
#ifndef CDCONTAINERS_SRC_TREE_H
#define CDCONTAINERS_SRC_TREE_H

#include <cdcontainers/data-info.h>

#include <stddef.h>

#define CDC_MAKE_FIND_NODE_FN(T)                                           \
  static T cdc_find_tree_node(T node, void *key, cdc_binary_pred_fn_t cmp) \
  {                                                                        \
    while (node != NULL && cdc_not_eq(cmp, node->key, key)) {              \
      if (cmp(key, node->key)) {                                           \
        node = node->left;                                                 \
      } else {                                                             \
        node = node->right;                                                \
      }                                                                    \
    }                                                                      \
    return node;                                                           \
  }

#define CDC_MAKE_MIN_NODE_FN(T)      \
  static T cdc_min_tree_node(T node) \
  {                                  \
    if (node == NULL) {              \
      return NULL;                   \
    }                                \
    while (node->left != NULL) {     \
      node = node->left;             \
    }                                \
    return node;                     \
  }

#define CDC_MAKE_MAX_NODE_FN(T)      \
  static T cdc_max_tree_node(T node) \
  {                                  \
    if (node == NULL) {              \
      return NULL;                   \
    }                                \
    while (node->right != NULL) {    \
      node = node->right;            \
    }                                \
    return node;                     \
  }

#define CDC_MAKE_SUCCESSOR_FN(T)             \
  static T cdc_tree_successor(T node)        \
  {                                          \
    if (node->right) {                       \
      return cdc_min_tree_node(node->right); \
    }                                        \
    T p = node->parent;                      \
    while (p && node == p->right) {          \
      node = p;                              \
      p = p->parent;                         \
    }                                        \
    return p;                                \
  }

#define CDC_MAKE_PREDECESSOR_FN(T)          \
  static T cdc_tree_predecessor(T node)     \
  {                                         \
    if (node->left) {                       \
      return cdc_max_tree_node(node->left); \
    }                                       \
    T p = node->parent;                     \
    while (p && node == p->left) {          \
      node = p;                             \
      p = p->parent;                        \
    }                                       \
    return p;                               \
  }

#define CDC_MAKE_TREE_HEIGTH_FN(T)            \
  static size_t cdc_tree_height(T node)       \
  {                                           \
    if (node == NULL) {                       \
      return 0;                               \
    }                                         \
    size_t lh = cdc_tree_height(node->left);  \
    size_t rh = cdc_tree_height(node->right); \
    return CDC_MAX(lh, rh) + 1;               \
  }

#endif  // CDCONTAINERS_SRC_TREE_H
