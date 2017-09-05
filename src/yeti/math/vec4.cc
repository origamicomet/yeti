//===-- yeti/math/vec4.cc -------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/math/vec4.h"

namespace yeti {

const Vec4 Vec4::ZERO   = Vec4(0.f, 0.f, 0.f, 0.f);
const Vec4 Vec4::ONE    = Vec4(1.f, 1.f, 1.f, 1.f);

const Vec4 Vec4::X_AXIS = Vec4(1.f, 0.f, 0.f, 0.f);
const Vec4 Vec4::Y_AXIS = Vec4(0.f, 1.f, 0.f, 0.f);
const Vec4 Vec4::Z_AXIS = Vec4(0.f, 0.f, 1.f, 0.f);
const Vec4 Vec4::W_AXIS = Vec4(0.f, 0.f, 0.f, 1.f);

} // yeti
