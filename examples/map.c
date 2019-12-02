// The MIT License (MIT)
// Copyright (c) 2018 Maksim Andrianov
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

// Program to emulates the phone book.
#define CDC_USE_SHORT_NAMES
#include <cdcontainers/cdc.h>

#include <stdio.h>
#include <string.h>

int str_lt(const void *lhs, const void *rhs)
{
  return strcmp((const char *)lhs, (const char *)rhs) < 0;
}

int handle_error(map_t *phone_book)
{
  map_dtor(phone_book);
  return EXIT_FAILURE;
}

void print_phone_book(map_t *phone_book)
{
  map_iter_t iter = CDC_INIT_STRUCT;
  if (map_iter_ctor(phone_book, &iter) != CDC_STATUS_OK)
  {
    printf("Error map_iter_init.\n");
    return;
  }

  map_begin(phone_book, &iter);
  while (map_iter_has_next(&iter))
  {
    printf("%s - %s\n",(char *)map_iter_key(&iter), (char *)map_iter_value(&iter));
    map_iter_next(&iter);
  }
}

int main(int argc, char **argv)
{
  CDC_UNUSED(argc);
  CDC_UNUSED(argv);

  map_t *phone_book = NULL;
  data_info_t data_info = CDC_INIT_STRUCT;
  data_info.cmp = str_lt;

  pair_t Lilia_Friedman = {.first = "Lilia Friedman", .second = "(892) 670-4739"};
  pair_t Tariq_Beltran = {.first = "Tariq Beltran", .second = "(489) 600-7575"};
  pair_t Laiba_Juarez = {.first = "Laiba Juarez", .second = "(303) 885-5692"};
  pair_t Elliott_Mooney = {.first = "Elliott Mooney", .second = "(945) 616-4482"};

  // map based on avl tree
  if (map_ctorl(cdc_map_avl, &phone_book, &data_info, &Lilia_Friedman, &Tariq_Beltran,
                &Laiba_Juarez, &Elliott_Mooney, CDC_END) != CDC_STATUS_OK) {
    return EXIT_FAILURE;
  }

  printf("Phone book:\n");
  print_phone_book(phone_book);

  if (map_insert(phone_book, "Zak Byers", "(551) 396-1880",
                 NULL /* iterator */, NULL /* is_inserted */) != CDC_STATUS_OK) {
    return handle_error(phone_book);
  }

  printf("Phone book after adding Zak Byers:\n");
  print_phone_book(phone_book);

  if (map_count(phone_book, "Tariq Beltran") != 0)
    map_erase(phone_book, "Tariq Beltran");

  printf("Phone book after erasing Tariq Beltran:\n");
  print_phone_book(phone_book);

  if (map_insert_or_assign(phone_book, "Zak Byers", "(555) 396-188",
                           NULL /* iterator */, NULL /* is_inserted */) != CDC_STATUS_OK) {
    return handle_error(phone_book);
  }

  printf("Phone book after update phone of Zak Byers:\n");
  print_phone_book(phone_book);
  return EXIT_SUCCESS;
}
