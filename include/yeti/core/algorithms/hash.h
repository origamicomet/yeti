//===-- yeti/core/algorithms/hash.h ---------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Hash functions.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_ALGORITHMS_HASH_H_
#define _YETI_CORE_ALGORITHMS_HASH_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"

namespace yeti {
namespace core {

/// Calculates an 8-bit pearson hash of @buf with the @seed.
extern YETI_PUBLIC u8 pearson_hash_8(const char *buf, u8 seed = 0);

/// Calculates an 8-bit pearson hash of @buf with the @seed.
extern YETI_PUBLIC u8 pearson_hash_8(const void *buf, u8 buf_len, u8 seed = 0);

/// Calculates the 32-bit FNV1A hash of @buf.
extern YETI_PUBLIC u32 fnv1a_hash_32(const char *buf);

/// Calculates the 32-bit FNV1A hash of @buf.
extern YETI_PUBLIC u32 fnv1a_hash_32(const void *buf, u32 buf_len);

/// Calculates the 64-bit FNV1A hash of @buf.
extern YETI_PUBLIC u64 fnv1a_hash_64(const char *buf);

/// Calculates the 64-bit FNV1A hash of @buf.
extern YETI_PUBLIC u64 fnv1a_hash_64(const void *buf, u64 buf_len);

/// Calculates the 32-bit murmur hash of @buf with the @seed.
extern YETI_PUBLIC u32 murmur_hash_32(const char *buf, u32 seed = 0);

/// Calculates the 32-bit murmur hash of @buf with the @seed.
extern YETI_PUBLIC u32 murmur_hash_32(const void *buf, u32 buf_len, u32 seed = 0);

/// Calculates the 64-bit murmur hash of @buf with the @seed.
extern YETI_PUBLIC u64 murmur_hash_64(const char *buf, u64 seed = 0);

/// Calculates the 64-bit murmur hash of @buf with the @seed.
extern YETI_PUBLIC u64 murmur_hash_64(const void *buf, u64 buf_len, u64 seed = 0);

} // core
} // yeti

#endif // _YETI_CORE_ALGORITHMS_HASH_H_
