#include "status.h"
#include "stdio.h"

void cds_print_stat(enum cds_stat s)
{
        static const char *desscriptions[] = {
                "CDS_STATUS_OK",
                "CDS_STATUS_BAD_ALLOC",
                "CDS_STATUS_OUT_OF_RANGE",
                "CDS_STATUS_OVERFLOW",

                "CDS_STATUS_UNKN"
        };

        if (s < CDS_STATUS_OK && s > CDS_STATUS_UNKN)
                s = CDS_STATUS_UNKN;

        printf("%s", desscriptions[s]);
}
