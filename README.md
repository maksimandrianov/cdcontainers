# [cdcontainers](https://maksimandrianov.github.io/cdcontainers.github.io/) - data containers and data structures for C

![](https://repository-images.githubusercontent.com/109067289/e3459080-0fac-11ea-8f62-94d4455f0e53)

[![Build Status](https://travis-ci.org/maksimandrianov/cdcontainers.svg?branch=master)](https://travis-ci.org/maksimandrianov/cdcontainers)
[![Documentation](https://img.shields.io/badge/api-reference-blue.svg)](https://maksimandrianov.github.io/cdcontainers.github.io/)

Library of data containers and data structures for C programming language. The cdcontainers interface is similar to C ++ STL.
The library contains the following data containers:
* cdc_array - dynamic array
* cdc_list - doubly linked list
* cdc_deque - double-ended queue
* cdc_heap - binary heap
* cdc_binomial_heap - binomial heap 
* cdc_pairing_heap - pairing heap 
* cdc_hash_table - hash table with collisions resolved by chaining
* cdc_avl_tree - avl tree
* cdc_splay_tree - splay tree
* cdc_treep - сartesian tree

and following interfaces:
* cdc_stack (Can work with: cdc_array, cdc_list, cdc_deque)
* cdc_queue (Can work with: cdc_array, cdc_list, cdc_deque)
* cdc_proirity_queue (Can work with: cdc_heap, cdc_binomial_heap, cdc_pairing_heap)
* cdc_map (Can work with: cdc_avl_tree, cdc_splay_tree, cdc_treep)

### Usage

Example of using a array from a library cdcontainers:

```c
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
  if (map_iter_init(phone_book, &iter) != CDC_STATUS_OK)
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
```
The output of this program is the following:
```
Phone book:
Elliott Mooney - (945) 616-4482
Laiba Juarez - (303) 885-5692
Lilia Friedman - (892) 670-4739
Tariq Beltran - (489) 600-7575
Phone book after adding Zak Byers:
Elliott Mooney - (945) 616-4482
Laiba Juarez - (303) 885-5692
Lilia Friedman - (892) 670-4739
Tariq Beltran - (489) 600-7575
Zak Byers - (551) 396-1880
Phone book after erasing Tariq Beltran:
Elliott Mooney - (945) 616-4482
Laiba Juarez - (303) 885-5692
Lilia Friedman - (892) 670-4739
Zak Byers - (551) 396-1880
Phone book after update phone of Zak Byers:
Elliott Mooney - (945) 616-4482
Laiba Juarez - (303) 885-5692
Lilia Friedman - (892) 670-4739
Zak Byers - (555) 396-188
```

### Installation - Unix

To build and install cdcontainers from source, the following tools are needed:
* make
* cmake
* gcc (or your other favorite compiler with minimal support ISO/IEC 9899:1999 (C99) standard)
* cunit (for testing)

On Ubuntu or Debian, you can install them with:

    $  sudo apt-get install cmake gcc make libcunit1-dev

To build and install cdstructures, do the following steps:

    $ git clone https://github.com/maksimandrianov/cdcontainers
    $ cd cdcontainers
    $ mkdir cmake && cd cmake
    $ cmake ..
    $ make
    $ sudo make install

#### Hint on install location

By default, the package will be installed to /usr/local. However, on many platforms, /usr/local/lib is not part of LD_LIBRARY_PATH. You can add it, but it may be easier to just install to /usr instead. To do this, invoke cmake as follows:

    $ cmake -DCMAKE_INSTALL_PREFIX=/usr ..

### Testing

To build and run tests, do the following steps:

    $ make tests
    $ make check


