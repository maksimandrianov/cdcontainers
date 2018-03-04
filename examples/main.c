// The MIT License (MIT)
// Copyright (c) 2017 Maksim Andrianov
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
#include <stdio.h>
#include <stdlib.h>
#include <cdcontainers/vector.h>

void exit_wiht_stat(enum cdc_stat s)
{
        cdc_print_stat(s);
        exit(EXIT_FAILURE);
}

void print_int_vector(struct cdc_vector *v)
{
        int i;

        for (i = 0; i < cdc_vector_size(v); ++i)
                printf("%i\n", *((int*)cdc_vector_get(v, i)));
}

int main(int argc, char** argv)
{

        struct cdc_vector *v, *w;
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


        if ((ret = cdc_vector_ctorl(&v, NULL, &a, &b, &c, &d, &f, NULL))
                        != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        print_int_vector(v);

        cdc_vector_dtor(v);


        if ((ret = cdc_vector_ctorl(&v, NULL, &c, &d, NULL))
                        != CDC_STATUS_OK)
                exit_wiht_stat(ret);

        if ((ret = cdc_vector_ctorl(&w, NULL, &a, &b, NULL))
                        != CDC_STATUS_OK)
                exit_wiht_stat(ret);


        cdc_vector_swap(v, w);

        print_int_vector(v);
        print_int_vector(w);

        cdc_vector_dtor(v);
        cdc_vector_dtor(w);

}
