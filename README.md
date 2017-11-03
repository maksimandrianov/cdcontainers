# cdstructures
Library of data structures and collections in C programming language.
The library contains the following data structures and collections:
* vector

# Building

Packages needed for the build, or the build requires:
* make
* cmake
* gcc
* cunit (for testing)

To build cdstructures, do the following steps:

    $ git clone https://github.com/maksimandrianov/cdstructures
    $ cd cdstructures
    $ mkdir cmake && cd cmake
    $ cmake ..
    $ make

# Testing

To build and run tests, do the following steps:

    $ make tests
    $ make check
