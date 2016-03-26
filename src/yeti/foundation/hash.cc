//===-- yeti/foundation/hash.cc ---------------------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/hash.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace yeti {
namespace foundation {

u64 murmur(const char *buf, u64 seed) {
  yeti_assert_debug(buf != NULL);
  return murmur((const uintptr_t)buf, strlen(buf), seed);
}

u64 murmur(const uintptr_t buf, u64 buf_len, u64 seed) {
  yeti_assert_debug(buf != NULL);
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  static const u32 m = 0x5bd1e995;
  static const u32 r = 24;

  u32 h1 = u32(seed) ^ buf_len;
  u32 h2 = u32(seed >> 32);

  const u32 *data = (const u32 *)buf;

  while (buf_len >= 8) {
    u32 k1 = *data++;
    k1 *= m; k1 ^= k1 >> r; k1 *= m;
    h1 *= m; h1 ^= k1;
    buf_len -= 4;

    u32 k2 = *data++;
    k2 *= m; k2 ^= k2 >> r; k2 *= m;
    h2 *= m; h2 ^= k2;
    buf_len -= 4;
  }

  if (buf_len >= 4) {
    u32 k1 = *data++;
    k1 *= m; k1 ^= k1 >> r; k1 *= m;
    h1 *= m; h1 ^= k1;
    buf_len -= 4;
  }

  switch (buf_len) {
    case 3: h2 ^= ((unsigned char *)data)[2] << 16ul;
    case 2: h2 ^= ((unsigned char *)data)[1] << 8ul;
    case 1: h2 ^= ((unsigned char *)data)[0];
            h2 *= m;
  }

  h1 ^= h2 >> 18; h1 *= m;
  h2 ^= h1 >> 22; h2 *= m;
  h1 ^= h2 >> 17; h1 *= m;
  h2 ^= h1 >> 19; h2 *= m;

  return ((((u64)h1) << 32ull) | (u64)h2);
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  static const u64 m = 0xc6a4a7935bd1e995ull;
  static const u64 r = 47;

  u64 h = seed ^ (buf_len * m);

  const u64 *data = (const u64 *)buf;
  const u64 *end = data + (buf_len/8);

  while (data != end) {
    u64 k = *data++;
    k *= m;
    k ^= k >> r;
    k *= m;
    h ^= k;
    h *= m;
  }

  const unsigned char *stragglers = (const unsigned char *)data;

  switch (buf_len & 7) {
    case 7: h ^= u64(stragglers[6]) << 48ull;
    case 6: h ^= u64(stragglers[5]) << 40ull;
    case 5: h ^= u64(stragglers[4]) << 32ull;
    case 4: h ^= u64(stragglers[3]) << 24ull;
    case 3: h ^= u64(stragglers[2]) << 16ull;
    case 2: h ^= u64(stragglers[1]) << 8ull;
    case 1: h ^= u64(stragglers[0]);
            h *= m;
  }

  h ^= h >> r;
  h *= m;
  h ^= h >> r;

  return h;
#endif
}

} // foundation
} // yeti
