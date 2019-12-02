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

#include "cdcontainers/data-info.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static struct cdc_binomial_heap_node *new_node(void *key)
{
  struct cdc_binomial_heap_node *node = (struct cdc_binomial_heap_node *)calloc(
      sizeof(struct cdc_binomial_heap_node), 1);
  if (!node) return NULL;

  node->key = key;
  return node;
}

static void free_node(struct cdc_binomial_heap *h,
                      struct cdc_binomial_heap_node *node)
{
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(node->key);
  }

  free(node);
}

static void free_heap(struct cdc_binomial_heap *h,
                      struct cdc_binomial_heap_node *root)
{
  struct cdc_binomial_heap_node *tmp = NULL;
  while (root) {
    free_heap(h, root->child);
    tmp = root->sibling;
    free_node(h, root);
    root = tmp;
  }
}

static bool is_heap(struct cdc_binomial_heap_node *root,
                    cdc_binary_pred_fn_t compar)
{
  while (root) {
    if (!is_heap(root->child, compar) ||
        (root->parent && compar(root->key, root->parent->key))) {
      return false;
    }

    root = root->sibling;
  }

  return true;
}

static struct cdc_binomial_heap_node *find_prev_top(struct cdc_binomial_heap *h)
{
  if (h->root == NULL) {
    return NULL;
  }

  struct cdc_binomial_heap_node *m = h->root;
  struct cdc_binomial_heap_node *prev = NULL;
  struct cdc_binomial_heap_node *next = h->root;
  while (next->sibling) {
    prev = next;
    if (h->dinfo->cmp(m->key, next->key)) {
      m = next->key;
    }
    next = next->sibling;
  }

  return prev;
}

static struct cdc_binomial_heap_node *find_top(struct cdc_binomial_heap *h)
{
  struct cdc_binomial_heap_node *node = find_prev_top(h);
  return node ? node->sibling : h->root;
}

static void update_top(struct cdc_binomial_heap *h,
                       struct cdc_binomial_heap_node *v)
{
  if (h->top == NULL || (v && h->dinfo->cmp(v->key, h->top->key))) {
    h->top = v;
  }
}

static void link(struct cdc_binomial_heap_node *a,
                 struct cdc_binomial_heap_node *broot)
{
  a->parent = broot;
  a->sibling = broot->child;
  broot->child = a;
  ++broot->degree;
}

static struct cdc_binomial_heap_node *merge(struct cdc_binomial_heap_node *a,
                                            struct cdc_binomial_heap_node *b)
{
  if (a == NULL) {
    return b;
  }

  if (b == NULL) {
    return a;
  }

  struct cdc_binomial_heap_node *result = NULL;
  struct cdc_binomial_heap_node *next = NULL;
  while (b != NULL && a != NULL) {
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

  if (a == NULL) {
    next->sibling = b;
  } else {
    next->sibling = a;
  }

  return result;
}

static struct cdc_binomial_heap_node *meld(struct cdc_binomial_heap_node *a,
                                           struct cdc_binomial_heap_node *b,
                                           cdc_binary_pred_fn_t compare)
{
  struct cdc_binomial_heap_node *result = merge(a, b);
  if (result == NULL) {
    return result;
  }

  struct cdc_binomial_heap_node *prev = NULL;
  struct cdc_binomial_heap_node *curr = result;
  struct cdc_binomial_heap_node *next = result->sibling;
  while (next) {
    if (curr->degree != next->degree ||
        (next->sibling && (next->sibling->degree == curr->degree))) {
      prev = curr;
      curr = next;
    } else if (compare(curr->key, next->key)) {
      curr->sibling = next->sibling;
      link(next, curr);
    } else {
      if (prev == NULL) {
        result = next;
      } else {
        prev->sibling = next;
      }

      link(curr, next);
      next = curr;
    }

    next = curr->sibling;
  }

  return result;
}

static struct cdc_binomial_heap_node *decrease_key(
    struct cdc_binomial_heap *h, struct cdc_binomial_heap_node *node, void *key)
{
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(node->key);
  }

  node->key = key;
  struct cdc_binomial_heap_node *p = node->parent;
  while (p != NULL && h->dinfo->cmp(node->key, p->key)) {
    CDC_SWAP(void *, node->key, p->key);
    node = p;
    p = node->parent;
  }

  return node;
}

