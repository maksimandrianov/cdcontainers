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

#include <string.h>
#include <stdint.h>
#include "data-info.h"

static inline void free_node(struct cdc_list *l, struct cdc_list_node *node,
                             bool must_free)
{
        assert(node != NULL);

        if (must_free && CDC_HAS_DFREE(l))
                l->dinfo->dfree(node->data);

        free(node);
}

static inline void free_nodes(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_node *current = l->head;
        struct cdc_list_node *next;


        while (current) {
                next = current->next;
                free_node(l, current, true);
                current = next;

        };
}

static inline enum cdc_stat insert_mid(struct cdc_list *l,
                                       struct cdc_list_node *n, void *value)
{
        assert(l != NULL);

        struct cdc_list_node *node;

        node = (struct cdc_list_node *)malloc(sizeof(struct cdc_list_node));
        if (node == NULL)
                return CDC_STATUS_BAD_ALLOC;

        node->data = value;
        node->prev = n->prev->next;
        n->prev->next = node;
        node->next = n;
        n->prev =  node;
        ++l->size;
        return CDC_STATUS_OK;
}

static inline enum cdc_stat insert(struct cdc_list *l, struct cdc_list_node *n,
                                   void *value)
{
        if (n == l->head)
                return cdc_list_push_front(l, value);

        if (n == NULL)
                return cdc_list_push_back(l, value);

        return insert_mid(l, n, value);
}

static inline struct cdc_list_node *get_node(struct cdc_list *l, size_t index)
{
        assert(l != NULL);

        struct cdc_list_node *node;
        size_t i;

        for (node = l->head, i = 0; node != NULL && i != index;
             node = node->next, ++i)
                /* empty */;

        return i != index ? NULL : node;
}

static inline size_t distance(struct cdc_list_node *first,
                              struct cdc_list_node *last)
{
        size_t i;

        for (i = 0; first != last; first = first->next, ++i)
                /* empty */;

        return i;
}

static inline enum cdc_stat pop_back_f(struct cdc_list *l, bool must_free)
{
        assert(l != NULL);
        assert(l->tail != NULL);

        struct cdc_list_node *new_tail = l->tail->prev;

        free_node(l, l->tail, must_free);
        if (new_tail) {
                new_tail->next = NULL;
                l->tail = new_tail;
        } else {
                l->tail = NULL;
                l->head = NULL;
        }

        --l->size;
        return CDC_STATUS_OK;
}

static inline enum cdc_stat pop_front_f(struct cdc_list *l, bool must_free)
{
        assert(l != NULL);
        assert(l->head != NULL);

        struct cdc_list_node *new_head = l->head->next;

        free_node(l, l->head, must_free);
        if (new_head) {
                new_head->prev = NULL;
                l->head = new_head;
        } else {
                l->tail = NULL;
                l->head = NULL;
        }

        --l->size;
        return CDC_STATUS_OK;
}

static inline enum cdc_stat remove_mid(struct cdc_list *l,
                                       struct cdc_list_node *node, void **elem)
{
        assert(l != NULL);

        node->next->prev = node->prev;
        node->prev->next = node->next;
        --l->size;

        if (elem)
                *elem = node->data;

        free_node(l, node, !elem);
        return CDC_STATUS_OK;
}

static inline enum cdc_stat remove(struct cdc_list *l,
                                   struct cdc_list_node *node, void **elem)
{
        if (node == l->tail) {
                if (elem)
                        *elem = l->tail->data;

                return pop_back_f(l, !elem);
        }

        if (node == l->head) {
                if (elem)
                        *elem = l->head->data;

                return pop_front_f(l, !elem);
        }

        return remove_mid(l, node, elem);
}

