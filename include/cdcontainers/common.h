#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H

#include <stdbool.h>
#include <stddef.h>

#define CDC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CDC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CDC_SWAP(T, x, y) do \
        { \
                T tmp = x;  \
                x = y; \
                y = tmp; \
        } while (0)

typedef void (*cdc_free_func_t)(void *);
typedef bool (*cdc_lt_func_t)(const void *, const void *);
typedef bool (*cdc_gt_func_t)(const void *, const void *);
typedef bool (*cdc_eq_func_t)(const void *, const void *);

struct cdc_data_info {
        cdc_free_func_t free;
        cdc_lt_func_t lt;
        cdc_gt_func_t gt;
        cdc_eq_func_t eq;
        size_t size;
};

#define CDC_HAS_FREE(container) (container->info && container->info->free)
#define CDC_HAS_LT(container) (container->info && container->info->lt)
#define CDC_HAS_GT(container) (container->info && container->info->gt)
#define CDC_HAS_EQ(container) (container->info && container->info->eq)
#define CDC_HAS_SIZE(container) (container->info && container->info->size > 0)

typedef int (*cdc_compar_func_t)(const void *, const void *);


#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H
