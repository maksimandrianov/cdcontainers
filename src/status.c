#include "cdcontainers/status.h"

#include <stdio.h>

void cdc_print_stat(enum cdc_stat s)
{
        static const char *desscriptions[] = {
                "CDC_STATUS_OK",
                "CDC_STATUS_BAD_ALLOC",
                "CDC_STATUS_OUT_OF_RANGE",
                "CDC_STATUS_OVERFLOW",

                "CDC_STATUS_UNKN"
        };

        if (s < CDC_STATUS_OK && s > CDC_STATUS_UNKN)
                s = CDC_STATUS_UNKN;

        printf("%s", desscriptions[s]);
}
