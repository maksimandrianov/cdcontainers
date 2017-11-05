#ifndef CDCONTAINERS_INCLUDE_STATUS_H
#define CDCONTAINERS_INCLUDE_STATUS_H


enum cdc_stat {
        cdc_STATUS_OK = 0,
        cdc_STATUS_BAD_ALLOC,
        cdc_STATUS_OUT_OF_RANGE,
        cdc_STATUS_OVERFLOW,

        cdc_STATUS_UNKN
};

void cdc_print_stat(enum cdc_stat s);

#endif  // CDSTRUCTURES_INCLUDE_STATUS_H
