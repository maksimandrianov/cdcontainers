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
#include "cdcontainers/binomial-heap.h"

#include "cdcontainers/data-info.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static binomial_heap_node_t *new_node(void *key)
{
  binomial_heap_node_t *node = (binomial_heap_node_t *)calloc(sizeof(binomial_heap_node_t), 1);
  if (!node) return NULL;

  node->key = key;
  return node;
}

static void free_node(binomial_heap_t *h, binomial_heap_node_t *node)
{
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(node->key);
  }

  free(node);
}

static void free_heap(binomial_heap_t *h, binomial_heap_node_t *root)
{
  binomial_heap_node_t *tmp = NULL;
  while (root) {
    free_heap(h, root->child);
    tmp = root->sibling;
    free_node(h, root);
    root = tmp;
  }
}

static bool is_heap(binomial_heap_node_t *root, cdc_binary_pred_fn_t compar)
{
  while (root) {
    if (!is_heap(root->child, compar) || (root->parent && compar(root->key, root->parent->key))) {
      return false;
    }

    root = root->sibling;
  }

  return true;
}

static binomial_heap_node_t *find_prev_top(binomial_heap_t *h)
{
  if (h->root == NULL) {
    return NULL;
  }

  binomial_heap_node_t *m = h->root;
  binomial_heap_node_t *prev = NULL;
  binomial_heap_node_t *next = h->root;
  while (next->sibling) {
    prev = next;
    if (h->dinfo->cmp(m->key, next->key)) {
      m = next->key;
    }
    next = next->sibling;
  }

  return prev;
}

static binomial_heap_node_t *find_top(binomial_heap_t *h)
{
  binomial_heap_node_t *node = find_prev_top(h);
  return node ? node->sibling : h->root;
}

static void update_top(binomial_heap_t *h, binomial_heap_node_t *v)
{
  if (h->top == NULL || (v && h->dinfo->cmp(v->key, h->top->key))) {
    h->top = v;
  }
}

static void link(binomial_heap_node_t *a, binomial_heap_node_t *broot)
{
  a->parent = broot;
  a->sibling = broot->child;
  broot->child = a;
  ++broot->degree;
}

static binomial_heap_node_t *merge(binomial_heap_node_t *a, binomial_heap_node_t *b)
{
  if (a == NULL) {
    return b;
  }

  if (b == NULL) {
    return a;
  }

  binomial_heap_node_t *result = NULL;
  binomial_heap_node_t *next = NULL;
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

static binomial_heap_node_t *meld(binomial_heap_node_t *a, binomial_heap_node_t *b,
                                  cdc_binary_pred_fn_t compare)
{
  binomial_heap_node_t *result = merge(a, b);
  if (result == NULL) {
    return result;
  }

  binomial_heap_node_t *prev = NULL;
  binomial_heap_node_t *curr = result;
  binomial_heap_node_t *next = result->sibling;
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

static binomial_heap_node_t *decrease_key(binomial_heap_t *h, binomial_heap_node_t *node, void *key)
{
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(node->key);
  }

  node->key = key;
  binomial_heap_node_t *p = node->parent;
  while (p != NULL && h->dinfo->cmp(node->key, p->key)) {
    CDC_SWAP(void *, node->key, p->key);
    node = p;
    p = node->parent;
  }

  return node;
}

static binomial_heap_node_t *increase_key(binomial_heap_t *h, binomial_heap_node_t *pos, void *key)
{
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(pos->key);
  }

  pos->key = key;
  binomial_heap_node_t *ch = pos->child, *t = NULL;
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

static stat_t init_varg(binomial_heap_t *h, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    stat_t ret = binomial_heap_insert(h, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  return CDC_STATUS_OK;
}

stat_t binomial_heap_ctor(binomial_heap_t **h, data_info_t *info)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  binomial_heap_t *tmp = (binomial_heap_t *)calloc(sizeof(binomial_heap_t), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (info && !(tmp->dinfo = di_shared_ctorc(info))) {
    free(tmp);
    return CDC_STATUS_BAD_ALLOC;
  }

  *h = tmp;
  return CDC_STATUS_OK;
}

stat_t binomial_heap_ctorl(binomial_heap_t **h, data_info_t *info, ...)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  stat_t ret = binomial_heap_ctorv(h, info, args);
  va_end(args);
  return ret;
}

stat_t binomial_heap_ctorv(binomial_heap_t **h, data_info_t *info, va_list args)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  stat_t ret = binomial_heap_ctor(h, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  return init_varg(*h, args);
}

void binomial_heap_dtor(binomial_heap_t *h)
{
  assert(h != NULL);

  free_heap(h, h->root);
  di_shared_dtor(h->dinfo);
  free(h);
}

stat_t binomial_heap_extract_top(binomial_heap_t *h)
{
  assert(h != NULL);

  binomial_heap_node_t *tmp = NULL;
  binomial_heap_node_t *prev_top = find_prev_top(h);
  if (prev_top) {
    tmp = prev_top->sibling;
    prev_top->sibling = tmp->sibling;
  } else {
    tmp = h->root;
    h->root = tmp->sibling;
  }

  binomial_heap_node_t *root = tmp->child;
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

stat_t binomial_heap_riinsert(binomial_heap_t *h, void *key, binomial_heap_iter_t *ret)
{
  assert(h != NULL);

  binomial_heap_node_t *node = new_node(key);
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

void binomial_heap_change_key(binomial_heap_t *h, binomial_heap_iter_t *pos, void *key)
{
  assert(h != NULL);
  assert(pos != NULL);
  assert(h == pos->container);

  binomial_heap_node_t *node = NULL;
  binomial_heap_node_t *curr = pos->current;
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

void binomial_heap_clear(binomial_heap_t *h)
{
  assert(h != NULL);

  free_heap(h, h->root);
  h->size = 0;
  h->root = NULL;
  h->top = NULL;
}

void binomial_heap_swap(binomial_heap_t *a, binomial_heap_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(binomial_heap_node_t *, a->root, b->root);
  CDC_SWAP(binomial_heap_node_t *, a->top, b->top);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}

void binomial_heap_merge(binomial_heap_t *h, binomial_heap_t *other)
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

bool binomial_heap_is_heap(binomial_heap_t *h)
{
  assert(h != NULL);

  return is_heap(h->root, h->dinfo->cmp);
}
