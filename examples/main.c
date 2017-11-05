#include "cdstructures.h"
#include <stdio.h>


void exit_wiht_stat(enum cdc_stat s)
{
        cdc_print_stat(s);
        exit(EXIT_FAILURE);
}

void print_int_vector(cdc_vector_t *v)
{
        int i;

        for (i = 0; i < cdc_vector_size(v); ++i)
                printf("%i\n", *((int*)cdc_vector_get(v, i)));
}

int main(int argc, char** argv)
{

        cdc_vector_t *v, *w;
        enum cdc_stat ret;

        if ((ret = cdc_vector_ctor(&v, NULL)) != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        int a = 0, b = 1, c = 2, d = 4, f = 5;

        if ((ret = cdc_vector_push_back(v, &a)) != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cdc_vector_push_back(v, &b)) != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cdc_vector_push_back(v, &c)) != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cdc_vector_push_back(v, &d)) != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cdc_vector_push_back(v, &f)) != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        print_int_vector(v);

        while (!cdc_vector_empty(v))
                cdc_vector_pop_back(v);

        cdc_vector_dtor(v);


        if ((ret = cdc_vector_ctor_l(&v, NULL, &a, &b, &c, &d, &f, NULL))
                        != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        print_int_vector(v);

        cdc_vector_dtor(v);


        if ((ret = cdc_vector_ctor_l(&v, NULL, &c, &d, NULL))
                        != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cdc_vector_ctor_l(&w, NULL, &a, &b, NULL))
                        != CDC_STATUS_OK)
                exit_wiht_stat(ret);


        cdc_vector_swap(v, w);

        print_int_vector(v);
        print_int_vector(w);

        cdc_vector_dtor(v);
        cdc_vector_dtor(w);

}
