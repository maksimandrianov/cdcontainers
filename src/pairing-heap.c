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
#include "cdcontainers/pairing-heap.h"

#include "cdcontainers/data-info.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void free_node(pairing_heap_t *h, pairing_heap_node_t *node)
{
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(node->key);
  }

  free(node);
}

static void free_heap(pairing_heap_t *h, pairing_heap_node_t *root)
{
  while (root) {
    free_heap(h, root->child);
    pairing_heap_node_t *tmp = root->sibling;
    free_node(h, root);
    root = tmp;
  }
}

static bool is_heap(pairing_heap_node_t *root, cdc_binary_pred_fn_t compar)
{
  while (root) {
    if (!is_heap(root->child, compar) || (root->parent && compar(root->key, root->parent->key))) {
      return false;
    }

    root = root->sibling;
  }
  return true;
}

static void link(pairing_heap_node_t *a, pairing_heap_node_t *broot)
{
  a->parent = broot;
  a->sibling = broot->child;
  broot->child = a;
}

static pairing_heap_node_t *meld(pairing_heap_node_t *a, pairing_heap_node_t *b,
                                 cdc_binary_pred_fn_t compare)
{
  if (a == NULL) {
    return b;
  }

  if (b == NULL) {
    return a;
  }

  if (compare(a->key, b->key)) {
    CDC_SWAP(pairing_heap_node_t *, a, b);
  }

  link(a, b);
  return b;
}

static pairing_heap_node_t *two_pass_meld(pairing_heap_node_t *root, cdc_binary_pred_fn_t compare)
{
  if (root == NULL || root->sibling == NULL) {
    return root;
  }

  pairing_heap_node_t *a = root;
  pairing_heap_node_t *b = root->sibling;
  pairing_heap_node_t *c = root->sibling->sibling;
  a->parent = NULL;
  a->sibling = NULL;
  b->parent = NULL;
  b->sibling = NULL;
  return meld(meld(a, b, compare), two_pass_meld(c, compare), compare);
}

static pairing_heap_node_t *decrease_key(pairing_heap_t *h, pairing_heap_node_t *pos, void *key)
{
  pairing_heap_node_t *node = pos;
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(node->key);
  }

  node->key = key;
  pairing_heap_node_t *p = node->parent;
  while (p != NULL && h->dinfo->cmp(node->key, p->key)) {
    CDC_SWAP(void *, node->key, p->key);
    node = p;
    p = node->parent;
  }

  return node;
}

static pairing_heap_node_t *increase_key(pairing_heap_t *h, pairing_heap_node_t *pos, void *key)
{
  if (CDC_HAS_DFREE(h->dinfo)) {
    h->dinfo->dfree(pos->key);
  }

  pairing_heap_node_t *ch = pos->child;
  pos->key = key;
  while (ch != NULL) {
    if (h->dinfo->cmp(ch->key, pos->key)) {
      pairing_heap_node_t *t = ch;
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

static stat_t init_varg(pairing_heap_t *h, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    stat_t ret = pairing_heap_insert(h, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  return CDC_STATUS_OK;
}

stat_t pairing_heap_ctor(pairing_heap_t **h, data_info_t *info)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  pairing_heap_t *tmp = (pairing_heap_t *)calloc(sizeof(pairing_heap_t), 1);
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

stat_t pairing_heap_ctorl(pairing_heap_t **h, data_info_t *info, ...)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  va_list args;
  va_start(args, info);
  stat_t ret = pairing_heap_ctorv(h, info, args);
  va_end(args);
  return ret;
}

stat_t pairing_heap_ctorv(pairing_heap_t **h, data_info_t *info, va_list args)
{
  assert(h != NULL);
  assert(CDC_HAS_CMP(info));

  stat_t ret = pairing_heap_ctor(h, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  return init_varg(*h, args);
}

void pairing_heap_dtor(pairing_heap_t *h)
{
  assert(h != NULL);

  free_heap(h, h->root);
  di_shared_dtor(h->dinfo);
  free(h);
}

stat_t pairing_heap_extract_top(pairing_heap_t *h)
{
  assert(h != NULL);

  pairing_heap_node_t *node = h->root;
  h->root = two_pass_meld(h->root->child, h->dinfo->cmp);
  free_node(h, node);
  --h->size;
  return CDC_STATUS_OK;
}

stat_t pairing_heap_riinsert(pairing_heap_t *h, void *key, pairing_heap_iter_t *ret)
{
  assert(h != NULL);

  pairing_heap_node_t *root = (pairing_heap_node_t *)calloc(sizeof(pairing_heap_node_t), 1);
  if (!root) {
    return CDC_STATUS_BAD_ALLOC;
  }

  root->key = key;
  h->root = meld(h->root, root, h->dinfo->cmp);
  ++h->size;
  if (ret) {
    ret->container = h;
    ret->current = root;
  }

  return CDC_STATUS_OK;
}

void pairing_heap_change_key(pairing_heap_t *h, pairing_heap_iter_t *pos, void *key)
{
  //        assert(h != NULL);
  //        assert(pos != NULL);
  //        assert(h == pos->container);

  //        pairing_heap_node_t *node;
  //        pairing_heap_node_t *curr = pos->current;

  //        if (h->compar(key, curr->key))
  //                node = decrease_key(h, curr, key);
  //        else
  //                node = increase_key(h, curr, key);

  //        pos->current = node;
}

void pairing_heap_clear(pairing_heap_t *h)
{
  assert(h != NULL);

  free_heap(h, h->root);
  h->size = 0;
  h->root = NULL;
}

void pairing_heap_swap(pairing_heap_t *a, pairing_heap_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(pairing_heap_node_t *, a->root, b->root);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}

void pairing_heap_merge(pairing_heap_t *h, pairing_heap_t *other)
{
  assert(h);
  assert(other);

  h->root = meld(h->root, other->root, h->dinfo->cmp);
  h->size += other->size;
  other->size = 0;
  other->root = NULL;
}

bool pairing_heap_is_heap(pairing_heap_t *h)
{
  assert(h != NULL);

  return is_heap(h->root, h->dinfo->cmp);
}
