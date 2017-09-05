//===-- yeti/foundation/hash.cc -------------------------*- mode: C++11 -*-===//
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

u32 fnv1a_hash_32(const char *buf) {
  yeti_assert_debug(buf != NULL);
  return fnv1a_hash_32((const void *)buf, strlen(buf));
}

u32 fnv1a_hash_32(const void *buf, u32 buf_len) {
  const u8 *start = (const u8 *)buf;
  const u8 *end = start + buf_len;
  u32 hash = UINT32_C(2166136261);
  for (const u8 *I = start; I != end; ++I)
    hash = (hash ^ *I) * UINT32_C(16777619);
  return hash;
}

u64 fnv1a_hash_64(const char *buf) {
  yeti_assert_debug(buf != NULL);
  return fnv1a_hash_64((const void *)buf, strlen(buf));
}

u64 fnv1a_hash_64(const void *buf, u64 buf_len) {
  const u8 *start = (const u8 *)buf;
  const u8 *end = start + buf_len;
  u64 hash = UINT64_C(14695981039346656037);
  for (const u8 *I = start; I != end; ++I)
    hash = (hash ^ *I) * UINT64_C(1099511628211);
  return hash;
}

u32 murmur_hash_32(const char *buf, u32 seed) {
  yeti_assert_debug(buf != NULL);
  return murmur_hash_32((const void *)buf, strlen(buf), seed);
}

u32 murmur_hash_32(const void *buf, u32 buf_len, u32 seed) {
  yeti_assert_debug(buf != NULL);

  static const u32 m = UINT32_C(0x5bd1e995);
  static const u32 r = UINT32_C(24);

  u32 h = seed ^ buf_len;

  const u8 *data = (const u8 *)buf;

  while (buf_len >= 4) {
    u32 k = *(const u32 *)data;

    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    data += 4;
    buf_len -= 4;
  }

  const u8 *stragglers = (const u8 *)data;
  switch (buf_len) {
    case 3: h ^= stragglers[2] << UINT32_C(16);
    case 2: h ^= stragglers[1] << UINT32_C(8);
    case 1: h ^= stragglers[0];
            h *= m;
  }

  h ^= h >> UINT32_C(13);
  h *= m;
  h ^= h >> UINT32_C(15);

  return h;
}

u64 murmur_hash_64(const char *buf, u64 seed) {
  yeti_assert_debug(buf != NULL);
  return murmur_hash_64((const void *)buf, strlen(buf), seed);
}

u64 murmur_hash_64(const void *buf, u64 buf_len, u64 seed) {
  yeti_assert_debug(buf != NULL);
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  static const u32 m = UINT32_C(0x5bd1e995);
  static const u32 r = UINT32_C(24);

  u32 h1 = u32(seed) ^ buf_len;
  u32 h2 = u32(seed >> UINT32_C(32));

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

  const u8 *stragglers = (const u8 *)data;
  switch (buf_len) {
    case 3: h2 ^= ((const u8 *)stragglers)[2] << UINT32_C(16);
    case 2: h2 ^= ((const u8 *)stragglers)[1] << UINT32_C(8);
    case 1: h2 ^= ((const u8 *)stragglers)[0];
            h2 *= m;
  }

  h1 ^= h2 >> 18; h1 *= m;
  h2 ^= h1 >> 22; h2 *= m;
  h1 ^= h2 >> 17; h1 *= m;
  h2 ^= h1 >> 19; h2 *= m;

  return ((((u64)h1) << UINT64_C(32)) | (u64)h2);
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  static const u64 m = UINT64_C(0xc6a4a7935bd1e995);
  static const u64 r = UINT64_C(47);

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

  const u8 *stragglers = (const u8 *)data;
  switch (buf_len & 7) {
    case 7: h ^= u64(stragglers[6]) << UINT64_C(48);
    case 6: h ^= u64(stragglers[5]) << UINT64_C(40);
    case 5: h ^= u64(stragglers[4]) << UINT64_C(32);
    case 4: h ^= u64(stragglers[3]) << UINT64_C(24);
    case 3: h ^= u64(stragglers[2]) << UINT64_C(16);
    case 2: h ^= u64(stragglers[1]) << UINT64_C(8);
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
