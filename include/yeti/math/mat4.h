//===-- yeti/math/mat4.h --------------------------------*- mode: C++11 -*-===//
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
/// \brief 4x4 matrix mathematics.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_MATH_MAT4_H_
#define _YETI_MATH_MAT4_H_

#include "yeti/core.h"

#include "yeti/math/vec3.h"
#include "yeti/math/vec4.h"

#include "yeti/math/quaternion.h"

#include <math.h>

namespace yeti {

/// \brief Represents arbitrary linear transformations of three-dimensional space.
class YETI_PUBLIC Mat4 {
 public:
  Mat4();

  Mat4(const f32 m[4][4]);

  Mat4(const f32 m00, const f32 m01, const f32 m02, const f32 m03,
       const f32 m10, const f32 m11, const f32 m12, const f32 m13,
       const f32 m20, const f32 m21, const f32 m22, const f32 m23,
       const f32 m30, const f32 m31, const f32 m32, const f32 m33);

  Mat4(const Mat4 &m);

  Mat4 operator=(const Mat4 &m);

 public:
  f32 &operator()(const unsigned i, const unsigned j);
  f32 operator()(const unsigned i, const unsigned j) const;

 public:
  friend Mat4 operator*(const Mat4 &lhs, const Mat4 &rhs);
  friend Vec3 operator*(const Vec3 &v, const Mat4 &m);
  friend Vec4 operator*(const Vec4 &v, const Mat4 &m);

 public:
  /// Builds a prespective projection matrix.
  ///
  /// \param @field_of_view Horizontal field-of-view in degrees.
  /// \param @aspect Aspect ratio of projected image.
  /// \param @near Near plane.
  /// \param @far Far plane.
  ///
  static Mat4 perspective(const f32 field_of_view,
                          const f32 aspect,
                          const f32 near,
                          const f32 far);

  /// Builds an orthographic projection matrix.
  ///
  /// \param @top Top of viewing volumne.
  /// \param @left Left of viewing volume.
  /// \param @bottom Bottom of viewing volume.
  /// \param @right Right of viewing volume.
  /// \param @near Near plane.
  /// \param @far Far plane.
  ///
  static Mat4 orthographic(const f32 top,
                           const f32 left,
                           const f32 bottom,
                           const f32 right,
                           const f32 near,
                           const f32 far);

 public:
  /// Builds a translation matrix.
  static Mat4 translate(const Vec3 &translation);

  /// Builds a rotation matrix.
  static Mat4 rotate(const Quaternion &rotation);

  /// Builds a scale matrix.
  static Mat4 scale(const Vec3 &scale);

 public:
  /// Builds a matrix that transforms to @position, oriented with @rotation
  /// and scaled by @scale.
  static Mat4 compose(const Vec3 &position,
                      const Quaternion &rotation,
                      const Vec3 &scale);

  /// Decomposes a matrix into its translation, rotation, and scale components.
  ///
  /// \warning Due to floating-point inaccuracy, @scale will not be recovered
  /// exactly. Don't rely on decomposition for accumulative algorithms as
  /// this will introduce error at a noticeable rate!
  ///
  static void decompose(const Mat4 &matrix,
                        Vec3 *translation,
                        Quaternion *rotation,
                        Vec3 *scale);

 public:
  /// \brief Calculates the determinant of this matrix.
  f32 determinant() const;

  /// \brief Derives the inverse of this matrix.
  Mat4 inverse() const;

  /// \brief Derives the transposition of this matrix.
  Mat4 transpose() const;

 public:
  static const Mat4 IDENTITY;
  static const Mat4 ZERO;

