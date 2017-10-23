//===-- yeti/math/mat3.h --------------------------------*- mode: C++11 -*-===//
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
/// \brief 3x3 matrix mathematics.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_MATH_MAT3_H_
#define _YETI_MATH_MAT3_H_

#include "yeti/core.h"

#include "yeti/math/vec3.h"
#include "yeti/math/vec3.h"

#include "yeti/math/quaternion.h"

#include <math.h>

namespace yeti {

/// \brief Represents rotations of three-dimensional space.
class YETI_PUBLIC Mat3 {
 public:
  Mat3();

  Mat3(const f32 m[3][3]);

  Mat3(const f32 m00, const f32 m01, const f32 m02,
       const f32 m10, const f32 m11, const f32 m12,
       const f32 m20, const f32 m21, const f32 m22);

  Mat3(const Mat3 &m);

  Mat3 operator=(const Mat3 &m);

 public:
  f32 &operator()(const unsigned i, const unsigned j);
  f32 operator()(const unsigned i, const unsigned j) const;

 public:
  friend Mat3 operator*(const Mat3 &lhs, const Mat3 &rhs);
  friend Vec3 operator*(const Mat3 &m, const Vec3 &v);

 public:
  /// Derives a matrix from a rotation of @angle radians around @axis.
  static Mat3 from_axis_angle(const Vec3 &axis, const f32 angle);

  /// Derives a matrix from rotations in radians around principle axes.
  static Mat3 from_euler_angles(const f32 x, const f32 y, const f32 z);

 public:
  /// \brief Calculates the determinant of this matrix.
  f32 determinant() const;

  /// \brief Derives the inverse of this matrix.
  Mat3 inverse() const;

  /// \brief Derives the transposition of this matrix.
  Mat3 transpose() const;

 public:
  static const Mat3 IDENTITY;
  static const Mat3 ZERO;

 private:
  f32 M[3][3];
};

YETI_INLINE Mat3::Mat3() {
  // NOTE(mtwilliams): Not initialized for efficiency sake.
}

YETI_INLINE Mat3::Mat3(const f32 m00, const f32 m01, const f32 m02,
                       const f32 m10, const f32 m11, const f32 m12,
                       const f32 m20, const f32 m21, const f32 m22)
{
  M[0][0] = m00; M[1][0] = m01; M[2][0] = m02;
  M[0][1] = m10; M[1][1] = m11; M[2][1] = m12;
  M[0][2] = m20; M[1][2] = m21; M[2][2] = m22;
}

YETI_INLINE Mat3::Mat3(const f32 m[3][3]) {
  core::memory::copy((const void *)&m[0][0], (void *)&M[0][0], sizeof(M));
}

YETI_INLINE Mat3::Mat3(const Mat3 &m) {
  core::memory::copy((const void *)&m.M[0][0], (void *)&M[0][0], sizeof(M));
}

YETI_INLINE Mat3 Mat3::operator=(const Mat3 &m) {
  core::memory::copy((const void *)&m.M[0][0], (void *)&M[0][0], sizeof(M));
  return *this;
}

// PERF(mtwillianms): Mark as `constexpr` when possible.

YETI_INLINE f32 &Mat3::operator()(const unsigned i, const unsigned j) {
  return M[j][i];
}

YETI_INLINE f32 Mat3::operator()(const unsigned i, const unsigned j) const {
  return M[j][i];
}

YETI_INLINE Mat3 operator*(const Mat3 &lhs, const Mat3 &rhs) {
  Mat3 concatenated;

  concatenated.M[0][0] = lhs.M[0][0] * rhs.M[0][0] + lhs.M[1][0] * rhs.M[0][1] + lhs.M[2][0] * rhs.M[0][2];
  concatenated.M[1][0] = lhs.M[0][0] * rhs.M[1][0] + lhs.M[1][0] * rhs.M[1][1] + lhs.M[2][0] * rhs.M[1][2];
  concatenated.M[2][0] = lhs.M[0][0] * rhs.M[2][0] + lhs.M[1][0] * rhs.M[2][1] + lhs.M[2][0] * rhs.M[2][2];

  concatenated.M[0][1] = lhs.M[0][1] * rhs.M[0][0] + lhs.M[1][1] * rhs.M[0][1] + lhs.M[2][1] * rhs.M[0][2];
  concatenated.M[1][1] = lhs.M[0][1] * rhs.M[1][0] + lhs.M[1][1] * rhs.M[1][1] + lhs.M[2][1] * rhs.M[1][2];
  concatenated.M[2][1] = lhs.M[0][1] * rhs.M[2][0] + lhs.M[1][1] * rhs.M[2][1] + lhs.M[2][1] * rhs.M[2][2];

  concatenated.M[0][2] = lhs.M[0][2] * rhs.M[0][0] + lhs.M[1][2] * rhs.M[0][1] + lhs.M[2][2] * rhs.M[0][2];
  concatenated.M[1][2] = lhs.M[0][2] * rhs.M[1][0] + lhs.M[1][2] * rhs.M[1][1] + lhs.M[2][2] * rhs.M[1][2];
  concatenated.M[2][2] = lhs.M[0][2] * rhs.M[2][0] + lhs.M[1][2] * rhs.M[2][1] + lhs.M[2][2] * rhs.M[2][2];

  return concatenated;
}

} // yeti

#endif // _YETI_MATH_MAT3_H_
