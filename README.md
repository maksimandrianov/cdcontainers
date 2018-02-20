# cdcontainers - data containers and collections for C

Library of data containers and collections for C programming language. The cdcontainers interface is similar to C ++ STL.
The library contains the following data containers and collections:
* vector
* list
* deque
* heap
* stack (based on vector or list)
* queue (based on deque or list)
* proirity_queue (based on heap)

### Usage

Example of using a vector from a library cdcontainers:

```c
#define CDC_USE_SHORT_NAMES  // for short names (functions and structs without prefix cdc_*)
#include <cdcontainers/vector.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    vector_t *v;
    int a = 7, b = 8, i;

    if (vector_ctor(&v, NULL) != CDC_STATUS_OK)
        /* error handling */;

    if (vector_push_back(v, &a) != CDC_STATUS_OK)
        /* error handling */;

    if (vector_push_back(v, &b) != CDC_STATUS_OK)
        /* error handling */;

    for (i = 0; i < vector_size(v); ++i)
        printf("%i ", *((int *)vector_get(v, i)));

    printf("\n");

    vector_dtor(v);
}
```

The output of this program is the following

    $ 7 8

### Installation - Unix

To build and install cdcontainers from source, the following tools are needed:
* make
* cmake
* gcc
* cunit (for testing)

On Ubuntu, you can install them with:

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


