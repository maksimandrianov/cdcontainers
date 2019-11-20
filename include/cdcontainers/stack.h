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
/**
 * @file
 * @author Maksim Andrianov <maksimandrianov1@yandex.ru>
 * @brief The cdc_stack is a struct and functions that provide a stack
 */
#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_STACK_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_STACK_H

#include <cdcontainers/common.h>
#include <cdcontainers/interfaces/isequence.h>
#include <cdcontainers/status.h>

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief The cdc_stack struct
 * @warning To avoid problems, do not change the structure fields in the code.
 * Use only special functions to access and change structure fields.
 */
struct cdc_stack {
  void *container;
  const struct cdc_sequence_table *table;
};

/**
 * @brief Constructs an empty stack.
 * @param table - method table for a particular implementation
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_stack_ctor(const struct cdc_sequence_table *table,
                             struct cdc_stack **s, struct cdc_data_info *info);

/**
 * @brief Constructs a stack, initialized by an arbitrary number of pointers.
 * The last item must be NULL.
 * @param table - method table for a particular implementation
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_stack_ctorl(const struct cdc_sequence_table *table,
                              struct cdc_stack **s, struct cdc_data_info *info,
                              ...);

/**
 * @brief Constructs a stack, initialized by args. The last item must be NULL.
 * @param table - method table for a particular implementation
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_stack_ctorv(const struct cdc_sequence_table *table,
                              struct cdc_stack **s, struct cdc_data_info *info,
                              va_list args);

/**
 * @brief Constructs an empty stack based on list.
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_stackl_ctor(struct cdc_stack **s,
                                            struct cdc_data_info *info)
{
  assert(s != NULL);

  return cdc_stack_ctor(cdc_seq_list, s, info);
}

/**
 * @brief Constructs a stack based on list, initialized by an arbitrary number
 * of pointers. The last item must be NULL.
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_stackl_ctorl(struct cdc_stack **s, struct cdc_data_info *info,
                               ...);

/**
 * @brief Constructs a stack based on list, initialized by args
 * The last item must be NULL.
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_stackl_ctorv(struct cdc_stack **s,
                                             struct cdc_data_info *info,
                                             va_list args)
{
  assert(s != NULL);

  return cdc_stack_ctorv(cdc_seq_list, s, info, args);
}

/**
 * @brief Constructs an empty stack based on vector.
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_stackv_ctor(struct cdc_stack **s,
                                            struct cdc_data_info *info)
{
  assert(s != NULL);

  return cdc_stack_ctor(cdc_seq_vector, s, info);
}

/**
 * @brief Constructs a stack based on vector, initialized by an arbitrary number
 * of pointers. The last item must be NULL.
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
enum cdc_stat cdc_stackv_ctorl(struct cdc_stack **s, struct cdc_data_info *info,
                               ...);

/**
 * @brief Constructs a stack based on vector, initialized by args. The last item
 * must be NULL.
 * @param s - cdc_stack
 * @param info - cdc_data_info
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_stackv_ctorv(struct cdc_stack **s,
                                             struct cdc_data_info *info,
                                             va_list args)
{
  assert(s != NULL);

  return cdc_stack_ctorv(cdc_seq_vector, s, info, args);
}

/**
 * @brief Destroys the stack.
 * @param s - cdc_stack
 */
void cdc_stack_dtor(struct cdc_stack *s);

// Element access
/**
 * @brief Returns a pointer to the stack's top item. This function assumes
 * that the stack isn't empty.
 * @param s - cdc_stack
 * @return pointer to the stack's top item
 */
static inline void *cdc_stack_top(struct cdc_stack *s)
{
  assert(s != NULL);

  return s->table->back(s->container);
}

// Capacity
/**
 * @brief Returns true if the stack has size 0; otherwise returns false.
 * @param s - cdc_stack
 * @return true if the list has size 0; otherwise returns false
 */
static inline bool cdc_stack_empty(struct cdc_stack *s)
{
  assert(s != NULL);

  return s->table->empty(s->container);
}

/**
 * @brief Returns the number of items in the stack.
 * @param s - cdc_stack
 * @return size
 */
static inline size_t cdc_stack_size(struct cdc_stack *s)
{
  assert(s != NULL);

  return s->table->size(s->container);
}

// Modifiers
/**
 * @brief Adds element elem to the top of the stack.
 * @param s - cdc_stack
 * @param elem
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_stack_push(struct cdc_stack *s, void *elem)
{
  assert(s != NULL);

  return s->table->push_back(s->container, elem);
}

/**
 * @brief Removes the top item from the stack. This function assumes that the
 * stack isn't empty.
 * @param s - cdc_stack
 * @return CDC_STATUS_OK in a successful case or an excellent value indicating
 * an error
 */
static inline enum cdc_stat cdc_stack_pop(struct cdc_stack *s)
{
  assert(s != NULL);

  return s->table->pop_back(s->container);
}

/**
 * @brief Swaps stack a and b. This operation is very fast and never fails.
 * @param a - cdc_stack
 * @param b - cdc_stack
 */
void cdc_stack_swap(struct cdc_stack *a, struct cdc_stack *b);

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef struct cdc_stack stack_t;

#define stack_ctor(...) cdc_stack_ctor(__VA_ARGS__)
#define stack_ctorl(...) cdc_stack_ctorl(__VA_ARGS__)
#define stack_ctorv(...) cdc_stack_ctorv(__VA_ARGS__)
#define stack_dtor(...) cdc_stack_dtor(__VA_ARGS__)

// Element access
#define stack_top(...) cdc_stack_top(__VA_ARGS__)

// Capacity
#define stack_empty(...) cdc_stack_empty(__VA_ARGS__)
#define stack_size(...) cdc_stack_size(__VA_ARGS__)

// Modifiers
#define stack_push(...) cdc_stack_push(__VA_ARGS__)
#define stack_pop(...) cdc_stack_pop(__VA_ARGS__)
#define stack_swap(...) cdc_stack_swap(__VA_ARGS__)
#endif

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_STACK_H
