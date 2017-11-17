/**
  * @file
  * @author Maksim Andrianov <maksimandrianov1@yandex.ru>
  * @brief The cdc_stackv is a struct and functions that provides stack
  * and based on vector
  */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKV_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKV_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <cdcontainers/status.h>
#include <cdcontainers/common.h>
#include <cdcontainers/vector.h>

/**
 * @brief The cdc_stackv struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_stackv {
        struct cdc_vector *vector;
};

/**
 * @brief Constructs an empty stack.
 * Returned CDC_STATUS_OK in a successful case or an excellent value
 * indicating an error
 */
enum cdc_stat cdc_stackv_ctor(struct cdc_stackv **s,
                              struct cdc_data_info *info);

/**
 * @brief Constructs a stack, initialized by an arbitrary number of pointers.
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_stackv_ctorl(struct cdc_stackv **s,
                               struct cdc_data_info *info, ...);

/**
 * @brief Constructs a stack, initialized by args
 * The last item must be NULL. Returned CDC_STATUS_OK in a successful case
 * or an excellent value indicating an error
 */
enum cdc_stat cdc_stackv_ctorv(struct cdc_stackv **s,
                               struct cdc_data_info *info, va_list args);

/**
 * @brief Destroys the vector.
 */
void cdc_stackv_dtor(struct cdc_stackv *s);

// Element access
/**
 * @brief Returns a pointer to the stack's top item. This function assumes
 * that the stack isn't empty.
 */
static inline void *cdc_stackv_top(struct cdc_stackv *s)
{
        assert(s != NULL);
        assert(cdc_vector_size(s->vector) > 0);

        return cdc_vector_back(s->vector);
}

// Capacity
/**
 * @brief Returns true if the stack has size 0; otherwise returns false.
 */
static inline bool cdc_stackv_empty(struct cdc_stackv *s)
{
      assert(s != NULL);

      return cdc_vector_empty(s->vector);
}

/**
 * @brief Returns the number of items in the stack.
 */
static inline size_t cdc_stackv_size(struct cdc_stackv *s)
{
        assert(s != NULL);

        return cdc_vector_size(s->vector);
}

// Modifiers
/**
 * @brief Adds element t to the top of the stack. Returned CDC_STATUS_OK in a
 * successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_stackv_push(struct cdc_stackv *s, void *elem);

/**
 * @brief Removes the top item from the stack.
 * This function assumes that the stack isn't empty. Returned CDC_STATUS_OK in
 * a successful case or an excellent value indicating an error
 */
enum cdc_stat cdc_stackv_pop(struct cdc_stackv *s);

/**
 * @brief Swaps stack a and b. This operation is very fast and never fails.
 */
void cdc_stackv_swap(struct cdc_stackv *a, struct cdc_stackv *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_stackv stackv_t;

#define stackv_ctor(...)   cdc_stackv_ctor(__VA_ARGS__)
#define stackv_ctorl(...)  cdc_stackv_ctorl(__VA_ARGS__)
#define stackv_ctorv(...)  cdc_stackv_ctorv(__VA_ARGS__)
#define stackv_dtor(...)   cdc_stackv_dtor(__VA_ARGS__)

// Element access
#define stackv_top(...)    struct cdc_stackvop(__VA_ARGS__)

// Capacity
#define stackv_empty(...)  cdc_stackv_empty(__VA_ARGS__)
#define stackv_size(...)   cdc_stackv_size(__VA_ARGS__)

// Modifiers
#define stackv_push(...)   cdc_stackv_push(__VA_ARGS__)
#define stackv_pop(...)    cdc_stackv_pop(__VA_ARGS__)
#define stackv_swap(...)   cdc_stackv_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_STACKV_H
