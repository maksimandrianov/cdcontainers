/**
  * @file
  * @author Maksim Andrianov <maksimandrianov1@yandex.ru>
  * @brief The cdc_queue is a struct and functions that provide a queue
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>

/**
 * @brief The cdc_queue_table struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_queue_table {
        enum cdc_stat (*ctor)(void **cntr, struct cdc_data_info *info);
        enum cdc_stat (*ctorv)(void **cntr, struct cdc_data_info *info,
                               va_list args);
        void (*dtor)(void *cntr);
        void *(*front)(void *cntr);
        void *(*back)(void *cntr);
        bool (*empty)(void *cntr);
        size_t (*size)(void *cntr);
        enum cdc_stat (*push)(void *cntr, void *elem);
        enum cdc_stat (*pop)(void *cntr);
};

/**
 * @brief The cdc_queue struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_queue {
        void *container;
        const struct cdc_queue_table *table;
};

/**
 * @brief Table for the queue based on the list
 */
extern const void *cdc_queuel_table;

/**
 * @brief Table for the queue based on the deque
 */
extern const void *cdc_queued_table;

/**
 * @brief Constructs an empty queue.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_queue_ctor(const struct cdc_queue_table *table,
                             struct cdc_queue **q, struct cdc_data_info *info);

/**
 * @brief Constructs a queue, initialized by an arbitrary number of pointers.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_queue_ctorl(const struct cdc_queue_table *table,
                              struct cdc_queue **q,
                              struct cdc_data_info *info, ...);

/**
 * @brief Constructs a queue, initialized by args
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_queue_ctorv(const struct cdc_queue_table *table,
                              struct cdc_queue **q,
                              struct cdc_data_info *info, va_list args);

/**
 * @brief Destroys the queue.
 */
void cdc_queue_dtor(struct cdc_queue *q);

// Element access
/**
 * @brief Returns point to the first element in the queue. This function assumes
 * that the queue isn't empty.
 */
static inline void *cdc_queue_front(struct cdc_queue *q)
{
        assert(q != NULL);

        return q->table->front(q->container);
}


/**
 * @brief Returns point to the last element in the queue. This function assumes
 * that the queue isn't empty.
 */
static inline void *cdc_queue_back(struct cdc_queue *q)
{
        assert(q != NULL);

        return q->table->back(q->container);
}

// Capacity
/**
 * @brief Returns true if the queue has size 0; otherwise returns false.
 */
static inline bool cdc_queue_empty(struct cdc_queue *q)
{
        assert(q != NULL);

        return q->table->empty(q->container);
}

/**
 * @brief Returns the number of items in the queue.
 */
static inline size_t cdc_queue_size(struct cdc_queue *q)
{
        assert(q != NULL);

        return q->table->size(q->container);
}

// Modifiers
/**
 * @brief Adds value elem to the tail of the queue. Returned CDC_STATUS_OK in a
 * successful case or an excellent value indicating an error
 */
static inline enum cdc_stat cdc_queue_push(struct cdc_queue *q, void *elem)
{
        assert(q != NULL);

        return q->table->push(q->container, elem);
}

/**
 * @brief Removes the head item in the queue.
 * This function assumes that the queue isn't empty. Returned CDC_STATUS_OK in
 * a successful case or an excellent value indicating an error
 */
static inline enum cdc_stat cdc_queue_pop(struct cdc_queue *q)
{
        assert(q != NULL);

        return q->table->pop(q->container);
}

/**
 * @brief Swaps queues a and b. This operation is very fast and never fails.
 */
void cdc_queue_swap(struct cdc_queue *a, struct cdc_queue *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_queue queue_t;

#define queue_ctor(...)   cdc_queue_ctor(__VA_ARGS__)
#define queue_ctorl(...)  cdc_queue_ctorl(__VA_ARGS__)
#define queue_ctorv(...)  cdc_queue_ctorv(__VA_ARGS__)
#define queue_dtor(...)   cdc_queue_dtor(__VA_ARGS__)

// Element access
#define queue_front(...)  cdc_queue_front(__VA_ARGS__)
#define queue_back(...)   cdc_queue_back(__VA_ARGS__)

// Capacity
#define queue_empty(...)  cdc_queue_empty(__VA_ARGS__)
#define queue_size(...)   cdc_queue_size(__VA_ARGS__)

// Modifiers
#define queue_push(...)   cdc_queue_push(__VA_ARGS__)
#define queue_pop(...)    cdc_queue_pop(__VA_ARGS__)
#define queue_swap(...)   cdc_queue_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUE_H
