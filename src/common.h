#ifndef CDSTRUCTURES_SRC_COMMON_H
#define CDSTRUCTURES_SRC_COMMON_H

#include <stdlib.h>

#define CDS_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CDS_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CDS_SWAP(T, x, y) do \
        { \
                T tmp = x;  \
                x = y; \
                y = tmp; \
        } while (0)



#endif  // CDSTRUCTURES_CDSTRUCTURES_COMMON_H
