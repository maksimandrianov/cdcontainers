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
#define CDC_USE_SHORT_NAMES
#include <cdcontainers/casts.h>
#include <cdcontainers/global.h>
#include <cdcontainers/vector.h>

#include <stdio.h>
#include <stdlib.h>

void exit_wiht_stat(enum cdc_stat s)
{
  cdc_print_stat(s);
  exit(EXIT_FAILURE);
}

void print_int_vector(vector_t *v)
{
  for (size_t i = 0; i < vector_size(v); ++i) {
    printf("%i\n", CDC_PTR_TO_INT(vector_get(v, i)));
  }
}

int main(int argc, char **argv)
{
  CDC_UNUSED(argc);
  CDC_UNUSED(argv);

  vector_t *v = NULL;
  vector_t *w = NULL;

  enum cdc_stat ret;
  if ((ret = cdc_vector_ctor(&v, NULL)) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  int a = 0, b = 1, c = 2, d = 4, f = 5;
  if ((ret = vector_push_back(v, CDC_INT_TO_PTR(a))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }
  if ((ret = vector_push_back(v, CDC_INT_TO_PTR(b))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }
  if ((ret = vector_push_back(v, CDC_INT_TO_PTR(c))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }
  if ((ret = vector_push_back(v, CDC_INT_TO_PTR(d))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }
  if ((ret = vector_push_back(v, CDC_INT_TO_PTR(f))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  print_int_vector(v);
  while (!vector_empty(v)) {
    vector_pop_back(v);
  }

  vector_dtor(v);
  if ((ret = vector_ctorl(&v, NULL, CDC_PTR_TO_INT(a), CDC_PTR_TO_INT(b),
                          CDC_PTR_TO_INT(c), CDC_PTR_TO_INT(d),
                          CDC_PTR_TO_INT(f), CDC_END)) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  print_int_vector(v);
  vector_dtor(v);
  if ((ret = vector_ctorl(&v, NULL, CDC_PTR_TO_INT(c), CDC_PTR_TO_INT(d),
                          CDC_END)) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  if ((ret = vector_ctorl(&w, NULL, CDC_PTR_TO_INT(a), CDC_PTR_TO_INT(b),
                          CDC_END)) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  vector_swap(v, w);
  print_int_vector(v);
  print_int_vector(w);
  vector_dtor(v);
  vector_dtor(w);
  return 0;
}
