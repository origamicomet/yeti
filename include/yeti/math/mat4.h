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
///
class YETI_PUBLIC Mat4 {
 public:
  Mat4();
  Mat4(const Mat4 &m);
  Mat4 operator=(const Mat4 &m);

 public:
  friend Mat4 operator*(const Mat4 &lhs, const Mat4 &rhs);
  friend Vec3 operator*(const Mat4 &m, const Vec3 &v);
  friend Vec4 operator*(const Mat4 &m, const Vec4 &v);

 public:
  /// Builds a prespective projection matrix.
  ///
  /// \param @field_of_view Horizontal field-of-view.
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
  static Mat4 translate(const Vec3 &position);

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
                        Vec3 *position,
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

 public:
  f32 m[4][4];
};

} // yeti

#endif // _YETI_MATH_MAT4_H_
