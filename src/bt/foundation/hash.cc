/*
 * This file is part of Butane.
 *
 * Copyright (c) Michael Williams <devbug@bitbyte.ca>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution. Neither the name of the nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* ========================================================================== */
    #include <bt/foundation/hash.h>
/* ========================================================================== */

#include <string.h>

/* ========================================================================== */
/*  MurmurHash2:                                                              */
/* ========================================================================== */

uint32_t bt_murmur_hash(const void *buf, const size_t buf_sz, const uint32_t seed) {
  // bt_assert(debug, buf != NULL);

  const uint32_t m = UINT32_C(0x5bd1e995);
  const uint32_t r = UINT32_C(24);

  const uint8_t *key = (const uint8_t *)buf;
  size_t key_len = buf_sz;
  uint32_t h = seed ^ key_len;

  while (key_len >= 4) {
    uint32_t k = *(const uint32_t *)key;
    k *= m;
    k ^= k >> r;
    k *= m;
    h *= m;
    h ^= k;
    key += 4;
    key_len -= 4;
  }

  switch (key_len) {
    case 3: h ^= key[2] << UINT32_C(16);
    case 2: h ^= key[1] << UINT32_C(8);
    case 1: h ^= key[0];
  }

  h *= m;
  h ^= h >> UINT32_C(13);
  h *= m;
  h ^= h >> UINT32_C(15);

  return h;
}

uint32_t bt_murmur_hash_str(const char *str, const uint32_t seed) {
  // bt_assert(debug, str != NULL);
  return bt_murmur_hash((const void *)str, strlen(str) + 1, seed);
}
