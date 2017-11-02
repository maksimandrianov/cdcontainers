#include "cdstructures.h"
#include <stdio.h>


void exit_wiht_stat(enum cds_stat s)
{
        cds_print_stat(s);
        exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{

        cds_vector_t *v;
        enum cds_stat ret;
        int i;

        if ((ret = cds_vector_ctor(&v)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        int a = 0, b = 1, c = 2, d = 4, f = 5;

        if ((ret = cds_vector_push_back(v, &a)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cds_vector_push_back(v, &b)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cds_vector_push_back(v, &c)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cds_vector_push_back(v, &d)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cds_vector_push_back(v, &f)) != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        for (i = 0; i < cds_vector_size(v); ++i) {
                printf("%i\n", *((int*)cds_vector_get(v, i)));
        }

        while (!cds_vector_empty(v)) {
                cds_vector_pop_back(v);
        }

        cds_vector_dtor(v);


        if ((ret = cds_vector_ctor_list(&v, &a, &b, &c, &d, &f, NULL))
                        != CDS_STATUS_OK)
                exit_wiht_stat(ret);

        for (i = 0; i < cds_vector_size(v); ++i) {
                printf("%i\n", *((int*)cds_vector_get(v, i)));
        }

        cds_vector_dtor(v);

}
