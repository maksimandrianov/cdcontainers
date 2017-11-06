#include "cdcontainers/list.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "cdcontainers/common.h"

struct node {
        struct node *next;
        struct node *prev;
        void *data;
};

struct cdc_list {
        struct node *head;
        struct node *tail;
        size_t size;
        cdc_free_func_t fp_free;
};

static inline void cdc_list_add(struct node *new_node,
                                struct node *prev_node,
                                struct node *next_node)
{
        assert(new_node != NULL);

        if (next_node)
                next_node->prev = new_node;

        new_node->next = next_node;
        new_node->prev = prev_node;

        if (prev_node)
                prev_node->next = new_node;
}

static inline void cdc_list_remove(struct node *prev_node,
                                   struct node *next_node)
{
        if (next_node)
                next_node->prev = prev_node;

        if (prev_node)
                prev_node->next = next_node;
}

static inline void cdc_list_free_all_node(cdc_list_t *l)
{
        assert(l != NULL);

        struct node *current = l->head;
        struct node *next;

        do {
                next = current->next;
                if (l->fp_free)
                        l->fp_free(current->data);
                free(current);
                current = next;

        } while(next != NULL);
}

static inline struct node *cdc_list_get_node(cdc_list_t *l, size_t index)
{
        assert(l != NULL);

        struct node *node;
        size_t i;

        for (node = l->head, i = 0; node != NULL && i != index;
             node = node->next, ++i)
                /* empty */;

        return node;
}

static inline void cdc_free_node(cdc_list_t *l, struct node *node, bool is_free)
{
        assert(node != NULL);

        if (is_free && l->fp_free)
                (*l->fp_free)(node->data);

        free(node);
}

static inline enum cdc_stat cdc_list_pop_back_f(cdc_list_t *l, bool is_free)
{
        assert(l != NULL);
        assert(l->tail != NULL);

        struct node *new_tail = l->tail->prev;

        cdc_free_node(l, l->tail, is_free);
        --l->size;

        if (new_tail) {
                cdc_list_remove(new_tail, NULL);
                l->tail = new_tail;
        } else {
                l->tail = NULL;
                l->head = NULL;
        }

        return CDC_STATUS_OK;
}

static inline enum cdc_stat cdc_list_pop_front_f(cdc_list_t *l, bool is_free)
{
        assert(l != NULL);
        assert(l->head != NULL);

        struct node *new_head = l->head->next;

        cdc_free_node(l, l->head, is_free);
        --l->size;

        if (new_head) {
                cdc_list_remove(NULL, new_head);
                l->head = new_head;
        } else {
                l->tail = NULL;
                l->head = NULL;
        }

        return CDC_STATUS_OK;
}

static inline enum cdc_stat cdc_list_init_varg(cdc_list_t *l, va_list args)
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

