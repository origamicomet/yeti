//===-- yeti/math/constants.h ---------------------------*- mode: C++11 -*-===//
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
/// \brief Mathematical constants.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_MATH_CONSTANTS_H_
#define _YETI_MATH_CONSTANTS_H_

#include "yeti/core.h"

namespace yeti {

/// \brief Euler's constant.
/// \detail The limit of `(1 + 1/n) * n` as `n` approaches infinity.
static const f32 EULER = 2.71828182845904523536f;

/// \brief Ratio of a circle's circumference to its diameter.
static const f32 PI = 3.14159265358979323846264338327950288f;

/// \brief Twice as good as Pi.
/// \see yeti::PI
static const f32 TAU = 2.f * PI;

} // yeti

#endif // _YETI_MATH_CONSTANTS_H_
