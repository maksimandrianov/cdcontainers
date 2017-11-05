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

struct cds_list {
        struct list_node *head;
        struct list_node *tail;
        size_t size;
        void (*fp_free)(void *);
};

static void cds_list_add(struct list_node *new_node,
                         struct list_node *prev_node,
                         struct list_node *next_node)
{
        assert(new_node != NULL);

        if (next_node != NULL)
                next_node->prev = new_node;

        new_node->next = next_node;
        new_node->prev = prev_node;

        if (prev_node != NULL)
                prev_node->next = new_node;
}

static void cds_list_remove(struct list_node *prev_node,
                            struct list_node *next_node)
{
        next_node->prev = prev_node;
        prev_node->next = next_node;
}

enum cds_stat cds_list_ctor(cds_list_t **l, void (*fp_free)(void *))
{
        cds_list_t *tmp;
        enum cds_stat ret;

        if (!(tmp = (cds_list_t *)malloc(sizeof(cds_list_t))))
                return CDS_STATUS_BAD_ALLOC;

        tmp->head    = NULL;
        tmp->tail    = NULL;
        tmp->size    = 0;
        tmp->fp_free = fp_free;

        *l = tmp;

        return CDS_STATUS_OK;
}

enum cds_stat cds_list_ctor_l(cds_list_t **l, void (*fp_free)(void *), ...)
{
        assert(l != NULL);

        enum cds_stat ret;
        va_list args;
        void *elem;

        if ((ret = cds_list_ctor(l, fp_free)) != CDS_STATUS_OK)
                return ret;

        va_start(args, fp_free);
        while ((elem = va_arg(args, void *)) != NULL) {
                if ((ret = cds_list_push_back(*l, elem)) != CDS_STATUS_OK) {
                        va_end(args);
                        return ret;
                }
        }

        va_end(args);

        return CDS_STATUS_OK;
}

void cds_list_dtor(cds_list_t *l)
{
        assert(l != NULL);

        if (l->head != NULL) {
                struct list_node *current = l->head;
                struct list_node *next;

                do {
                        next = current->next;
                        free(current);
                        if (l->fp_free)
                                l->fp_free(current->data);
                        current = next;

                } while(next != NULL);
        }

        free(l);
}

enum cds_stat cds_list_push_back(cds_list_t *l, void *elem)
{
        assert(l != NULL);

        struct list_node *node;

        node = (struct list_node *)malloc(sizeof(struct list_node));
        if (node == NULL)
                return CDS_STATUS_BAD_ALLOC;

        node->data = elem;
        if (l->tail == NULL) {
                cds_list_add(node, NULL, NULL);
                l->head = node;
                l->tail = node;
        } else {
                cds_list_add(node, l->tail, NULL);
                l->tail = node;
        }

        ++l->size;

        return CDS_STATUS_OK;
}

enum cds_stat cds_list_pop_back(cds_list_t *l)
{

}

enum cds_stat cds_list_push_front(cds_list_t *l, void *elem)
{
        assert(l != NULL);

        struct list_node *node;

        node = (struct list_node *)malloc(sizeof(struct list_node));
        if (node == NULL)
                return CDS_STATUS_BAD_ALLOC;

        node = elem;
        if (l->head == NULL) {
                cds_list_add(node, NULL, NULL);
                l->head = node;
                l->tail = node;
        } else {

                cds_list_add(node, NULL, l->head);
                l->head = node;
        }

        ++l->size;

        return CDS_STATUS_OK;
}

void cds_list_foreach(cds_list_t *l, void (*cb)(void *, size_t))
{
        assert(l != NULL);
        assert(cb != NULL);

        struct list_node *node;
        size_t i;

        for (node = l->head, i = 0; node != NULL; node = node->next, ++i)
                (*cb)(node->data, i);
}

enum cds_stat cds_list_at(cds_list_t *l, size_t index, void **elem)
{
        assert(l != NULL);
        assert(elem != NULL);

        struct list_node *node;
        size_t i;

        if (index > l->size)
                return CDS_STATUS_OUT_OF_RANGE;

        for (node = l->head, i = 0; node != NULL && i != index;
             node = node->next, ++i)
                /*empty*/;

        *elem = node->data;

        return CDS_STATUS_OK;
}

enum cds_stat cds_list_pop_front(cds_list_t *l)
{

}

size_t cds_list_size(cds_list_t *l)
{
        assert(l != NULL);

        return l->size;
}

bool cds_list_empty(cds_list_t *l)
{
        assert(l != NULL);

        return l->size == 0;
}
