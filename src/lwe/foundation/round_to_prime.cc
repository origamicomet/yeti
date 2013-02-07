// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#include <lwe/foundation.h>

static const lwe_size_t _primes[] =  {
  8 + 3,
  16 + 3,
  32 + 5,
  64 + 3,
  128 + 3,
  256 + 27,
  512 + 9,
  1024 + 9,
  2048 + 5,
  4096 + 3,
  8192 + 27,
  16384 + 43,
  32768 + 3,
  65536 + 45,
  131072 + 29,
  262144 + 3,
  524288 + 21,
  1048576 + 7,
  2097152 + 17,
  4194304 + 15,
  8388608 + 9,
  16777216 + 43,
  33554432 + 35,
  67108864 + 15,
  134217728 + 29,
  268435456 + 3,
  536870912 + 11,
  1073741824 + 85,
};

lwe_size_t _lwe_round_to_prime( lwe_size_t n )
{
  static const lwe_size_t num_primes = sizeof(_primes) / sizeof(lwe_size_t);

  for (lwe_size_t i = 0, new_n = 8; i < num_primes; i++, new_n <<= 1)
    if (new_n > n)
      return _primes[i];

  return 0xFFFFFFFFu;
}