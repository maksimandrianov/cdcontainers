#include "cdstructures.h"
#include <stdio.h>


void exit_wiht_stat(enum cds_stat s)
{
        cds_print_stat(s);
        exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{

        struct cds_vector *v;
        enum cds_stat ret;
        int i;

        if ((ret = cds_vec_ctor(&v)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        int a = 0, b = 1, c = 2, d = 4, f = 5;

        if ((ret = cds_vec_push_back(v, &a)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cds_vec_push_back(v, &b)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cds_vec_push_back(v, &c)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cds_vec_push_back(v, &d)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cds_vec_push_back(v, &f)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        for (i = 0; i < cds_vec_size(v); ++i) {
                printf("%i\n", *((int*)cds_vec_get(v, i)));
        }

        cds_vec_dtor(v);

}
