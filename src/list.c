#include "list.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include "common.h"

struct list_node {
        struct list_node *next;
        struct list_node *prev;
        void *data;
};

struct cdc_list {
        struct list_node *head;
        struct list_node *tail;
        size_t size;
        void (*fp_free)(void *);
};

static inline void cdc_list_add(struct list_node *new_node,
                                struct list_node *prev_node,
                                struct list_node *next_node)
{
        assert(new_node != NULL);

        if (next_node)
                next_node->prev = new_node;

        new_node->next = next_node;
        new_node->prev = prev_node;

        if (prev_node)
                prev_node->next = new_node;
}

static inline void cdc_list_remove(struct list_node *prev_node,
                                   struct list_node *next_node)
{
        if (next_node)
                next_node->prev = prev_node;

        if (prev_node)
                prev_node->next = next_node;
}

static inline struct list_node *cdc_list_get_node(cdc_list_t *l, size_t index)
{
        assert(l != NULL);

        struct list_node *node;
        size_t i;

        for (node = l->head, i = 0; node != NULL && i != index;
             node = node->next, ++i)
                /* empty */;

        return node;
}

enum cdc_stat cdc_list_ctor(cdc_list_t **l, void (*fp_free)(void *))
{
        cdc_list_t *tmp;
        enum cdc_stat ret;

        tmp = (cdc_list_t *)malloc(sizeof(cdc_list_t));
        if (!tmp)
                return CDC_STATUS_BAD_ALLOC;

        tmp->head    = NULL;
        tmp->tail    = NULL;
        tmp->size    = 0;
        tmp->fp_free = fp_free;

        *l = tmp;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_ctor_l(cdc_list_t **l, void (*fp_free)(void *), ...)
{
        assert(l != NULL);

        enum cdc_stat ret;
        va_list args;
        void *elem;

        ret = cdc_list_ctor(l, fp_free);
        if (ret != CDC_STATUS_OK)
                return ret;

        va_start(args, fp_free);
        while ((elem = va_arg(args, void *)) != NULL) {
                ret = cdc_list_push_back(*l, elem);
                if (ret != CDC_STATUS_OK) {
                        va_end(args);
                        return ret;
                }
        }

        va_end(args);

        return CDC_STATUS_OK;
}

void cdc_list_dtor(cdc_list_t *l)
{
        assert(l != NULL);

        if (l->head != NULL) {
                struct list_node *current = l->head;
                struct list_node *next;

                do {
                        next = current->next;
                        if (l->fp_free)
                                l->fp_free(current->data);
                        free(current);
                        current = next;

                } while(next != NULL);
        }

        free(l);
}

enum cdc_stat cdc_list_push_back(cdc_list_t *l, void *elem)
{
        assert(l != NULL);

        struct list_node *node;

        node = (struct list_node *)malloc(sizeof(struct list_node));
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

        struct list_node *new_tail = l->tail->prev;

        // remove tail data
        free(l->tail);
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

enum cdc_stat cdc_list_push_front(cdc_list_t *l, void *elem)
{
        assert(l != NULL);

        struct list_node *node;

        node = (struct list_node *)malloc(sizeof(struct list_node));
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

        struct list_node *node;
        size_t i;

        for (node = l->head, i = 0; node != NULL; node = node->next, ++i)
                (*cb)(node->data, i);
}

enum cdc_stat cdc_list_at(cdc_list_t *l, size_t index, void **elem)
{
        assert(l != NULL);
        assert(elem != NULL);

        struct list_node *node;

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

        struct list_node *new_head = l->head->next;

        // remove head data
        free(l->head);
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

enum cdc_stat cdc_list_insert(cdc_list_t *v, size_t index, void *elem)
{

}

enum cdc_stat cdc_list_erase(cdc_list_t *v, size_t index, void **elem)
{

}

void cdc_list_clear(cdc_list_t *v)
{

}

void *cdc_list_front(cdc_list_t *l)
{
        assert(l != NULL);

        return l->head->data;
}

void *cdc_list_back(cdc_list_t *l)
{
        assert(l != NULL);

        return l->tail->data;
}

void cdc_list_swap(cdc_list_t *a, cdc_list_t *b)
{
        assert(a != NULL);
        assert(b != NULL);

        CDC_SWAP(struct list_node *,  a->head,     b->head);
        CDC_SWAP(struct list_node *,  a->tail,     b->tail);
        CDC_SWAP(size_t,              a->size,     b->size);
        CDC_SWAP(void *,              a->fp_free,  b->fp_free);
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
