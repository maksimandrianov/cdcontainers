#ifndef CDCONTAINERS_INCLUDE_STATUS_H
#define CDCONTAINERS_INCLUDE_STATUS_H


enum cdc_stat {
        CDC_STATUS_OK = 0,
        CDC_STATUS_BAD_ALLOC,
        CDC_STATUS_OUT_OF_RANGE,
        CDC_STATUS_OVERFLOW,

        CDC_STATUS_UNKN
};

void cdc_print_stat(enum cdc_stat s);

#endif  // CDSTRUCTURES_INCLUDE_STATUS_H