static inline void cmerge(struct cdc_list_node **ha, struct cdc_list_node **ta,
                          struct cdc_list_node *hb, struct cdc_list_node *tb,
                          cdc_binary_pred_fn_t compare)
{

        struct cdc_list_node *a = *ha, *b = hb, *head = NULL, *tail;

        if (b == NULL)
                return;

        if (a == NULL) {
                *ha = hb;
                *ta = tb;
                return;
        }

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

static inline void halve(struct cdc_list_node **ha, struct cdc_list_node **ta,
                         struct cdc_list_node **hb, struct cdc_list_node **tb)
{

        struct cdc_list_node *slow = *ha, *fast = slow;

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
        struct cdc_list_node *ha = *head, *ta = *tail, *hb, *tb;

        if (ha == NULL || ha->next == NULL)
                return;

        halve(&ha, &ta, &hb, &tb);
        merge_sort(&ha, &ta, compare);
        merge_sort(&hb, &tb, compare);
        cmerge(&ha, &ta, hb, tb, compare);

        *head = ha;
        *tail = ta;
}

static inline enum cdc_stat init_varg(struct cdc_list *l, va_list args)
{
        assert(l != NULL);

        enum cdc_stat ret;
        void *elem;

        while ((elem = va_arg(args, void *)) != NULL) {
                ret = cdc_list_push_back(l, elem);
                if (ret != CDC_STATUS_OK)
                        return ret;
        }

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_ctor(struct cdc_list **l, struct cdc_data_info *info)
{
        assert(l !=  NULL);

        struct cdc_list *tmp;

        tmp = (struct cdc_list *)calloc(sizeof(struct cdc_list), 1);
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        if (info && !(tmp->dinfo = cdc_di_shared_ctorc(info))) {
                free(tmp);
                return CDC_STATUS_BAD_ALLOC;
        }

        *l = tmp;
        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_ctorl(struct cdc_list **l,
                             struct cdc_data_info *info, ...)
{
        assert(l != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, info);
        ret = cdc_list_ctorv(l, info, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_list_ctorv(struct cdc_list **l,
                             struct cdc_data_info *info, va_list args)
{
        assert(l != NULL);

        enum cdc_stat ret;

        ret = cdc_list_ctor(l, info);
        if (ret != CDC_STATUS_OK)
                return ret;

        return init_varg(*l, args);
}

void cdc_list_dtor(struct cdc_list *l)
{
        assert(l != NULL);

        free_nodes(l);
        cdc_di_shared_dtor(l->dinfo);
        free(l);
}

void cdc_list_set(struct cdc_list  *l, size_t index, void *value)
{
        assert(l != NULL);

        get_node(l, index)->data = value;
}

enum cdc_stat cdc_list_push_back(struct cdc_list *l, void *value)
{
        assert(l != NULL);

        struct cdc_list_node *node;

        node = (struct cdc_list_node *)malloc(sizeof(struct cdc_list_node));
        if (!node)
                return CDC_STATUS_BAD_ALLOC;

        node->data = value;
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

enum cdc_stat cdc_list_pop_back(struct cdc_list *l)
{
        assert(l != NULL);
        assert(l->tail != NULL);

        return pop_back_f(l, true);
}

enum cdc_stat cdc_list_push_front(struct cdc_list *l, void *value)
{
        assert(l != NULL);

        struct cdc_list_node *node;

        node = (struct cdc_list_node *)malloc(sizeof(struct cdc_list_node));
        if (node == NULL)
                return CDC_STATUS_BAD_ALLOC;

        node->data = value;
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

        struct cdc_list_node *node;

        for (node = l->head; node != NULL; node = node->next)
                cb(node->data);
}

enum cdc_stat cdc_list_at(struct cdc_list *l, size_t index, void **elem)
{
        assert(l != NULL);
        assert(elem != NULL);

        struct cdc_list_node *node;

        if (index > l->size - 1)
                return CDC_STATUS_OUT_OF_RANGE;

        node = get_node(l, index);
        *elem = node->data;

        return CDC_STATUS_OK;
}

void *cdc_list_get(struct cdc_list *l, size_t index)
{
        assert(l != NULL);

        return get_node(l, index)->data;
}

enum cdc_stat cdc_list_pop_front(struct cdc_list *l)
{
        assert(l != NULL);
        assert(l->head != NULL);

        return pop_front_f(l, true);
}

enum cdc_stat cdc_list_insert(struct cdc_list *l, size_t index, void *value)
{
        assert(l != NULL);
        assert(index <= l->size);

        struct cdc_list_node *node = get_node(l, index);

        return insert(l, node, value);
}

enum cdc_stat cdc_list_iinsert(struct cdc_list_iter before, void *value)
{
        assert(before.container != NULL);

        return insert(before.container, before.current, value);
}

enum cdc_stat cdc_list_remove(struct cdc_list *l, size_t index, void **elem)
{
        assert(l != NULL);
        assert(index < l->size);

        struct cdc_list_node *node = get_node(l, index);

        return remove(l, node, elem);
}

enum cdc_stat cdc_list_iremove(struct cdc_list_iter pos, void **elem)
{
        assert(pos.container);

        return remove(pos.container, pos.current, elem);
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

void cdc_list_splice(struct cdc_list_iter position, struct cdc_list_iter first,
                     struct cdc_list_iter last)
{
        assert(first.container == last.container);
        assert(first.container != position.container);

        struct cdc_list_iter prev_last = cdc_list_iter_prev(last);
        size_t len = distance(first.current, last.current);

        if (first.current->prev) {
                if (last.current) {
                        first.current->prev->next = last.current;
                        last.current->prev = first.current->prev;
                } else {
                        last.container->tail = first.current->prev;
                        last.container->tail->next = NULL;
                }
        } else {
                if (last.current) {
                        last.container->head = last.current;
                        last.container->head->prev = NULL;
                } else {
                        last.container->head = NULL;
                        last.container->tail = NULL;
                }
        }

        if (position.current) {
                if (position.current->prev) {
                        position.current->prev->next = first.current;
                        first.current->prev = position.current->prev;
                        position.current->prev = prev_last.current;
                        prev_last.current->next = position.current;
                } else {
                        position.container->head->prev = prev_last.current;
                        prev_last.current->next = position.container->head;
                        position.container->head = first.current;
                        first.current->prev = NULL;
                }
        } else {
                if (position.container->tail) {
                        position.container->tail->next = first.current;
                        first.current->prev = position.container->tail;
                        position.container->tail = prev_last.current;
                } else {
                        first.current->prev = NULL;
                        prev_last.current->next = NULL;
                        position.container->head = first.current;
                        position.container->tail = prev_last.current;
                }
        }

        position.container->size += len;
        first.container->size -= len;
}

void cdc_list_ssplice(struct cdc_list_iter position, struct cdc_list_iter first)
{
        cdc_list_splice(position, first, cdc_list_end(first.container));
}

void cdc_list_lsplice(struct cdc_list_iter position, struct cdc_list *other)
{
        assert(other);

        cdc_list_splice(position, cdc_list_begin(other), cdc_list_end(other));
}

void cdc_list_cmerge(struct cdc_list *l, struct cdc_list *other,
                     cdc_binary_pred_fn_t compare)
{
        assert(l != NULL);
        assert(other);
        assert(compare);

        cmerge(&l->head, &l->tail, other->head, other->tail, compare);
        l->size = l->size + other->size;
        other->head = NULL;
        other->tail = NULL;
        other->size = 0;
}

void cdc_list_merge(struct cdc_list *l, struct cdc_list *other)
{
        assert(l != NULL);

        cdc_list_cmerge(l, other, l->dinfo->lt);
}

void cdc_list_erase_if(struct cdc_list *l, cdc_unary_pred_fn_t pred)
{
        assert(l != NULL);

        struct cdc_list_node *curr = l->head, *tmp;

        while (curr) {
                if (pred(curr->data)) {
                        tmp = curr->next;
                        remove(l, curr, NULL);
                        curr = tmp;
                        if (curr == NULL)
                                return;
                } else {
                        curr = curr->next;
                }
        }
}

void cdc_list_reverse(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_node *curr = l->head, *next;

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

        struct cdc_list_node *curr = l->head, *next;

        if (curr == NULL)
                return;

        next = curr->next;
        while (next) {
                if (pred(curr->data, next->data)) {
                        remove(l, next, NULL);
                        next = curr->next;
                        if (next == NULL)
                                return;
                }

                curr = next;
                next = next->next;
        }
}

void cdc_list_unique(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_node *curr = l->head, *next;

        if (curr == NULL)
                return;

        next = curr->next;
        while (next) {
                if (cdc_eq(l->dinfo->lt, curr->data, next->data)) {
                        remove(l, next, NULL);
                        next = curr->next;
                        if (next == NULL)
                                return;
                }

                curr = next;
                next = next->next;
        }
}

void cdc_list_csort(struct cdc_list *l, cdc_binary_pred_fn_t compare)
{
        assert(l != NULL);
        assert(compare);

        merge_sort(&l->head, &l->tail, compare);
}

void cdc_list_sort(struct cdc_list *l)
{
        assert(l != NULL);

        cdc_list_csort(l, l->dinfo->lt);
}
