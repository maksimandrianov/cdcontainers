#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUE_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_QUEUE_H

#include <cdcontainers/queuel.h>
#include <cdcontainers/queued.h>

typedef struct cdc_queued_t cdc_queue_t;

#define cdc_queue_ctor(...)   cdc_queued_ctor(__VA_ARGS__)
#define cdc_queue_ctorl(...)  cdc_queued_ctorl(__VA_ARGS__)
#define cdc_queue_ctorv(...)  cdc_queued_ctorv(__VA_ARGS__)
#define cdc_queue_dtor(...)   cdc_queued_dtor(__VA_ARGS__)

// Element access
#define cdc_queue_front(...)  cdc_queued_front(__VA_ARGS__)
#define cdc_queue_back(...)   cdc_queued_back(__VA_ARGS__)

// Capacity
#define cdc_queue_empty(...)  cdc_queued_empty(__VA_ARGS__)
#define cdc_queue_size(...)   cdc_queued_size(__VA_ARGS__)

// Modifiers
#define cdc_queue_push(...)   cdc_queued_push(__VA_ARGS__)
#define cdc_queue_pop(...)    cdc_queued_pop(__VA_ARGS__)
#define cdc_queue_swap(...)   cdc_queued_swap(__VA_ARGS__)

// Short names
#ifdef CDC_USE_SHORT_NAMES
typedef cdc_queue_t queue_t;

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