 private:
  f32 M[4][4];
};

YETI_INLINE Mat4::Mat4() {
  // NOTE(mtwilliams): Not initialized for efficiency sake.
}

YETI_INLINE Mat4::Mat4(const f32 m00, const f32 m01, const f32 m02, const f32 m03,
                       const f32 m10, const f32 m11, const f32 m12, const f32 m13,
                       const f32 m20, const f32 m21, const f32 m22, const f32 m23,
                       const f32 m30, const f32 m31, const f32 m32, const f32 m33)
{
  M[0][0] = m00; M[1][0] = m01; M[2][0] = m02; M[3][0] = m03;
  M[0][1] = m10; M[1][1] = m11; M[2][1] = m12; M[3][1] = m13;
  M[0][2] = m20; M[1][2] = m21; M[2][2] = m22; M[3][2] = m23;
  M[0][3] = m30; M[1][3] = m31; M[2][3] = m32; M[3][3] = m33;
}

YETI_INLINE Mat4::Mat4(const f32 m[4][4]) {
  core::memory::copy((const void *)&m[0][0], (void *)&M[0][0], sizeof(M));
}

YETI_INLINE Mat4::Mat4(const Mat4 &m) {
  core::memory::copy((const void *)&m.M[0][0], (void *)&M[0][0], sizeof(M));
}

YETI_INLINE Mat4 Mat4::operator=(const Mat4 &m) {
  core::memory::copy((const void *)&m.M[0][0], (void *)&M[0][0], sizeof(M));
  return *this;
}

// PERF(mtwillianms): Mark as `constexpr` when possible.

YETI_INLINE f32 &Mat4::operator()(const unsigned i, const unsigned j) {
  return M[j][i];
}

YETI_INLINE f32 Mat4::operator()(const unsigned i, const unsigned j) const {
  return M[j][i];
}

YETI_INLINE Mat4 operator*(const Mat4 &lhs, const Mat4 &rhs) {
  Mat4 concatenated;

  concatenated.M[0][0] = lhs.M[0][0] * rhs.M[0][0] + lhs.M[0][1] * rhs.M[1][0] + lhs.M[0][2] * rhs.M[2][0] + lhs.M[0][3] * rhs.M[3][0];
  concatenated.M[0][1] = lhs.M[0][0] * rhs.M[0][1] + lhs.M[0][1] * rhs.M[1][1] + lhs.M[0][2] * rhs.M[2][1] + lhs.M[0][3] * rhs.M[3][1];
  concatenated.M[0][2] = lhs.M[0][0] * rhs.M[0][2] + lhs.M[0][1] * rhs.M[1][2] + lhs.M[0][2] * rhs.M[2][2] + lhs.M[0][3] * rhs.M[3][2];
  concatenated.M[0][3] = lhs.M[0][0] * rhs.M[0][3] + lhs.M[0][1] * rhs.M[1][3] + lhs.M[0][2] * rhs.M[2][3] + lhs.M[0][3] * rhs.M[3][3];

  concatenated.M[1][0] = lhs.M[1][0] * rhs.M[0][0] + lhs.M[1][1] * rhs.M[1][0] + lhs.M[1][2] * rhs.M[2][0] + lhs.M[1][3] * rhs.M[3][0];
  concatenated.M[1][1] = lhs.M[1][0] * rhs.M[0][1] + lhs.M[1][1] * rhs.M[1][1] + lhs.M[1][2] * rhs.M[2][1] + lhs.M[1][3] * rhs.M[3][1];
  concatenated.M[1][2] = lhs.M[1][0] * rhs.M[0][2] + lhs.M[1][1] * rhs.M[1][2] + lhs.M[1][2] * rhs.M[2][2] + lhs.M[1][3] * rhs.M[3][2];
  concatenated.M[1][3] = lhs.M[1][0] * rhs.M[0][3] + lhs.M[1][1] * rhs.M[1][3] + lhs.M[1][2] * rhs.M[2][3] + lhs.M[1][3] * rhs.M[3][3];

  concatenated.M[2][0] = lhs.M[2][0] * rhs.M[0][0] + lhs.M[2][1] * rhs.M[1][0] + lhs.M[2][2] * rhs.M[2][0] + lhs.M[2][3] * rhs.M[3][0];
  concatenated.M[2][1] = lhs.M[2][0] * rhs.M[0][1] + lhs.M[2][1] * rhs.M[1][1] + lhs.M[2][2] * rhs.M[2][1] + lhs.M[2][3] * rhs.M[3][1];
  concatenated.M[2][2] = lhs.M[2][0] * rhs.M[0][2] + lhs.M[2][1] * rhs.M[1][2] + lhs.M[2][2] * rhs.M[2][2] + lhs.M[2][3] * rhs.M[3][2];
  concatenated.M[2][3] = lhs.M[2][0] * rhs.M[0][3] + lhs.M[2][1] * rhs.M[1][3] + lhs.M[2][2] * rhs.M[2][3] + lhs.M[2][3] * rhs.M[3][3];

  concatenated.M[3][0] = lhs.M[3][0] * rhs.M[0][0] + lhs.M[3][1] * rhs.M[1][0] + lhs.M[3][2] * rhs.M[2][0] + lhs.M[3][3] * rhs.M[3][0];
  concatenated.M[3][1] = lhs.M[3][0] * rhs.M[0][1] + lhs.M[3][1] * rhs.M[1][1] + lhs.M[3][2] * rhs.M[2][1] + lhs.M[3][3] * rhs.M[3][1];
  concatenated.M[3][2] = lhs.M[3][0] * rhs.M[0][2] + lhs.M[3][1] * rhs.M[1][2] + lhs.M[3][2] * rhs.M[2][2] + lhs.M[3][3] * rhs.M[3][2];
  concatenated.M[3][3] = lhs.M[3][0] * rhs.M[0][3] + lhs.M[3][1] * rhs.M[1][3] + lhs.M[3][2] * rhs.M[2][3] + lhs.M[3][3] * rhs.M[3][3];

  return concatenated;
}

YETI_INLINE Vec3 operator*(const Vec3 &v, const Mat4 &m) {
  Vec3 transformed;

  f32 w = 1.0f / (m.M[3][0] * v.x + m.M[3][1] * v.y + m.M[3][2] * v.z + m.M[3][3]);

  transformed.x = (m.M[0][0] * v.x + m.M[0][1] * v.y + m.M[0][2] * v.z + m.M[0][3]) * w;
  transformed.y = (m.M[1][0] * v.x + m.M[1][1] * v.y + m.M[1][2] * v.z + m.M[1][3]) * w;
  transformed.z = (m.M[2][0] * v.x + m.M[2][1] * v.y + m.M[2][2] * v.z + m.M[2][3]) * w;

  return transformed;
}

YETI_INLINE Vec4 operator*(const Vec4 &v, const Mat4 &m) {
  Vec4 transformed;

  transformed.x = m.M[0][0] * v.x + m.M[0][1] * v.y + m.M[0][2] * v.z + m.M[0][3] * v.w;
  transformed.y = m.M[1][0] * v.x + m.M[1][1] * v.y + m.M[1][2] * v.z + m.M[1][3] * v.w;
  transformed.z = m.M[2][0] * v.x + m.M[2][1] * v.y + m.M[2][2] * v.z + m.M[2][3] * v.w;
  transformed.w = m.M[3][0] * v.x + m.M[3][1] * v.y + m.M[3][2] * v.z + m.M[3][3] * v.w;

  return transformed;
}

/// \brief Extracts translation from @matrix.
extern YETI_PUBLIC Vec3 translation_from_matrix(const Mat4 &matrix);

/// \brief Extracts rotation from @matrix.
extern YETI_PUBLIC Quaternion rotation_from_matrix(const Mat4 &matrix);

/// \brief Extracts scale from @matrix.
extern YETI_PUBLIC Vec3 scale_from_matrix(const Mat4 &matrix);

} // yeti

#endif // _YETI_MATH_MAT4_H_
