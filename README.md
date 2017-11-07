# cdcontainers - data containers and collections for C

Library of data containers and collections for C programming language.
The library contains the following data containers and collections:
* vector
* list
* stack (based on vector - stackv or list - stackl)
* queue (based on list - queuel)

# Building & installing - Unix

Packages needed for the build, or the build requires:
* make
* cmake
* gcc
* cunit (for testing)

On Ubuntu, you can install them with:

    $  sudo apt-get install cmake gcc make libcunit1-dev

To build cdstructures, do the following steps:

    $ git clone https://github.com/maksimandrianov/cdcontainers
    $ cd cdcontainers
    $ mkdir cmake && cd cmake
    $ cmake ..
    $ make
    $ make install

## Hint on install location

By default, the package will be installed to /usr/local. However, on many platforms, /usr/local/lib is not part of LD_LIBRARY_PATH. You can add it, but it may be easier to just install to /usr instead. To do this, invoke cmake as follows:

    $ cmake -DCMAKE_INSTALL_PREFIX=/usr ..

# Testing

To build and run tests, do the following steps:

    $ make tests
    $ make check


