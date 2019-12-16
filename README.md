# [cdcontainers](https://maksimandrianov.github.io/cdcontainers.github.io/) - data containers and data structures for C

![](https://repository-images.githubusercontent.com/109067289/e3459080-0fac-11ea-8f62-94d4455f0e53)

[![Build Status](https://travis-ci.org/maksimandrianov/cdcontainers.svg?branch=master)](https://travis-ci.org/maksimandrianov/cdcontainers)
[![Documentation](https://img.shields.io/badge/api-reference-blue.svg)](https://maksimandrianov.github.io/cdcontainers.github.io/)

Library of data containers and data structures for C programming language. The cdcontainers interface is similar to C ++ STL.
The library contains the following data containers:
* cdc_array - dynamic array
* cdc_list - doubly linked list
* cdc_circular_array - circular array
* cdc_heap - binary heap
* cdc_binomial_heap - binomial heap 
* cdc_pairing_heap - pairing heap 
* cdc_hash_table - hash table with collisions resolved by chaining
* cdc_avl_tree - avl tree
* cdc_splay_tree - splay tree
* cdc_treap - сartesian tree

and following adapters:
* cdc_deque (Can work with: cdc_array, cdc_list, cdc_circular_array)
* cdc_stack (Can work with: cdc_array, cdc_list, cdc_circular_array)
* cdc_queue (Can work with: cdc_array, cdc_list, cdc_circular_array)
* cdc_priority_queue (Can work with: cdc_heap, cdc_binomial_heap, cdc_pairing_heap)
* cdc_map (Can work with: cdc_avl_tree, cdc_splay_tree, cdc_treap, cdc_hash_table)

Example:
```c
#define CDC_USE_SHORT_NAMES  // for short names (functions and structs without prefix cdc_*)
#include <cdcontainers/cdc.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    array_t *array = NULL;

    if (array_ctor(&array, NULL) != CDC_STATUS_OK)
        /* error handling */;

    if (array_push_back(array, CDC_FROM_INT(8)) != CDC_STATUS_OK)
        /* error handling */;

    if (array_push_back(array, CDC_FROM_INT(2)) != CDC_STATUS_OK)
        /* error handling */;

    for (sizr_t i = 0; i < array_size(array); ++i)
        printf("%i\n", CDC_TO_INT(vector_get(v, i)));
        
    array_dtor(array);
}
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
    $ mkdir build && cd build
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

### Usage
You can find examples in the directory `examples`.

To run the example, compile and link against the cdcontainers library (libcdcontainers.a/.so). If you followed the build steps above, this library will be under the build directory you created.
```sh
# Example on linux after running the build steps above. Assumes the
# `cdcontainers` and `build` directories are under the current directory.
$ gcc examples/map.c -isystem include -Llib -lcdcontainers -o map
```

```
./map
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
