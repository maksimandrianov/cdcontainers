#include "status.h"
#include "stdio.h"

void cdc_print_stat(enum cdc_stat s)
{
        static const char *desscriptions[] = {
                "cdc_STATUS_OK",
                "cdc_STATUS_BAD_ALLOC",
                "cdc_STATUS_OUT_OF_RANGE",
                "cdc_STATUS_OVERFLOW",

                "cdc_STATUS_UNKN"
        };

        if (s < cdc_STATUS_OK && s > cdc_STATUS_UNKN)
                s = cdc_STATUS_UNKN;

        printf("%s", desscriptions[s]);
}