enum cdc_stat cdc_list_ctor(cdc_list_t **l, cdc_free_func_t func)
{
        cdc_list_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_list_t *)malloc(sizeof(cdc_list_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->head    = NULL;
        tmp->tail    = NULL;
        tmp->size    = 0;
        tmp->fp_free = func;

        *l = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_ctorl(cdc_list_t **l, cdc_free_func_t func, ...)
{
        assert(l != NULL);

        enum cdc_stat ret;
        va_list args;

        va_start(args, func);
        ret = cdc_list_ctorv(l, func, args);
        va_end(args);

        return ret;
}

enum cdc_stat cdc_list_ctorv(cdc_list_t **l, cdc_free_func_t func, va_list args)
{
        assert(l != NULL);

        enum cdc_stat ret;

        ret = cdc_list_ctor(l, func);
        if (ret != CDC_STATUS_OK)
                return ret;

        return cdc_list_init_varg(*l, args);
}

void cdc_list_dtor(cdc_list_t *l)
{
        assert(l != NULL);

        if (l->head != NULL)
                cdc_list_free_all_node(l);

        free(l);
}

enum cdc_stat cdc_list_push_back(cdc_list_t *l, void *elem)
{
        assert(l != NULL);

        struct node *node;

        node = (struct node *)malloc(sizeof(struct node));
        if (!node)
                return CDC_STATUS_BAD_ALLOC;

        node->data = elem;
        if (l->tail == NULL) {
                cdc_list_add(node, NULL, NULL);
                l->head = node;
                l->tail = node;
        } else {
                cdc_list_add(node, l->tail, NULL);
                l->tail = node;
        }

        ++l->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_pop_back(cdc_list_t *l)
{
        assert(l != NULL);
        assert(l->tail != NULL);

        return cdc_list_pop_back_f(l, true);
}

enum cdc_stat cdc_list_push_front(cdc_list_t *l, void *elem)
{
        assert(l != NULL);

        struct node *node;

        node = (struct node *)malloc(sizeof(struct node));
        if (node == NULL)
                return CDC_STATUS_BAD_ALLOC;

        node->data = elem;
        if (l->head == NULL) {
                cdc_list_add(node, NULL, NULL);
                l->head = node;
                l->tail = node;
        } else {
                cdc_list_add(node, NULL, l->head);
                l->head = node;
        }

        ++l->size;

        return CDC_STATUS_OK;
}

void cdc_list_foreach(cdc_list_t *l, void (*cb)(void *, size_t))
{
        assert(l != NULL);
        assert(cb != NULL);

        struct node *node;
        size_t i;

        for (node = l->head, i = 0; node != NULL; node = node->next, ++i)
                (*cb)(node->data, i);
}

enum cdc_stat cdc_list_at(cdc_list_t *l, size_t index, void **elem)
{
        assert(l != NULL);
        assert(elem != NULL);

        struct node *node;

        if (index > l->size)
                return CDC_STATUS_OUT_OF_RANGE;

        node = cdc_list_get_node(l, index);
        *elem = node->data;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_pop_front(cdc_list_t *l)
{
        assert(l != NULL);
        assert(l->head != NULL);

        return cdc_list_pop_front_f(l, true);
}

enum cdc_stat cdc_list_insert(cdc_list_t *l, size_t index, void *elem)
{
        assert(l);
        assert(index <= l->size);

        struct node *new_node, *prev_node;

        if (index == l->size)
                return cdc_list_push_back(l, elem);

        if (index == 0)
                return cdc_list_push_front(l, elem);

        new_node = (struct node *)malloc(sizeof(struct node));
        if (new_node == NULL)
                return CDC_STATUS_BAD_ALLOC;

        new_node->data = elem;
        prev_node = cdc_list_get_node(l, index - 1);
        cdc_list_add(new_node, prev_node, prev_node->next);
        ++l->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_erase(cdc_list_t *l, size_t index, void **elem)
{
        assert(l);
        assert(index < l->size);

        struct node *node;

        if (index == l->size - 1) {
                *elem = l->tail->data;
                return cdc_list_pop_back_f(l, false);
        }

        if (index == 0) {
                *elem = l->head->data;
                return cdc_list_pop_front_f(l, false);
        }

        node = cdc_list_get_node(l, index);
        *elem = node->data;
        cdc_list_remove(node->prev, node->next);
        --l->size;
        free(node);

        return CDC_STATUS_OK;
}

void cdc_list_clear(cdc_list_t *l)
{
        assert(l != NULL);

        cdc_list_free_all_node(l);
        l->size = 0;
        l->head = NULL;
        l->tail = NULL;
}

void *cdc_list_front(cdc_list_t *l)
{
        assert(l != NULL);
        assert(l->head != NULL);

        return l->head->data;
}

void *cdc_list_back(cdc_list_t *l)
{
        assert(l != NULL);
        assert(l->tail != NULL);

        return l->tail->data;
}

void cdc_list_swap(cdc_list_t *a, cdc_list_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct node *,   a->head,     b->head);
        CDC_SWAP(struct node *,   a->tail,     b->tail);
        CDC_SWAP(size_t,          a->size,     b->size);
        CDC_SWAP(cdc_free_func_t, a->fp_free,  b->fp_free);
}

size_t cdc_list_size(cdc_list_t *l)
{
        assert(l != NULL);

        return l->size;
}

bool cdc_list_empty(cdc_list_t *l)
{
        assert(l != NULL);

        return l->size == 0;
}
