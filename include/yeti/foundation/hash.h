//===-- yeti/foundation/hash.h ----------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_HASH_H_
#define _YETI_FOUNDATION_HASH_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

/// Calculates the 32-bit FNV1A hash of @buf.
extern YETI_PUBLIC u32 fnv1a_hash_32(const char *buf);

/// Calculates the 32-bit FNV1A hash of @buf.
extern YETI_PUBLIC u32 fnv1a_hash_32(const void *buf, u32 buf_len);

/// Calculates the 64-bit FNV1A hash of @buf.
extern YETI_PUBLIC u64 fnv1a_hash_64(const char *buf);

/// Calculates the 64-bit FNV1A hash of @buf.
extern YETI_PUBLIC u64 fnv1a_hash_64(const void *buf, u64 buf_len);

/// Calculates the 32-bit murmur hash of @buf with the @seed.
extern YETI_PUBLIC u32 murmur_hash_32(const char *buf, u32 seed = 0ull);

/// Calculates the 32-bit murmur hash of @buf with the @seed.
extern YETI_PUBLIC u32 murmur_hash_32(const void *buf, u32 buf_len, u32 seed = 0ull);

/// Calculates the 64-bit murmur hash of @buf with the @seed.
extern YETI_PUBLIC u64 murmur_hash_64(const char *buf, u64 seed = 0ull);

/// Calculates the 64-bit murmur hash of @buf with the @seed.
extern YETI_PUBLIC u64 murmur_hash_64(const void *buf, u64 buf_len, u64 seed = 0ull);

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_HASH_H_
