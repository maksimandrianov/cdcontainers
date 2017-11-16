#ifndef CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H
#define CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H

#define CDC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CDC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CDC_SWAP(T, x, y) do \
        { \
                T tmp = x;  \
                x = y; \
                y = tmp; \
        } while (0)

typedef void (*cdc_free_func_t)(void *);
typedef int (*cdc_compar_func_t)(const void *, const void *);

#endif  // CDCONTAINERS_INCLUDE_CDCONTAINERS_COMMON_H
