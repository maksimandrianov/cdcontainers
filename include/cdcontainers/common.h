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
#define CDC_INIT_STRUCT {0,}

struct cdc_data_info {
        void (*free)(void *);
        bool (*lt)(const void *, const void *);
        bool (*gt)(const void *, const void *);
        bool (*eq)(const void *, const void *);
        size_t size;
        size_t __cnt;
};

typedef int (*cdc_compar_func_t)(const void *, const void *);


#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H
