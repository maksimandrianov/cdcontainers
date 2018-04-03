# cdcontainers - data containers for C

Library of data containers for C programming language. The cdcontainers interface is similar to C ++ STL.
The library contains the following data containers:
* cdc_vector - dynamic array implementation
* cdc_list - doubly linked list implementation
* cdc_deque - double-ended queue implementation
* cdc_heap - binary heap implementation
* cdc_binomial_heap - binomial heap implementation 
* cdc_pairing_heap - pairing heap implementation 
* cdc_hash_table - hash table with collisions resolved by chaining implementation
* cdc_avl_tree - avl tree implementation
* cdc_splay_tree - splay tree implementation
* cdc_treep - сartesian tree implementation

and following interfaces:
* cdc_stack (Can work with: cdc_vector, cdc_list, cdc_deque)
* cdc_queue (Can work with: cdc_vector, cdc_list, cdc_deque)
* cdc_proirity_queue (Can work with: cdc_heap, cdc_binomial_heap, cdc_pairing_heap)
* cdc_map (Can work with: cdc_avl_tree, cdc_splay_tree, cdc_treep)

### Usage

Example of using a vector from a library cdcontainers:

```c
#define CDC_USE_SHORT_NAMES  // for short names (functions and structs without prefix cdc_*)
#include <cdcontainers/vector.h>
#include <cdcontainers/casts.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    vector_t *v;
    size_t i;

    if (vector_ctor(&v, NULL) != CDC_STATUS_OK)
        /* error handling */;

    if (vector_push_back(v, CDC_INT_TO_PTR(7)) != CDC_STATUS_OK)
        /* error handling */;

    if (vector_push_back(v, CDC_INT_TO_PTR(8)) != CDC_STATUS_OK)
        /* error handling */;

    for (i = 0; i < vector_size(v); ++i)
        printf("%i ", CDC_PTR_TO_INT(vector_get(v, i)));

    printf("\n");

    vector_dtor(v);
    
    return 0;
}
```

The output of this program is the following

    $ 7 8

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


