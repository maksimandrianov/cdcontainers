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
#include <cdcontainers/array.h>
#include <cdcontainers/casts.h>
#include <cdcontainers/global.h>

#include <stdio.h>
#include <stdlib.h>

void exit_wiht_stat(enum cdc_stat s)
{
  cdc_print_stat(s);
  exit(EXIT_FAILURE);
}

void print_int_array(array_t *v)
{
  for (size_t i = 0; i < array_size(v); ++i) {
    printf("%i\n", CDC_TO_INT(array_get(v, i)));
  }
}

int main(int argc, char **argv)
{
  CDC_UNUSED(argc);
  CDC_UNUSED(argv);

  array_t *v = NULL;
  array_t *w = NULL;

  enum cdc_stat ret;
  if ((ret = cdc_array_ctor(&v, NULL)) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  int a = 0, b = 1, c = 2, d = 4, f = 5;
  if ((ret = array_push_back(v, CDC_FROM_INT(a))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }
  if ((ret = array_push_back(v, CDC_FROM_INT(b))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }
  if ((ret = array_push_back(v, CDC_FROM_INT(c))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }
  if ((ret = array_push_back(v, CDC_FROM_INT(d))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }
  if ((ret = array_push_back(v, CDC_FROM_INT(f))) != CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  print_int_array(v);
  while (!array_empty(v)) {
    array_pop_back(v);
  }

  array_dtor(v);
  if ((ret = array_ctorl(&v, NULL, CDC_TO_INT(a), CDC_TO_INT(b), CDC_TO_INT(c),
                         CDC_TO_INT(d), CDC_TO_INT(f), CDC_END)) !=
      CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  print_int_array(v);
  array_dtor(v);
  if ((ret = array_ctorl(&v, NULL, CDC_TO_INT(c), CDC_TO_INT(d), CDC_END)) !=
      CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  if ((ret = array_ctorl(&w, NULL, CDC_TO_INT(a), CDC_TO_INT(b), CDC_END)) !=
      CDC_STATUS_OK) {
    exit_wiht_stat(ret);
  }

  array_swap(v, w);
  print_int_array(v);
  print_int_array(w);
  array_dtor(v);
  array_dtor(w);
  return 0;
}
