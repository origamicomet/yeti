//===-- yeti/math/vec2.cc -------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/math/vec2.h"

namespace yeti {

const Vec2 Vec2::ZERO   = Vec2(0.f, 0.f);
const Vec2 Vec2::ONE    = Vec2(1.f, 1.f);

const Vec2 Vec2::X_AXIS = Vec2(1.f, 0.f);
const Vec2 Vec2::Y_AXIS = Vec2(0.f, 1.f);

} // yeti
