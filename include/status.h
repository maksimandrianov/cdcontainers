#ifndef CDSTRUCTURES_INCLUDE_STATUS_H
#define CDSTRUCTURES_INCLUDE_STATUS_H


enum cds_stat {
        CDS_STATUS_OK = 0,
        CDS_STATUS_BAD_ALLOC,
        CDS_STATUS_OUT_OF_RANGE,
        CDS_STATUS_OVERFLOW,

        CDS_STATUS_UNKN
};

void cds_print_stat(enum cds_stat s);

#endif // CDSTRUCTURES_INCLUDE_STATUS_H
