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
#define CDC_USE_SHORT_NAMES
#include "cdcontainers/list.h"

#include "cdcontainers/data-info.h"

#include <stdint.h>
#include <string.h>

static list_node_t *make_new_node(void *val)
{
  list_node_t *node = (list_node_t *)malloc(sizeof(list_node_t));
  if (node) {
    node->data = val;
  }

  return node;
}

static void free_node(list_t *l, list_node_t *node)
{
  assert(l != NULL);

  if (CDC_HAS_DFREE(l->dinfo)) {
    l->dinfo->dfree(node->data);
  }

  free(node);
}

static void free_nodes(list_t *l)
{
  list_node_t *current = l->head;
  list_node_t *next = NULL;
  while (current) {
    next = current->next;
    free_node(l, current);
    current = next;
  };
}

static stat_t insert_mid(list_t *l, list_node_t *n, void *value)
{
  list_node_t *node = make_new_node(value);
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

static stat_t insert(list_t *l, list_node_t *n, void *value)
{
  if (n == l->head) {
    return list_push_front(l, value);
  }

  if (n == NULL) {
    return list_push_back(l, value);
  }

  return insert_mid(l, n, value);
}

static list_node_t *get_node(list_t *l, size_t index)
{
  list_node_t *node = NULL;
  size_t i;
  if (index < l->size / 2) {
    i = 0;
    for (node = l->head; node != NULL && i < index; node = node->next, ++i) {
      /* empty */;
    }
  } else {
    i = l->size - 1;
    for (node = l->tail; node != NULL && i > index; node = node->prev, --i) {
      /* empty */;
    }
  }

  return i != index ? NULL : node;
}

static size_t distance(list_node_t *first, list_node_t *last)
{
  size_t i = 0;
  for (; first != last; first = first->next, ++i) {
    /* empty */
  }

  return i;
}

static void erase_node(list_t *l, list_node_t *node)
{
  if (node == l->tail) {
    list_pop_back(l);
  } else if (node == l->head) {
    list_pop_front(l);
  } else {
    node->next->prev = node->prev;
    node->prev->next = node->next;
    free_node(l, node);
    --l->size;
  }
}

static void cmerge(list_node_t **ha, list_node_t **ta, list_node_t *hb, list_node_t *tb,
                   cdc_binary_pred_fn_t compare)
{
  list_node_t *a = *ha;
  list_node_t *b = hb;
  if (b == NULL) {
    return;
  }
  if (a == NULL) {
    *ha = hb;
    *ta = tb;
    return;
  }

  list_node_t *head = NULL;
  list_node_t *tail = NULL;
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

static void halve(list_node_t **ha, list_node_t **ta, list_node_t **hb, list_node_t **tb)
{
  list_node_t *slow = *ha;
  list_node_t *fast = slow;
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

static void merge_sort(list_node_t **head, list_node_t **tail, cdc_binary_pred_fn_t compare)
{
  list_node_t *ha = *head;
  list_node_t *ta = *tail;
  if (ha == NULL || ha->next == NULL) {
    return;
  }

  list_node_t *hb = NULL;
  list_node_t *tb = NULL;
  halve(&ha, &ta, &hb, &tb);
  merge_sort(&ha, &ta, compare);
  merge_sort(&hb, &tb, compare);
  cmerge(&ha, &ta, hb, tb, compare);
  *head = ha;
  *tail = ta;
}

static stat_t init_varg(list_t *l, va_list args)
{
  void *elem = NULL;
  while ((elem = va_arg(args, void *)) != CDC_END) {
    stat_t ret = list_push_back(l, elem);
    if (ret != CDC_STATUS_OK) {
      return ret;
    }
  }

  return CDC_STATUS_OK;
}

stat_t list_ctor(list_t **l, data_info_t *info)
{
  assert(l != NULL);

  list_t *tmp = (list_t *)calloc(sizeof(list_t), 1);
  if (!tmp) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (info && !(tmp->dinfo = di_shared_ctorc(info))) {
    free(tmp);
    return CDC_STATUS_BAD_ALLOC;
  }

  *l = tmp;
  return CDC_STATUS_OK;
}

stat_t list_ctorl(list_t **l, data_info_t *info, ...)
{
  assert(l != NULL);

  va_list args;
  va_start(args, info);
  stat_t ret = list_ctorv(l, info, args);
  va_end(args);
  return ret;
}

stat_t list_ctorv(list_t **l, data_info_t *info, va_list args)
{
  assert(l != NULL);

  stat_t ret = list_ctor(l, info);
  if (ret != CDC_STATUS_OK) {
    return ret;
  }

  return init_varg(*l, args);
}

void list_dtor(list_t *l)
{
  assert(l != NULL);

  free_nodes(l);
  di_shared_dtor(l->dinfo);
  free(l);
}

void list_set(list_t *l, size_t index, void *value)
{
  assert(l != NULL);

  get_node(l, index)->data = value;
}

stat_t list_push_back(list_t *l, void *value)
{
  assert(l != NULL);

  list_node_t *node = make_new_node(value);
  if (!node) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (l->tail == NULL) {
    node->prev = NULL;
    l->head = node;
  } else {
    node->prev = l->tail;
    l->tail->next = node;
  }

  node->next = NULL;
  l->tail = node;
  ++l->size;
  return CDC_STATUS_OK;
}

void list_pop_back(list_t *l)
{
  assert(l != NULL);
  assert(l->tail != NULL);

  list_node_t *new_tail = l->tail->prev;
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

stat_t list_push_front(list_t *l, void *value)
{
  assert(l != NULL);

  list_node_t *node = make_new_node(value);
  if (!node) {
    return CDC_STATUS_BAD_ALLOC;
  }

  if (l->head == NULL) {
    node->next = NULL;
    l->tail = node;
  } else {
    node->next = l->head;
    l->head->prev = node;
  }

  l->head = node;
  node->prev = NULL;
  ++l->size;
  return CDC_STATUS_OK;
}

void list_foreach(list_t *l, void (*cb)(void *))
{
  assert(l != NULL);
  assert(cb != NULL);

  list_node_t *node = l->head;
  while (node) {
    cb(node->data);
    node = node->next;
  }
}

stat_t list_at(list_t *l, size_t index, void **elem)
{
  assert(l != NULL);
  assert(elem != NULL);

  if (index + 1 > l->size) {
    return CDC_STATUS_OUT_OF_RANGE;
  }

  list_node_t *node = get_node(l, index);
  *elem = node->data;
  return CDC_STATUS_OK;
}

void *list_get(list_t *l, size_t index)
{
  assert(l != NULL);

  return get_node(l, index)->data;
}

void list_pop_front(list_t *l)
{
  assert(l != NULL);
  assert(l->head != NULL);

  list_node_t *new_head = l->head->next;
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

stat_t list_insert(list_t *l, size_t index, void *value)
{
  assert(l != NULL);
  assert(index <= l->size);

  list_node_t *node = get_node(l, index);
  return insert(l, node, value);
}

stat_t list_iinsert(list_iter_t *before, void *value)
{
  assert(before != NULL);

  return insert(before->container, before->current, value);
}

void list_erase(list_t *l, size_t index)
{
  assert(l != NULL);
  assert(index < l->size);

  list_node_t *node = get_node(l, index);
  erase_node(l, node);
}

void list_ierase(list_iter_t *pos)
{
  assert(pos != NULL);

  erase_node(pos->container, pos->current);
}

void list_clear(list_t *l)
{
  assert(l != NULL);

  free_nodes(l);
  l->size = 0;
  l->head = NULL;
  l->tail = NULL;
}

void list_swap(list_t *a, list_t *b)
{
  assert(a != NULL);
  assert(b != NULL);

  CDC_SWAP(list_node_t *, a->head, b->head);
  CDC_SWAP(list_node_t *, a->tail, b->tail);
  CDC_SWAP(size_t, a->size, b->size);
  CDC_SWAP(data_info_t *, a->dinfo, b->dinfo);
}

void list_splice(list_iter_t *position, list_iter_t *first, list_iter_t *last)
{
  assert(position != NULL);
  assert(first != NULL);
  assert(last != NULL);

  list_iter_t prev_last = *last;
  size_t len = distance(first->current, last->current);
  list_iter_prev(&prev_last);
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

void list_ssplice(list_iter_t *position, list_iter_t *first)
{
  assert(position != NULL);
  assert(first != NULL);

  list_iter_t end;
  list_end(first->container, &end);
  list_splice(position, first, &end);
}

void list_lsplice(list_iter_t *position, list_t *other)
{
  assert(position != NULL);
  assert(other != NULL);

  list_iter_t beg;
  list_iter_t end;
  list_begin(other, &beg);
  list_end(other, &end);
  list_splice(position, &beg, &end);
}

void list_cmerge(list_t *l, list_t *other, cdc_binary_pred_fn_t compare)
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

void list_merge(list_t *l, list_t *other)
{
  assert(l != NULL);
  assert(CDC_HAS_CMP(l->dinfo));

  list_cmerge(l, other, l->dinfo->cmp);
}

void list_erase_if(list_t *l, cdc_unary_pred_fn_t pred)
{
  assert(l != NULL);

  list_node_t *curr = l->head;
  list_node_t *tmp = NULL;
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

void list_reverse(list_t *l)
{
  assert(l != NULL);

  list_node_t *curr = l->head;
  list_node_t *next = NULL;
  while (curr) {
    next = curr->next;
    CDC_SWAP(list_node_t *, curr->next, curr->prev);
    curr = next;
  }

  CDC_SWAP(list_node_t *, l->head, l->tail);
}

void list_punique(list_t *l, cdc_binary_pred_fn_t pred)
{
  assert(l != NULL);

  list_node_t *curr = l->head;
  if (curr == NULL) {
    return;
  }

  list_node_t *next = curr->next;
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

void list_unique(list_t *l)
{
  assert(l != NULL);
  assert(CDC_HAS_CMP(l->dinfo));

  list_punique(l, l->dinfo->cmp);
}

void list_csort(list_t *l, cdc_binary_pred_fn_t compare)
{
  assert(l != NULL);
  assert(compare != NULL);

  merge_sort(&l->head, &l->tail, compare);
}

void list_sort(list_t *l)
{
  assert(l != NULL);
  assert(CDC_HAS_CMP(l->dinfo));

  list_csort(l, l->dinfo->cmp);
}
