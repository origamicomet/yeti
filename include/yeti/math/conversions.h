//===-- yeti/math/conversions.h -------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Various unit conversion helpers.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_MATH_CONVERSIONS_H_
#define _YETI_MATH_CONVERSIONS_H_

#include "yeti/core.h"

#include "yeti/math/constants.h"

#include <math.h>

namespace yeti {

// PERF(mtwillianms): Mark as `constexpr` when possible.

/// \brief Converts degrees to radians.
YETI_INLINE f32 degrees_to_radians(const f32 degrees) {
  return degrees * (180.f / PI);
}

/// \brief Converts radians to degrees.
YETI_INLINE f32 radians_to_degrees(const f32 radians) {
  return radians * (PI / 180.f);
}

} // yeti

#endif // _YETI_MATH_CONVERSIONS_H_
