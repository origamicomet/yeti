//===-- yeti/math/vec3.h --------------------------------*- mode: C++11 -*-===//
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
/// \brief Three-dimensional vector mathematics.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_MATH_VEC3_H_
#define _YETI_MATH_VEC3_H_

#include "yeti/core.h"

#include <math.h>

namespace yeti {

/// \brief Represents a point or direction in three-dimensional space.
///
class YETI_PUBLIC Vec3 {
 public:
  Vec3() : x(0.f), y(0.f), z(0.f) {}
  Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
  Vec3(const Vec3 &v) : x(v.x), y(v.y), z(v.z) {}
  Vec3 operator=(const Vec3 &v) { x = v.x; y = v.y; z = v.z; return *this; }

 public:
  friend Vec3 operator+(const Vec3 lhs, const Vec3 rhs);
  friend Vec3 operator-(const Vec3 lhs, const Vec3 rhs);
  friend Vec3 operator*(const Vec3 lhs, const Vec3 rhs);
  friend Vec3 operator*(const Vec3 v, const f32 s);
  friend Vec3 operator/(const Vec3 lhs, const Vec3 rhs);
  friend Vec3 operator/(const Vec3 v, const f32 s);

 public:
  /// \brief Returns a normalized version of this vector.
  ///
  Vec3 normalize() const;

  /// \brief Returns the length (or magnitude) of this vector.
  ///
  f32 length() const;

  /// \brief Returns the magnitude (or length) of this vector.
  ///
  f32 magnitude() const;

  /// \brief Calculates the dot-product between this vector and @v.
  ///
  f32 dot(const Vec3 v) const;

  /// \brief Calculates the cross-product between this vector and @v.
  ///
  Vec3 cross(const Vec3 v) const;

 public:
  /// \brief Returns the distance of @v2 from @v1.
  ///
  static f32 distance(const Vec3 v1, const Vec3 v2);

  /// \brief Returns the angle between @v1 and @v2.
  ///
  static f32 angle(const Vec3 v1, const Vec3 v2);

  /// \brief Returns a new vector with the smallest value taken from either @v1
  /// or @v2 for each dimension.
  ///
  static Vec3 min(const Vec3 v1, const Vec3 v2);

  /// \brief Returns a new vector with the largest value taken from either @v1
  /// or @v2 for each dimension.
  ///
  static Vec3 max(const Vec3 v1, const Vec3 v2);

  /// \brief Linearly interpolates between @v1 and @v2 based on @t.
  ///
  static Vec3 lerp(const Vec3 v1, const Vec3 v2, const f32 t);

 public:
  static const Vec3 ZERO;
  static const Vec3 ONE;

  static const Vec3 X_AXIS;
  static const Vec3 Y_AXIS;
  static const Vec3 Z_AXIS;

 public:
  f32 x, y, z;
};

YETI_INLINE Vec3 Vec3::normalize() const {
  const f32 inv_magnitude = 1.f / magnitude();
  return (*this) * inv_magnitude;
}

YETI_INLINE f32 Vec3::length() const {
  return sqrtf((*this).dot(*this));
}

YETI_INLINE f32 Vec3::magnitude() const {
  return sqrtf((*this).dot(*this));
}

YETI_INLINE f32 Vec3::dot(const Vec3 v) const {
  return (x * v.x) + (y * v.y) + (z * v.z);
}

YETI_INLINE Vec3 Vec3::cross(const Vec3 v) const {
  return Vec3(
    y * v.z - z * v.y,
    z * v.x - x * v.z,
    x * v.y - y * v.x
  );
}

YETI_INLINE Vec3 operator+(const Vec3 lhs, const Vec3 rhs) {
  return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

YETI_INLINE Vec3 operator-(const Vec3 lhs, const Vec3 rhs) {
  return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

YETI_INLINE Vec3 operator*(const Vec3 lhs, const Vec3 rhs) {
  return Vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

YETI_INLINE Vec3 operator*(const Vec3 v, const f32 s) {
  return Vec3(v.x * s, v.y * s, v.z * s);
}

YETI_INLINE Vec3 operator/(const Vec3 lhs, const Vec3 rhs) {
  return Vec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

YETI_INLINE Vec3 operator/(const Vec3 v, const f32 s) {
  return Vec3(v.x / s, v.y / s, v.z / s);
}

YETI_INLINE f32 Vec3::distance(const Vec3 v1, const Vec3 v2) {
  return (v1 - v2).length();
}

YETI_INLINE f32 Vec3::angle(const Vec3 v1, const Vec3 v2) {
  // Refer to http://math.stackexchange.com/questions/974178
  return acosf(v1.dot(v2) / (v1.length() * v2.length()));
}

YETI_INLINE Vec3 Vec3::min(const Vec3 v1, const Vec3 v2) {
  return Vec3(
    fminf(v1.x, v2.x),
    fminf(v1.y, v2.y),
    fminf(v1.z, v2.z)
  );
}

YETI_INLINE Vec3 Vec3::max(const Vec3 v1, const Vec3 v2) {
  return Vec3(
    fmaxf(v1.x, v2.x),
    fmaxf(v1.y, v2.y),
    fmaxf(v1.z, v2.z)
  );
}

// TODO(mtwilliams): Determine if this is faster.
YETI_INLINE Vec3 Vec3::lerp(const Vec3 v1, const Vec3 v2, const f32 t) {
  // Linear interpolation is `v1*(1-t) + v2*t`, which is equivalent to:
  return v1 + (v2 - v1) * t;
}

} // yeti

#endif // _YETI_MATH_VEC3_H_
