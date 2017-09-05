//===-- yeti/foundation/types.h ---------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_TYPES_H_
#define _YETI_FOUNDATION_TYPES_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

// TODO(mtwilliams): Don't rely on `stdint.h`.
// TODO(mtwilliams): Provide fallbacks for shitty compilers.
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <float.h>

// We disable local function reference checking because we don't care if our
// conversion helpers are dead-code eliminated as they're in scope to promote
// inlining.
#pragma warning(disable: 4505)

YETI_BEGIN_EXTERN_C // {

typedef size_t yeti_size_t;

typedef int8_t yeti_int8_t;
typedef uint8_t yeti_uint8_t;
typedef int16_t yeti_int16_t;
typedef uint16_t yeti_uint16_t;
typedef int32_t yeti_int32_t;
typedef uint32_t yeti_uint32_t;
typedef int64_t yeti_int64_t;
typedef uint64_t yeti_uint64_t;

typedef float yeti_float32_t;
typedef double yeti_float64_t;

typedef bool yeti_bool_t;

YETI_END_EXTERN_C // }

namespace yeti {

typedef yeti_int8_t i8;
typedef yeti_uint8_t u8;
typedef yeti_int16_t i16;
typedef yeti_uint16_t u16;
typedef yeti_int32_t i32;
typedef yeti_uint32_t u32;
typedef yeti_int64_t i64;
typedef yeti_uint64_t u64;

typedef yeti_float32_t f32;
typedef yeti_float64_t f64;

/// Casts a `u8` into a `i8`.
/// \note Clamps on overflow.
static i8 i8_from_u8(const u8 value) {
  return (value > 127) ? 127 : (i8)value;
}

/// Casts a `i16` into a `i8`.
/// \note Clamps on underflow or overflow.
static i8 i8_from_i16(const i16 value) {
  const bool underflow = (value < -128);
  const bool overflow  = (value > 127);
  return (underflow ? -128 : (overflow ? 127 : (i8)value));
}

/// Casts a `u16` into a `i8`.
/// \note Clamps on overflow.
static i8 i8_from_u16(const u16 value) {
  return (value > 127) ? 127 : (i8)value;
}

/// Casts a `i32` into a `i8`.
/// \note Clamps on underflow or overflow.
static i8 i8_from_i32(const i32 value) {
  const bool underflow = (value < -128);
  const bool overflow  = (value > 127);
  return (underflow ? -128 : (overflow ? 127 : (i8)value));
}

/// Casts a `u32` into a `i8`.
/// \note Clamps on overflow.
static i8 i8_from_u32(const u32 value) {
  return (value > 127) ? 127 : (i8)value;
}

/// Casts a `i64` into a `i8`.
/// \note Clamps on underflow or overflow.
static i8 i8_from_i64(const i64 value) {
  const bool underflow = (value < -128);
  const bool overflow  = (value > 127);
  return (underflow ? -128 : (overflow ? 127 : (i8)value));
}

/// Casts a `u64` into a `i8`.
/// \note Clamps on overflow.
static i8 i8_from_u64(const u64 value) {
  return (value > 127) ? 127 : (i8)value;
}

/// Casts a `i8` into a `u8`.
/// \note Clamps on underflow.
static u8 u8_from_i8(const i8 value) {
  return (value > 0) ? (u8)value : 0;
}

/// Casts a `i16` into a `u8`.
/// \note Clamps on underflow or overflow.
static u8 u8_from_i16(const i16 value) {
  const bool underflow = (value < 0);
  const bool overflow  = (value > 255);
  return (underflow ? 0 : (overflow ? 255 : (u8)value));
}

/// Casts a `u16` into a `u8`.
/// \note Clamps on overflow.
static u8 u8_from_u16(const u16 value) {
  return (value > 255) ? 255 : (u8)value;
}

/// Casts a `i32` into a `u8`.
/// \note Clamps on underflow or overflow.
static u8 u8_from_i32(const i32 value) {
  const bool underflow = (value < 0);
  const bool overflow  = (value > 255);
  return (underflow ? 0 : (overflow ? 255 : (u8)value));
}

/// Casts a `u32` into a `u8`.
/// \note Clamps on overflow.
static u8 u8_from_u32(const u32 value) {
  return (value > 255) ? 255 : (u8)value;
}

/// Casts a `i64` into a `u8`.
/// \note Clamps on underflow or overflow.
static u8 u8_from_i64(const i64 value) {
  const bool underflow = (value < 0);
  const bool overflow  = (value > 255);
  return (underflow ? 0 : (overflow ? 255 : (u8)value));
}

/// Casts a `u64` into a `u8`.
/// \note Clamps on overflow.
static u8 u8_from_u64(const u64 value) {
  return (value > 255) ? 255 : (u8)value;
}

/// Casts a `u16` into a `i16`.
/// \note Clamps on overflow.
static i16 i16_from_u16(const u16 value) {
  return (value > 32767) ? 32767 : (i16)value;
}

/// Casts a `i32` into a `i16`.
/// \note Clamps on underflow or overflow.
static i16 i16_from_i32(const i32 value) {
  const bool underflow = (value < -32768);
  const bool overflow  = (value > 32767);
  return (underflow ? -32768 : (overflow ? 32767 : (i16)value));
}

/// Casts a `u32` into a `i16`.
/// \note Clamps on overflow.
static i16 i16_from_u32(const u32 value) {
  return (value > 32767) ? 32767 : (i16)value;
}

/// Casts a `i64` into a `i16`.
/// \note Clamps on underflow or overflow.
static i16 i16_from_i64(const i64 value) {
  const bool underflow = (value < -32768);
  const bool overflow  = (value > 32767);
  return (underflow ? -32768 : (overflow ? 32767 : (i16)value));
}

/// Casts a `u64` into a `i16`.
/// \note Clamps on overflow.
static i16 i16_from_u64(const u64 value) {
  return (value > 32767) ? 32767 : (i16)value;
}

/// Casts a `i16` into a `u16`.
/// \note Clamps on underflow.
static u16 u16_from_i16(const i16 value) {
  return (value > 0) ? (u16)value : 0;
}

/// Casts a `i32` into a `u16`.
/// \note Clamps on underflow or overflow.
static u16 u16_from_i32(const i32 value) {
  const bool underflow = (value < 0);
  const bool overflow  = (value > 65535);
  return (underflow ? 0 : (overflow ? 65535 : (u16)value));
}

/// Casts a `u32` into a `u16`.
/// \note Clamps on overflow.
static u16 u16_from_u32(const u32 value) {
  return (value > 65535) ? 65535 : (u16)value;
}

/// Casts a `i64` into a `u16`.
/// \note Clamps on underflow or overflow.
static u16 u16_from_i64(const i64 value) {
  const bool underflow = (value < 0);
  const bool overflow  = (value > 65535);
  return (underflow ? 0 : (overflow ? 65535 : (u16)value));
}

/// Casts a `u64` into a `u16`.
/// \note Clamps on overflow.
static u16 u16_from_u64(const u64 value) {
  return (value > 65535) ? 65535 : (u16)value;
}

/// Casts a `u32` into a `i32`.
/// \note Clamps on overflow.
static i32 i32_from_u32(const u32 value) {
  return (value > 2147483647) ? 2147483647 : (i32)value;
}

/// Casts a `i64` into a `i32`.
/// \note Clamps on underflow or overflow.
static i32 i32_from_i64(const i64 value) {
  static const i64 min = INT64_C(-2147483648);
  static const i64 max = INT64_C(2147483647);
  const bool underflow = (value < min);
  const bool overflow  = (value > max);
  return (underflow ? min : (overflow ? max : (i32)value));
}

/// Casts a `u64` into a `i32`.
/// \note Clamps on overflow.
static i32 i32_from_u64(const u64 value) {
  return (value > 2147483647) ? 2147483647 : (i32)value;
}

/// Casts a `i32` into a `u32`.
/// \note Clamps on underflow.
static u32 u32_from_i32(const i32 value) {
  return (value > 0) ? (u32)value : 0;
}

/// Casts a `i64` into a `u32`.
/// \note Clamps on underflow or overflow.
static u32 u32_from_i64(const i64 value) {
  const bool underflow = (value < 0);
  const bool overflow  = (value > 4294967295);
  return (underflow ? 0 : (overflow ? 4294967295 : (u32)value));
}

/// Casts a `u64` into a `u32`.
/// \note Clamps on overflow.
static u32 u32_from_u64(const u64 value) {
  return (value > 4294967295) ? 4294967295 : (u32)value;
}

/// Casts a `u64` into a `i64`.
/// \note Clamps on overflow.
static i64 i64_from_u64(const u64 value) {
  static const i64 max = UINT64_C(9223372036854775807);
  return (value > max) ? max : (i64)value;
}

/// Casts a `i64` into a `u64`.
/// \note Clamps on underflow.
static u64 u64_from_i64(const i64 value) {
  return (value > 0) ? (u64)value : 0;
}

/// Rounds a `f32` into a `i8`.
/// \note Clamps on underflow or overflow.
static i8 i8_from_f32(const f32 value) {
  const bool underflow = (value < -128.f);
  const bool overflow  = (value > 127.f);
  return (underflow ? -128 : (overflow ? 127 : (i8)value));
}

/// Rounds a `f32` into a `u8`.
/// \note Clamps on underflow or overflow.
static u8 u8_from_f32(const f32 value) {
  const bool underflow = (value < 0.f);
  const bool overflow  = (value > 255.f);
  return (underflow ? 0 : (overflow ? 255 : (u8)value));
}

/// Rounds a `f32` into a `i16`.
/// \note Clamps on underflow or overflow.
static i16 i16_from_f32(const f32 value) {
  const bool underflow = (value < -32768.f);
  const bool overflow  = (value > 32767.f);
  return (underflow ? -32768 : (overflow ? 32767 : (i16)value));
}

/// Rounds a `f32` into a `u16`.
/// \note Clamps on underflow or overflow.
static u16 u16_from_f32(const f32 value) {
  const bool underflow = (value < 0.f);
  const bool overflow  = (value > 65535.f);
  return (underflow ? 0 : (overflow ? 65535 : (u16)value));
}

/// Rounds a `f32` into a `i32`.
/// \note Clamps on underflow or overflow.
static i32 i32_from_f32(const f32 value) {
  static const f32 min = -2147483648.f;
  static const f32 max = 2147483647.f;
  const bool underflow = (value < min);
  const bool overflow  = (value > max);
  return (i32)(underflow ? min : (overflow ? max : value));
}

/// Rounds a `f32` into a `u32`.
/// \note Clamps on underflow or overflow.
static u32 u32_from_f32(const f32 value) {
  const bool underflow = (value < 0.f);
  const bool overflow  = (value > 4294967295.f);
  return (underflow ? 0 : (overflow ? 4294967295 : (u32)value));
}

/// Rounds a `f64` into a `i8`.
/// \note Clamps on underflow or overflow.
static i8 i8_from_f64(const f64 value) {
  const bool underflow = (value < -128.f);
  const bool overflow  = (value > 127.f);
  return (underflow ? -128 : (overflow ? 127 : (i8)value));
}

/// Rounds a `f64` into a `u8`.
/// \note Clamps on underflow or overflow.
static u8 u8_from_f64(const f64 value) {
  const bool underflow = (value < 0.f);
  const bool overflow  = (value > 255.f);
  return (underflow ? 0 : (overflow ? 255 : (u8)value));
}

/// Rounds a `f64` into a `i16`.
/// \note Clamps on underflow or overflow.
static i16 i16_from_f64(const f64 value) {
  const bool underflow = (value < -32768.f);
  const bool overflow  = (value > 32767.f);
  return (underflow ? -32768 : (overflow ? 32767 : (i16)value));
}

/// Rounds a `f64` into a `u16`.
/// \note Clamps on underflow or overflow.
static u16 u16_from_f64(const f64 value) {
  const bool underflow = (value < 0.f);
  const bool overflow  = (value > 65535.f);
  return (underflow ? 0 : (overflow ? 65535 : (u16)value));
}

/// Rounds a `f64` into a `i32`.
/// \note Clamps on underflow or overflow.
static i32 i32_from_f64(const f64 value) {
  static const f64 min = -2147483648.f;
  static const f64 max = 2147483647.f;
  const bool underflow = (value < min);
  const bool overflow  = (value > max);
  return (i32)(underflow ? min : (overflow ? max : value));
}

/// Rounds a `f64` into a `u32`.
/// \note Clamps on underflow or overflow.
static u32 u32_from_f64(const f64 value) {
  const bool underflow = (value < 0.f);
  const bool overflow  = (value > 4294967295.f);
  return (underflow ? 0 : (overflow ? 4294967295 : (u32)value));
}

/// Rounds a `f64` into a `i64`.
/// \note Clamps on underflow or overflow.
static i64 i64_from_f64(const f64 value) {
  static const i64 min = INT64_C(-9223372036854775808);
  static const i64 max = INT64_C(9223372036854775807);
  const bool underflow = (value < (f64)min);
  const bool overflow  = (value > (f64)max);
  return (underflow ? min : (overflow ? max : (i64)value));
}

/// Rounds a `f64` into a `u64`.
/// \note Clamps on underflow or overflow.
static u64 u64_from_f64(const f64 value) {
  static const u64 max = UINT64_C(18446744073709551615);
  const bool underflow = (value < 0.f);
  const bool overflow  = (value > (f64)max);
  return (underflow ? 0 : (overflow ? max : (u64)value));
}

}

#endif // _YETI_FOUNDATION_TYPES_H_
