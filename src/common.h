#ifndef CDCONTAINERS_SRC_COMMON_H
#define CDCONTAINERS_SRC_COMMON_H

#include <stdlib.h>

#define CDC_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CDC_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CDC_SWAP(T, x, y) do \
        { \
                T tmp = x;  \
                x = y; \
                y = tmp; \
        } while (0)

#endif  // CDSTRUCTURES_CDSTRUCTURES_COMMON_H
