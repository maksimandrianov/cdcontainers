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

// Program to find first n prime numbers.
#define CDC_USE_SHORT_NAMES
#include <cdcontainers/cdc.h>

#include <stdio.h>

// This function based on the Sieve of Eratosthenes.
// https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
array_t *find_uint_prime_numbers(unsigned int n)
{
  array_t *numbers = NULL;
  if (array_ctor(&numbers, NULL /* data info */) != CDC_STATUS_OK)
    return NULL;

  array_reserve(numbers, n + 1);
  for (unsigned int i = 0; i < n + 1; ++i)
    array_push_back(numbers, CDC_FROM_UINT(i));

  array_t *prime_numbers = NULL;
  if (array_ctor(&prime_numbers, NULL /* data info */) != CDC_STATUS_OK) {
    array_dtor(numbers);
    return NULL;
  }

  for (unsigned int p = 2; p < n + 1; ++p) {
    void *val = array_get(numbers, p);
    if (CDC_TO_UINT(val) != 0) {
      if (array_push_back(prime_numbers, val) != CDC_STATUS_OK) {
        array_dtor(prime_numbers);
        array_dtor(numbers);
        return NULL;
      }

      for (unsigned int i = p * p; i < n + 1; i += p)
        array_set(numbers, i, CDC_FROM_UINT(0));
    }
  }

  array_dtor(numbers);
  return prime_numbers;
}

int main(int argc, char **argv)
{
  CDC_UNUSED(argc);
  CDC_UNUSED(argv);

  array_t *prime_numbers = find_uint_prime_numbers(1000);
  if (!prime_numbers)
    return EXIT_FAILURE;

  for (unsigned i = 0; i < array_size(prime_numbers); ++i)
    printf("%u ", CDC_TO_UINT(array_get(prime_numbers, i)));

  printf("\n");
  array_dtor(prime_numbers);
  return EXIT_SUCCESS;
}
