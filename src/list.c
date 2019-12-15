// The MIT License (MIT)
// Copyright (c) 2017 Maksim Andrianov
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
#include "cdcontainers/list.h"

#include "cdcontainers/data-info.h"

#include <stdint.h>
#include <string.h>

static struct cdc_list_node *make_new_node(void *val)
{
  struct cdc_list_node *node =
      (struct cdc_list_node *)malloc(sizeof(struct cdc_list_node));
  if (node) {
    node->data = val;
  }

  return node;
}

static void free_node(struct cdc_list *l, struct cdc_list_node *node)
{
  assert(l != NULL);

  if (CDC_HAS_DFREE(l->dinfo)) {
    l->dinfo->dfree(node->data);
  }

  free(node);
}

static void free_nodes(struct cdc_list *l)
{
  struct cdc_list_node *current = l->head;
  struct cdc_list_node *next = NULL;
  while (current) {
    next = current->next;
    free_node(l, current);
    current = next;
  };
}

static enum cdc_stat insert_mid(struct cdc_list *l, struct cdc_list_node *n,
                                void *value)
{
  struct cdc_list_node *node = make_new_node(value);
  if (node == NULL) {
    return CDC_STATUS_BAD_ALLOC;
  }

  node->next = n;
  node->prev = n->prev;
  n->prev->next = node;
  n->prev = node;
  ++l->size;
  return CDC_STATUS_OK;
}

static enum cdc_stat insert(struct cdc_list *l, struct cdc_list_node *n,
                            void *value)
{
  if (n == l->head) {
    return cdc_list_push_front(l, value);
  }

  if (n == NULL) {
    return cdc_list_push_back(l, value);
  }

  return insert_mid(l, n, value);
}

static struct cdc_list_node *get_node(struct cdc_list *l, size_t index)
{
  struct cdc_list_node *node;
  size_t i = 0;
  for (node = l->head; node != NULL && i != index; node = node->next, ++i) {
    /* empty */;
  }

  return i != index ? NULL : node;
}

static size_t distance(struct cdc_list_node *first, struct cdc_list_node *last)
{
  size_t i = 0;
  for (; first != last; first = first->next, ++i) {
    /* empty */
  }

  return i;
}

static void erase_node(struct cdc_list *l, struct cdc_list_node *node)
{
  if (node == l->tail) {
    cdc_list_pop_back(l);
  } else if (node == l->head) {
    cdc_list_pop_front(l);
  } else {
    node->next->prev = node->prev;
    node->prev->next = node->next;
    free_node(l, node);
    --l->size;
  }
}

static void cmerge(struct cdc_list_node **ha, struct cdc_list_node **ta,
                   struct cdc_list_node *hb, struct cdc_list_node *tb,
                   cdc_binary_pred_fn_t compare)
{
  struct cdc_list_node *a = *ha;
  struct cdc_list_node *b = hb;
  if (b == NULL) {
    return;
  }
  if (a == NULL) {
    *ha = hb;
    *ta = tb;
    return;
  }

  struct cdc_list_node *head = NULL;
  struct cdc_list_node *tail = NULL;
  while (b != NULL && a != NULL) {
    if (compare(a->data, b->data)) {
      if (head == NULL) {
        head = tail = a;
      } else {
        tail->next = a;
        a->prev = tail;
        tail = a;
      }

      a = a->next;
    } else {
      if (head == NULL) {
        head = tail = b;
      } else {
        tail->next = b;
        b->prev = tail;
        tail = b;
      }

      b = b->next;
    }
  }

  if (a == NULL) {
    tail->next = b;
    b->prev = tail;
    tail = tb;
  } else if (b == NULL) {
    tail->next = a;
    a->prev = tail;
    tail = *ta;
  }

  *ha = head;
  *ta = tail;
}

static void halve(struct cdc_list_node **ha, struct cdc_list_node **ta,
                  struct cdc_list_node **hb, struct cdc_list_node **tb)
{
  struct cdc_list_node *slow = *ha;
  struct cdc_list_node *fast = slow;
  if (slow == NULL || slow->next == NULL) {
    *hb = NULL;
    *tb = NULL;
    return;
  }

  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }

  *hb = slow;
  *tb = *ta;
  *ta = slow->prev;
  (*ta)->next = NULL;
  (*hb)->prev = NULL;
}

static void merge_sort(struct cdc_list_node **head, struct cdc_list_node **tail,
                       cdc_binary_pred_fn_t compare)
{
  struct cdc_list_node *ha = *head;
  struct cdc_list_node *ta = *tail;
  if (ha == NULL || ha->next == NULL) {
    return;
  }

  struct cdc_list_node *hb = NULL;
  struct cdc_list_node *tb = NULL;
  halve(&ha, &ta, &hb, &tb);
  merge_sort(&ha, &ta, compare);
  merge_sort(&hb, &tb, compare);
  cmerge(&ha, &ta, hb, tb, compare);
  *head = ha;
  *tail = ta;
}

