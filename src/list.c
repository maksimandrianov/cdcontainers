#include "cdcontainers/list.h"

#include <string.h>
#include <stdint.h>
#include "data-info.h"

static inline void ladd(struct cdc_list_node *new_node,
                        struct cdc_list_node *prev_node,
                        struct cdc_list_node *next_node)
{
        assert(new_node != NULL);

        if (next_node)
                next_node->prev = new_node;

        new_node->next = next_node;
        new_node->prev = prev_node;

        if (prev_node)
                prev_node->next = new_node;
}

static inline void lremove(struct cdc_list_node *prev_node,
                           struct cdc_list_node *next_node)
{
        if (next_node)
                next_node->prev = prev_node;

        if (prev_node)
                prev_node->next = next_node;
}

static inline void free_all_nodes(struct cdc_list *l)
{
        assert(l != NULL);

        struct cdc_list_node *current = l->head;
        struct cdc_list_node *next;

        do {
                next = current->next;
                if (CDC_HAS_FREE(l))
                        l->dinfo->free(current->data);
                free(current);
                current = next;

        } while(next != NULL);
}

static inline struct cdc_list_node *get_node(struct cdc_list *l, size_t index)
{
        assert(l != NULL);

        struct cdc_list_node *node;
        size_t i;

        for (node = l->head, i = 0; node != NULL && i != index;
             node = node->next, ++i)
                /* empty */;

        return node;
}

static inline void free_node(struct cdc_list *l, struct cdc_list_node *node,
                             bool must_free)
{
        assert(node != NULL);

        if (must_free && CDC_HAS_FREE(l))
                l->dinfo->free(node->data);

        free(node);
}

static inline enum cdc_stat pop_back_f(struct cdc_list *l, bool must_free)
{
        assert(l != NULL);
        assert(l->tail != NULL);

        struct cdc_list_node *new_tail = l->tail->prev;

        free_node(l, l->tail, must_free);
        --l->size;

        if (new_tail) {
                lremove(new_tail, NULL);
                l->tail = new_tail;
        } else {
                l->tail = NULL;
                l->head = NULL;
        }

        return CDC_STATUS_OK;
}

static inline enum cdc_stat pop_front_f(struct cdc_list *l, bool must_free)
{
        assert(l != NULL);
        assert(l->head != NULL);

        struct cdc_list_node *new_head = l->head->next;

        free_node(l, l->head, must_free);
        --l->size;

        if (new_head) {
                lremove(NULL, new_head);
                l->head = new_head;
        } else {
                l->tail = NULL;
                l->head = NULL;
        }

        return CDC_STATUS_OK;
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

        if (l->head != NULL)
                free_all_nodes(l);

        cdc_di_shared_dtor(l->dinfo);
        free(l);
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
                ladd(node, NULL, NULL);
                l->head = node;
                l->tail = node;
        } else {
                ladd(node, l->tail, NULL);
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
                ladd(node, NULL, NULL);
                l->head = node;
                l->tail = node;
        } else {
                ladd(node, NULL, l->head);
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

        if (index > l->size)
                return CDC_STATUS_OUT_OF_RANGE;

        node = get_node(l, index);
        *elem = node->data;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_pop_front(struct cdc_list *l)
{
        assert(l != NULL);
        assert(l->head != NULL);

        return pop_front_f(l, true);
}

enum cdc_stat cdc_list_insert(struct cdc_list *l, size_t index, void *value)
{
        assert(l);
        assert(index <= l->size);

        struct cdc_list_node *new_node, *prev_node;

        if (index == l->size)
                return cdc_list_push_back(l, value);

        if (index == 0)
                return cdc_list_push_front(l, value);

        new_node = (struct cdc_list_node *)malloc(sizeof(struct cdc_list_node));
        if (new_node == NULL)
                return CDC_STATUS_BAD_ALLOC;

        new_node->data = value;
        prev_node = get_node(l, index - 1);
        ladd(new_node, prev_node, prev_node->next);
        ++l->size;

        return CDC_STATUS_OK;
}

enum cdc_stat cdc_list_erase(struct cdc_list *l, size_t index, void **elem)
{
        assert(l);
        assert(index < l->size);

        struct cdc_list_node *node;

        if (index == l->size - 1) {
                *elem = l->tail->data;
                return pop_back_f(l, false);
        }

        if (index == 0) {
                *elem = l->head->data;
                return pop_front_f(l, false);
        }

        node = get_node(l, index);
        *elem = node->data;
        lremove(node->prev, node->next);
        --l->size;
        free(node);

        return CDC_STATUS_OK;
}

void cdc_list_clear(struct cdc_list *l)
{
        assert(l != NULL);

        free_all_nodes(l);
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
