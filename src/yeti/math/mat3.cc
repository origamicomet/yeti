//===-- yeti/math/mat3.cc -------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/math/mat3.h"

namespace yeti {

const Mat3 Mat3::IDENTITY = Mat3(1.f, 0.f, 0.f,
                                 0.f, 1.f, 0.f,
                                 0.f, 0.f, 1.f);

const Mat3 Mat3::ZERO = Mat3(0.f, 0.f, 0.f,
                             0.f, 0.f, 0.f,
                             0.f, 0.f, 0.f);

#if 0
Mat3 Mat3::from_axis_angle(const Vec3 &axis, const f32 angle) {
}

Mat3 Mat3::from_euler_angles(const f32 x, const f32 y, const f32 z) {
}
#endif

f32 Mat3::determinant() const {
  return M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2])
       + M[1][0] * (M[2][1] * M[0][2] - M[0][1] * M[2][2])
       + M[2][0] * (M[0][1] * M[1][2] - M[1][1] * M[0][2]);
}

#if 0
Mat3 Mat3::inverse() const {
}
#endif

Mat3 Mat3::transpose() const {
  return Mat3(M[0][0], M[0][1], M[0][2],
              M[1][0], M[1][1], M[1][2],
              M[2][0], M[2][1], M[2][2]);
}

} // yeti