static enum cdc_stat init_varg(struct cdc_list *l, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    enum cdc_stat ret = cdc_list_push_back(l, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_ctor(struct cdc_list **l, struct cdc_data_info *info)
{
  assert(l != NULL);

  struct cdc_list *tmp = (struct cdc_list *)calloc(sizeof(struct cdc_list), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
    free(tmp);
    return CDC_STATUS_BAD_ALLOC;
  }

  *l = tmp;
  return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_ctorl(struct cdc_list **l, struct cdc_data_info *info,
                             ...)
{
  assert(l != NULL);

  va_list args;
  va_start(args, info);
  enum cdc_stat ret = cdc_list_ctorv(l, info, args);
  va_end(args);
  return ret;
}

enum cdc_stat cdc_list_ctorv(struct cdc_list **l, struct cdc_data_info *info,
                             va_list args)
{
  assert(l != NULL);

  enum cdc_stat ret = cdc_list_ctor(l, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  return init_varg(*l, args);
}

void cdc_list_dtor(struct cdc_list *l)
{
  assert(l != NULL);

  free_nodes(l);
  cdc_di_shared_dtor(l->dinfo);
  free(l);
}

void cdc_list_set(struct cdc_list *l, size_t index, void *value)
{
  assert(l != NULL);

  get_node(l, index)->data = value;
}

enum cdc_stat cdc_list_push_back(struct cdc_list *l, void *value)
{
  assert(l != NULL);

  struct cdc_list_node *node = make_new_node(value);
  if (!node) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (l->tail == NULL) {
    node->next = NULL;
    node->prev = NULL;
    l->head = node;
    l->tail = node;
  } else {
    node->next = NULL;
    node->prev = l->tail;
    l->tail->next = node;
    l->tail = node;
  }

  ++l->size;
  return CDC_STATUS_OK;
}

void cdc_list_pop_back(struct cdc_list *l)
{
  assert(l != NULL);
  assert(l->tail != NULL);

  struct cdc_list_node *new_tail = l->tail->prev;
  free_node(l, l->tail);
  if (new_tail) {
    new_tail->next = NULL;
    l->tail = new_tail;
  } else {
    l->tail = NULL;
    l->head = NULL;
  }

  --l->size;
}

enum cdc_stat cdc_list_push_front(struct cdc_list *l, void *value)
{
  assert(l != NULL);

  struct cdc_list_node *node = make_new_node(value);
  if (!node) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (l->head == NULL) {
    node->next = NULL;
    node->prev = NULL;
    l->head = node;
    l->tail = node;
  } else {
    node->next = l->head;
    node->prev = NULL;
    l->head->prev = node;
    l->head = node;
  }

  ++l->size;
  return CDC_STATUS_OK;
}

void cdc_list_foreach(struct cdc_list *l, void (*cb)(void *))
{
  assert(l != NULL);
  assert(cb != NULL);

  struct cdc_list_node *node = l->head;
  while (node) {
    cb(node->data);
    node = node->next;
  }
}

enum cdc_stat cdc_list_at(struct cdc_list *l, size_t index, void **elem)
{
  assert(l != NULL);
  assert(elem != NULL);

  if (index > l->size - 1) {
    return CDC_STATUS_OUT_OF_RANGE;
  }

  struct cdc_list_node *node = get_node(l, index);
  *elem = node->data;
  return CDC_STATUS_OK;
}

void *cdc_list_get(struct cdc_list *l, size_t index)
{
  assert(l != NULL);

  return get_node(l, index)->data;
}

void cdc_list_pop_front(struct cdc_list *l)
{
  assert(l != NULL);
  assert(l->head != NULL);

  struct cdc_list_node *new_head = l->head->next;
  free_node(l, l->head);
  if (new_head) {
    new_head->prev = NULL;
    l->head = new_head;
  } else {
    l->tail = NULL;
    l->head = NULL;
  }
  --l->size;
}

enum cdc_stat cdc_list_insert(struct cdc_list *l, size_t index, void *value)
{
  assert(l != NULL);
  assert(index <= l->size);

  struct cdc_list_node *node = get_node(l, index);
  return insert(l, node, value);
}

enum cdc_stat cdc_list_iinsert(struct cdc_list_iter *before, void *value)
{
  assert(before != NULL);

  return insert(before->container, before->current, value);
}

void cdc_list_erase(struct cdc_list *l, size_t index)
{
  assert(l != NULL);
  assert(index < l->size);

  struct cdc_list_node *node = get_node(l, index);
  erase_node(l, node);
}

void cdc_list_ierase(struct cdc_list_iter *pos)
{
  assert(pos != NULL);

  erase_node(pos->container, pos->current);
}

void cdc_list_clear(struct cdc_list *l)
{
  assert(l != NULL);

  free_nodes(l);
  l->size = 0;
  l->head = NULL;
  l->tail = NULL;
}

void cdc_list_swap(struct cdc_list *a, struct cdc_list *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(struct cdc_list_node *, a->head, b->head);
  CDC_SWAP(struct cdc_list_node *, a->tail, b->tail);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(struct cdc_data_info *, a->dinfo, b->dinfo);
}

void cdc_list_splice(struct cdc_list_iter *position,
                     struct cdc_list_iter *first, struct cdc_list_iter *last)
{
  assert(position != NULL);
  assert(first != NULL);
  assert(last != NULL);

  struct cdc_list_iter prev_last = *last;
  size_t len = distance(first->current, last->current);
  cdc_list_iter_prev(&prev_last);
  if (first->current->prev) {
    if (last->current) {
      first->current->prev->next = last->current;
      last->current->prev = first->current->prev;
    } else {
      last->container->tail = first->current->prev;
      last->container->tail->next = NULL;
    }
  } else {
    if (last->current) {
      last->container->head = last->current;
      last->container->head->prev = NULL;
    } else {
      last->container->head = NULL;
      last->container->tail = NULL;
    }
  }

  if (position->current) {
    if (position->current->prev) {
      position->current->prev->next = first->current;
      first->current->prev = position->current->prev;
      position->current->prev = prev_last.current;
      prev_last.current->next = position->current;
    } else {
      position->container->head->prev = prev_last.current;
      prev_last.current->next = position->container->head;
      position->container->head = first->current;
      first->current->prev = NULL;
    }
  } else {
    if (position->container->tail) {
      position->container->tail->next = first->current;
      first->current->prev = position->container->tail;
      position->container->tail = prev_last.current;
    } else {
      first->current->prev = NULL;
      prev_last.current->next = NULL;
      position->container->head = first->current;
      position->container->tail = prev_last.current;
    }
  }

  position->container->size += len;
  first->container->size -= len;
}

void cdc_list_ssplice(struct cdc_list_iter *position,
                      struct cdc_list_iter *first)
{
  assert(position != NULL);
  assert(first != NULL);

  struct cdc_list_iter end;
  cdc_list_end(first->container, &end);
  cdc_list_splice(position, first, &end);
}

void cdc_list_lsplice(struct cdc_list_iter *position, struct cdc_list *other)
{
  assert(position != NULL);
  assert(other != NULL);

  struct cdc_list_iter beg;
  struct cdc_list_iter end;
  cdc_list_begin(other, &beg);
  cdc_list_end(other, &end);
  cdc_list_splice(position, &beg, &end);
}

void cdc_list_cmerge(struct cdc_list *l, struct cdc_list *other,
                     cdc_binary_pred_fn_t compare)
{
  assert(l != NULL);
  assert(other != NULL);
  assert(compare != NULL);

  cmerge(&l->head, &l->tail, other->head, other->tail, compare);
  l->size = l->size + other->size;
  other->head = NULL;
  other->tail = NULL;
  other->size = 0;
}

void cdc_list_merge(struct cdc_list *l, struct cdc_list *other)
{
  assert(l != NULL);
  assert(CDC_HAS_CMP(l->dinfo));

  cdc_list_cmerge(l, other, l->dinfo->cmp);
}

void cdc_list_erase_if(struct cdc_list *l, cdc_unary_pred_fn_t pred)
{
  assert(l != NULL);

  struct cdc_list_node *curr = l->head;
  struct cdc_list_node *tmp = NULL;
  while (curr) {
    if (pred(curr->data)) {
      tmp = curr->next;
      erase_node(l, curr);
      curr = tmp;
      if (curr == NULL) {
        return;
      }
    } else {
      curr = curr->next;
    }
  }
}

void cdc_list_reverse(struct cdc_list *l)
{
  assert(l != NULL);

  struct cdc_list_node *curr = l->head;
  struct cdc_list_node *next = NULL;
  while (curr) {
    next = curr->next;
    CDC_SWAP(struct cdc_list_node *, curr->next, curr->prev);
    curr = next;
  }

  CDC_SWAP(struct cdc_list_node *, l->head, l->tail);
}

void cdc_list_punique(struct cdc_list *l, cdc_binary_pred_fn_t pred)
{
  assert(l != NULL);

  struct cdc_list_node *curr = l->head;
  if (curr == NULL) {
    return;
  }

  struct cdc_list_node *next = curr->next;
  while (next) {
    if (pred(curr->data, next->data)) {
      erase_node(l, next);
      next = curr->next;
    } else {
      curr = next;
      next = next->next;
    }
  }
}

void cdc_list_unique(struct cdc_list *l)
{
  assert(l != NULL);
  assert(CDC_HAS_CMP(l->dinfo));

  cdc_list_punique(l, l->dinfo->cmp);
}

void cdc_list_csort(struct cdc_list *l, cdc_binary_pred_fn_t compare)
{
  assert(l != NULL);
  assert(compare != NULL);

  merge_sort(&l->head, &l->tail, compare);
}

void cdc_list_sort(struct cdc_list *l)
{
  assert(l != NULL);
  assert(CDC_HAS_CMP(l->dinfo));

  cdc_list_csort(l, l->dinfo->cmp);
}
