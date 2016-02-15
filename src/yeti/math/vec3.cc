//===-- yeti/math/vec3.cc ---------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/math/vec3.h"

namespace yeti {

const Vec3 Vec3::ZERO   = Vec3(0.f, 0.f, 0.f);
const Vec3 Vec3::ONE    = Vec3(1.f, 1.f, 1.f);

const Vec3 Vec3::X_AXIS = Vec3(1.f, 0.f, 0.f);
const Vec3 Vec3::Y_AXIS = Vec3(0.f, 1.f, 0.f);
const Vec3 Vec3::Z_AXIS = Vec3(0.f, 0.f, 1.f);

} // yeti
