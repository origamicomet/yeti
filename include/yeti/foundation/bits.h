//===-- yeti/foundation/bits.h --------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_FOUNDATION_BITS_H_
#define _YETI_FOUNDATION_BITS_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"

#if YETI_COMPILER == YETI_COMPILER_MSVC
  #include <intrin.h>
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif

namespace yeti {
namespace foundation {

/// \namespace ::yeti::foundation::bit
namespace bit {
  /// Counts number of set bits.
  unsigned count(const u8 v);
  unsigned count(const u16 v);
  unsigned count(const u32 v);
  unsigned count(const u64 v);

  /// Counts number of leading zeros.
  unsigned clz(const u8 v);
  unsigned clz(const u16 v);
  unsigned clz(const u32 v);
  unsigned clz(const u64 v);

  /// Counts number of trailing zeros.
  unsigned ctz(const u8 v);
  unsigned ctz(const u16 v);
  unsigned ctz(const u32 v);
  unsigned ctz(const u64 v);
}

YETI_INLINE unsigned bit::count(const u8 v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  static const unsigned PRECOMPUTED[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 
  };

  return PRECOMPUTED[v];
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __builtin_popcountl(v);
#endif
}

YETI_INLINE unsigned bit::count(const u16 v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  const u8 *bytes = (const u8 *)&v;
  return count(bytes[0]) + count(bytes[1]);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __builtin_popcountl(v);
#endif
}

YETI_INLINE unsigned bit::count(const u32 v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  const u8 *bytes = (const u8 *)&v;
  return count(bytes[0]) + count(bytes[1]) + count(bytes[2]) + count(bytes[3]);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __builtin_popcountl(v);
#endif
}

YETI_INLINE unsigned bit::count(const u64 v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  const u8 *bytes = (const u8 *)&v;
  return count(bytes[0]) + count(bytes[1]) + count(bytes[2]) + count(bytes[3])
       + count(bytes[4]) + count(bytes[5]) + count(bytes[6]) + count(bytes[7]);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __builtin_popcountll(v);
#endif
}


YETI_INLINE unsigned bit::clz(const u8 v) {
  return clz((u32)v);
}

YETI_INLINE unsigned bit::clz(const u16 v) {
  return clz((u32)v);
}

YETI_INLINE unsigned bit::clz(const u32 v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  unsigned long bit;
  return _BitScanReverse((unsigned long *)&bit, v) ? (31 - bit) : 32;
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return v ? __builtin_clzl(v) : 32;
#endif
}

YETI_INLINE unsigned bit::clz(const u64 v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    unsigned long bit;
    return (_BitScanReverse(&bit, (v >> 32)) ? (31 - bit) : (_BitScanReverse(&bit, v) ? (63 - bit) : 64));
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    unsigned long bit;
    return _BitScanReverse64(&bit, v) ? (63 - bit) : 64;
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return v ? __builtin_ctzll(v) : 64;
#endif
}

YETI_INLINE unsigned bit::ctz(const u8 v) {
  return ctz((u32)v);
}

YETI_INLINE unsigned bit::ctz(const u16 v) {
  return ctz((u32)v);
}

YETI_INLINE unsigned bit::ctz(const u32 v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  unsigned long bit;
  return _BitScanForward((unsigned long *)&bit, v) ? bit : 32;
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return v ? __builtin_ctzl(v) : 32;
#endif
}

YETI_INLINE unsigned bit::ctz(const u64 v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    unsigned long bit;
    return _BitScanForward(&bit, v) ? bit : (_BitScanForward(&bit, (v >> 32)) ? (bit + 32) : 64);
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    unsigned long bit;
    return _BitScanForward64(&bit, v) ? bit : 64;
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return v ? __builtin_ctzll(v) : 64;
#endif
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_BITS_H_
