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

YETI_BEGIN_EXTERN_C // {

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

YETI_BEGIN_NAMESPACE // {

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

YETI_END_NAMESPACE // }

#endif // _YETI_FOUNDATION_TYPES_H_
