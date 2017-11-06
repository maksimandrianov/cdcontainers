# cdcontainers
Library of data containers and collections for C programming language.
The library contains the following data containers and collections:
* vector
* list
* stack (based on vector - stackv or list - stackl)
* queue (based on list - queuel)

# Building

Packages needed for the build, or the build requires:
* make
* cmake
* gcc
* cunit (for testing)

To build cdstructures, do the following steps:

    $ git clone https://github.com/maksimandrianov/cdcontainers
    $ cd cdcontainers
    $ mkdir cmake && cd cmake
    $ cmake ..
    $ make

# Testing

To build and run tests, do the following steps:

    $ make tests
    $ make check