static struct cdc_binomial_heap_node *increase_key(
    struct cdc_binomial_heap *h, struct cdc_binomial_heap_node *pos, void *key)
{
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(pos->key);
  }

  pos->key = key;
  struct cdc_binomial_heap_node *ch = pos->child, *t = NULL;
  while (ch != NULL) {
    if (h->dinfo->cmp(ch->key, pos->key)) {
      t = ch;
      while (ch->sibling != NULL) {
        if (h->dinfo->cmp(ch->sibling->key, t->key)) {
          t = ch->sibling;
        }

        ch = ch->sibling;
      }

      CDC_SWAP(void *, t->key, pos->key);
      pos = t;
      ch = pos->child;
    } else {
      ch = ch->sibling;
    }
  }

  return pos;
}

static enum cdc_stat init_varg(struct cdc_binomial_heap *h, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    enum cdc_stat ret = cdc_binomial_heap_insert(h, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  return CDC_STATUS_OK;
}

enum cdc_stat cdc_binomial_heap_ctor(struct cdc_binomial_heap **h,
                                     struct cdc_data_info *info)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  struct cdc_binomial_heap *tmp =
      (struct cdc_binomial_heap *)calloc(sizeof(struct cdc_binomial_heap), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
    free(tmp);
    return CDC_STATUS_BAD_ALLOC;
  }

  *h = tmp;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_binomial_heap_ctorl(struct cdc_binomial_heap **h,
                                      struct cdc_data_info *info, ...)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  enum cdc_stat ret = cdc_binomial_heap_ctorv(h, info, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_binomial_heap_ctorv(struct cdc_binomial_heap **h,
                                      struct cdc_data_info *info, va_list args)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  enum cdc_stat ret = cdc_binomial_heap_ctor(h, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  return init_varg(*h, args);
}

void cdc_binomial_heap_dtor(struct cdc_binomial_heap *h)
{
  assert(h != NULL);

  free_heap(h, h->root);
  cdc_di_shared_dtor(h->dinfo);
  free(h);
}

enum cdc_stat cdc_binomial_heap_extract_top(struct cdc_binomial_heap *h)
{
  assert(h != NULL);

  struct cdc_binomial_heap_node *tmp = NULL;
  struct cdc_binomial_heap_node *prev_top = find_prev_top(h);
  if (prev_top) {
    tmp = prev_top->sibling;
    prev_top->sibling = tmp->sibling;
  } else {
    tmp = h->root;
    h->root = tmp->sibling;
  }

  struct cdc_binomial_heap_node *root = tmp->child;
  free_node(h, tmp);
  tmp = root;
  while (tmp) {
    tmp->parent = NULL;
    tmp = tmp->sibling;
  }

  h->root = meld(h->root, root, h->dinfo->cmp);
  h->top = find_top(h);
  --h->size;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_binomial_heap_riinsert(struct cdc_binomial_heap *h, void *key,
                                         struct cdc_binomial_heap_iter *ret)
{
  assert(h != NULL);

  struct cdc_binomial_heap_node *node = new_node(key);
  if (!node) return CDC_STATUS_BAD_ALLOC;

  update_top(h, node);
  h->root = meld(h->root, node, h->dinfo->cmp);
  ++h->size;
  if (ret) {
    ret->container = h;
    ret->current = node;
  }

  return CDC_STATUS_OK;
}

void cdc_binomial_heap_change_key(struct cdc_binomial_heap *h,
                                  struct cdc_binomial_heap_iter *pos, void *key)
{
  assert(h != NULL);
  assert(pos != NULL);
  assert(h == pos->container);

  struct cdc_binomial_heap_node *node = NULL;
  struct cdc_binomial_heap_node *curr = pos->current;
  if (h->dinfo->cmp(key, curr->key)) {
    node = decrease_key(h, curr, key);
    update_top(h, node);

  } else {
    node = increase_key(h, curr, key);
    if (curr == h->top) {
      h->top = find_top(h);
    }
  }

  pos->current = node;
}

void cdc_binomial_heap_clear(struct cdc_binomial_heap *h)
{
  assert(h != NULL);

  free_heap(h, h->root);
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
  CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}

void cdc_binomial_heap_merge(struct cdc_binomial_heap *h,
                             struct cdc_binomial_heap *other)
{
  assert(h != NULL);
  assert(other != NULL);

  h->root = meld(h->root, other->root, h->dinfo->cmp);
  update_top(h, other->top);
  h->size += other->size;
  other->size = 0;
  other->root = NULL;
  other->top = NULL;
}

bool cdc_binomial_heap_is_heap(struct cdc_binomial_heap *h)
{
  assert(h != NULL);

  return is_heap(h->root, h->dinfo->cmp);
}
