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

#include "data-info.h"

#include <stddef.h>

#define MAKE_FIND_NODE(T)                                         \
  static T find_node(T node, void *key, cdc_binary_pred_fn_t cmp) \
  {                                                               \
    while (node != NULL && cdc_not_eq(cmp, node->key, key)) {     \
      if (cmp(key, node->key))                                    \
        node = node->left;                                        \
      else                                                        \
        node = node->right;                                       \
    }                                                             \
    return node;                                                  \
  }

#define MAKE_MIN_NODE(T)                          \
  static T min_node(T node)                       \
  {                                               \
    if (node == NULL) return NULL;                \
    while (node->left != NULL) node = node->left; \
    return node;                                  \
  }

#define MAKE_MAX_NODE(T)                            \
  static T max_node(T node)                         \
  {                                                 \
    if (node == NULL) return NULL;                  \
    while (node->right != NULL) node = node->right; \
    return node;                                    \
  }

#define MAKE_SUCCESSOR(T)                          \
  static T successor(T node)                       \
  {                                                \
    T p;                                           \
    if (node->right) return min_node(node->right); \
    p = node->parent;                              \
    while (p && node == p->right) {                \
      node = p;                                    \
      p = p->parent;                               \
    }                                              \
    return p;                                      \
  }

#define MAKE_PREDECESSOR(T)                      \
  static T predecessor(T node)                   \
  {                                              \
    T p;                                         \
    if (node->left) return max_node(node->left); \
    p = node->parent;                            \
    while (p && node == p->left) {               \
      node = p;                                  \
      p = p->parent;                             \
    }                                            \
    return p;                                    \
  }

#define MAKE_TREE_HEIGTH(T)         \
  static size_t tree_height(T node) \
  {                                 \
    size_t lh, rh;                  \
    if (node == NULL) return 0;     \
    lh = tree_height(node->left);   \
    rh = tree_height(node->right);  \
    return CDC_MAX(lh, rh) + 1;     \
  }

#endif  // CDCONTAINERS_SRC_TREE_H
