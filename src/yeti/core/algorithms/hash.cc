//===-- yeti/core/algorithms/hash.cc --------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/algorithms/hash.h"

// For sanity checks.
#include "yeti/core/debug/assert.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace yeti {
namespace core {

u8 pearson_hash_8(const char *buf, u8 seed) {
  yeti_assert_debug(buf != NULL);
  return pearson_hash_8((const void *)buf, strlen(buf), seed);
}

u8 pearson_hash_8(const void *buf, u8 buf_len, u8 seed) {
  static const u8 T[256] = {
     86,  42, 201, 202, 133,  69, 117,  62, 122, 250, 239, 100, 235, 127,  58, 145,
    135, 166,  32, 176,  21, 106, 142, 211,  98,  80,   0,   2, 169, 172, 208, 252,
     43,  66, 229, 128,  13, 216, 246, 214, 194, 155, 146, 249,   5, 147, 144, 203,
    199, 102, 189, 230,  73, 223, 206, 154, 149, 224,  88, 255,  19, 207,  45,  14,
    151, 198, 232, 253, 205, 225, 209,  17,  61, 184,  33,  34,  47, 109,  96, 187,
    107, 175, 162, 217, 231,  89,  26,  48,  30,  57,  10, 196, 118,  41, 112, 177,
    226,  85, 227, 157,  65, 131, 121, 186, 233,  71, 171, 103, 137,  60,   6, 111,
     83, 212, 241,  22,  37,  78,  52,  31, 219, 170,  24, 222, 173,   1, 243,   4,
     28,  29, 159,  35, 134, 161, 129,  55, 238, 179, 126, 181, 254,  18, 116, 242,
    240, 213, 110, 210,  87, 115, 168, 140,  39,  27,  46,  59, 190,  99, 130, 153,
    193, 165,  49,  91, 215,  15,  84,  16,   9, 191, 220,  44,  90,  51, 139,  95,
    164, 174, 178, 119,  93, 183, 248, 244,  72, 125, 237, 156, 192, 138, 195, 132,
     70, 218,  11, 124,   7, 143, 123, 158,  50,  36,  94, 200,  75, 167, 113,  54,
    221,  23,   8, 136, 105, 204,  67, 245,  64, 180,  40, 150,  81,  97, 141, 163,
     38,  74,   3,  56, 108, 234,  76,  63, 188, 247, 101,  25,  82, 104,  53, 197,
    148,  20,  79, 228, 251, 120,  92, 152, 114, 185,  12, 160,  68,  77, 182, 236,
  };

  const u8 *start = (const u8 *)buf;
  const u8 *end = start + buf_len;

  u8 hash = seed;

  for (const u8 *I = start; I != end; ++I)
    hash = T[hash ^ *I];

  return hash;
}

u32 fnv1a_hash_32(const char *buf) {
  yeti_assert_debug(buf != NULL);
  return fnv1a_hash_32((const void *)buf, strlen(buf));
}

u32 fnv1a_hash_32(const void *buf, u32 buf_len) {
  const u8 *start = (const u8 *)buf;
  const u8 *end = start + buf_len;

  u32 hash = 2166136261ul;

  for (const u8 *I = start; I != end; ++I)
    hash = (hash ^ *I) * 16777619ul;

  return hash;
}

u64 fnv1a_hash_64(const char *buf) {
  yeti_assert_debug(buf != NULL);
  return fnv1a_hash_64((const void *)buf, strlen(buf));
}

u64 fnv1a_hash_64(const void *buf, u64 buf_len) {
  const u8 *start = (const u8 *)buf;
  const u8 *end = start + buf_len;

  u64 hash = 14695981039346656037ull;

  for (const u8 *I = start; I != end; ++I)
    hash = (hash ^ *I) * 1099511628211ull;

  return hash;
}

u32 murmur_hash_32(const char *buf, u32 seed) {
  yeti_assert_debug(buf != NULL);
  return murmur_hash_32((const void *)buf, strlen(buf), seed);
}

u32 murmur_hash_32(const void *buf, u32 buf_len, u32 seed) {
  yeti_assert_debug(buf != NULL);

  static const u32 m = 0x5bd1e995ul;
  static const u32 r = 24ul;

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
    case 3: h ^= stragglers[2] << 16ul;
    case 2: h ^= stragglers[1] << 8ul;
    case 1: h ^= stragglers[0];
            h *= m;
  }

  h ^= h >> 13ul;
  h *= m;
  h ^= h >> 15ul;

  return h;
}

u64 murmur_hash_64(const char *buf, u64 seed) {
  yeti_assert_debug(buf != NULL);
  return murmur_hash_64((const void *)buf, strlen(buf), seed);
}

u64 murmur_hash_64(const void *buf, u64 buf_len, u64 seed) {
  yeti_assert_debug(buf != NULL);
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  static const u32 m = 0x5bd1e995ul;
  static const u32 r = 24ul;

  u32 h1 = u32(seed) ^ buf_len;
  u32 h2 = u32(seed >> 32ul);

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
    case 3: h2 ^= u32(stragglers[2]) << 16ul;
    case 2: h2 ^= u32(stragglers[1]) << 8ul;
    case 1: h2 ^= u32(stragglers[0]);
            h2 *= m;
  }

  h1 ^= h2 >> 18; h1 *= m;
  h2 ^= h1 >> 22; h2 *= m;
  h1 ^= h2 >> 17; h1 *= m;
  h2 ^= h1 >> 19; h2 *= m;

  return ((u64(h1) << 32ull) | u64(h2));
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  static const u64 m = 0xc6a4a7935bd1e995ull;
  static const u64 r = 47ull;

  u64 h = seed ^ (buf_len * m);

  const u64 *data = (const u64 *)buf;
  const u64 *end = data + (buf_len / 8);

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

} // core
} // yeti
