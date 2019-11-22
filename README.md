# cdcontainers - data containers and data structures for C

[![Build Status](https://travis-ci.org/maksimandrianov/cdcontainers.svg?branch=master)](https://travis-ci.org/maksimandrianov/cdcontainers)

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
// cdc_map_t example usage:
#define CDC_USE_SHORT_NAMES  // for short names (functions and structs without prefix cdc_*)
#include <cdcontainers/map.h>
#include <cdcontainers/casts.h>
#include <stdio.h>

static int lt_int(const void *l, const void *r)
{
  return l < r;
}

int main(int argc, char** argv)
{
  map_t *m = NULL;

  map_ctor1(cdc_map_avl, &m, NULL /* info */, lt_int);
  for (int i = 1; i < 10; ++i)
    map_insert(m, CDC_FROM_INT(i), CDC_FROM_INT(i * 10), NULL /* it */, NULL /* inserted */);

  map_iter_t it1;
  map_iter_t it2;

  map_iter_init(m, &it1);
  map_iter_init(m, &it2);

  map_begin(m, &it1);
  map_end(m, &it2);

  while (!map_iter_is_eq(&it1, &it2)) {
    printf("%i: %i\n", CDC_TO_INT(map_iter_key(&it1)), CDC_TO_INT(map_iter_value(&it1)));
    map_iter_next(&it1);
  }

  printf("\n");

  map_find(m, CDC_INT_TO(4), &it1);
  printf("Found %i: %i\n", CDC_TO_INT(map_iter_key(&it1)), CDC_TO_INT(map_iter_value(&it1)));

  map_iter_free(&it1);
  map_iter_free(&it2);
  map_dtor(m);
  return 0;
}
```
The output of this program is the following:
```
1: 10
2: 20
3: 30
4: 40
5: 50
6: 60
7: 70
8: 80
9: 90

Found 4: 40
```

```c
// cdc_array_t example usage:
#define CDC_USE_SHORT_NAMES  // for short names (functions and structs without prefix cdc_*)
#include <cdcontainers/array.h>
#include <cdcontainers/casts.h>
#include <stdio.h>

int main(int argc, char** argv)
{
  array_t *v = NULL;
  array_ctor(&v, NULL /* info */);

  array_push_back(v, CDC_FROM_INT(7));
  array_push_back(v, CDC_FROM_INT(8));

  for (size_t i = 0; i < array_size(v); ++i)
    printf("%i ", CDC_TO_INT(array_get(v, i)));

  printf("\n");

  array_dtor(v);
  return 0;
}
```

The output of this program is the following:
```
7 8
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


