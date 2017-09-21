//===-- yeti/math/vec2.h --------------------------------*- mode: C++11 -*-===//
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
/// \brief Two-dimensional vector mathematics.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_MATH_VEC2_H_
#define _YETI_MATH_VEC2_H_

#include "yeti/core.h"

#include <math.h>

namespace yeti {

/// \brief Represents a point or direction in two-dimensional space.
///
class YETI_PUBLIC Vec2 {
 public:
  Vec2() : x(0.f), y(0.f) {}
  Vec2(f32 x, f32 y) : x(x), y(y) {}
  Vec2(const Vec2 &v) : x(v.x), y(v.y) {}
  Vec2 operator=(const Vec2 &v) { x = v.x; y = v.y; return *this; }

 public:
  friend Vec2 operator+(const Vec2 lhs, const Vec2 rhs);
  friend Vec2 operator-(const Vec2 lhs, const Vec2 rhs);
  friend Vec2 operator*(const Vec2 lhs, const Vec2 rhs);
  friend Vec2 operator*(const Vec2 v, const f32 s);
  friend Vec2 operator/(const Vec2 lhs, const Vec2 rhs);
  friend Vec2 operator/(const Vec2 v, const f32 s);

 public:
  /// \brief Returns a normalized version of this vector.
  ///
  Vec2 normalize() const;

  /// \brief Returns the length (or magnitude) of this vector.
  ///
  f32 length() const;

  /// \brief Returns the magnitude (or length) of this vector.
  ///
  f32 magnitude() const;

 public:

  /// \brief Calculates the dot-product between this vector and @v.
  ///
  f32 dot(const Vec2 v) const;

 public:
  /// \brief Returns the distance of @v2 from @v1.
  ///
  static f32 distance(const Vec2 v1, const Vec2 v2);

  /// \brief Returns the angle between @v1 and @v2.
  ///
  static f32 angle(const Vec2 v1, const Vec2 v2);

  /// \brief Returns a new vector with the smallest value taken from either @v1
  /// or @v2 for each dimension.
  ///
  static Vec2 min(const Vec2 v1, const Vec2 v2);

  /// \brief Returns a new vector with the largest value taken from either @v1
  /// or @v2 for each dimension.
  ///
  static Vec2 max(const Vec2 v1, const Vec2 v2);

  /// \brief Linearly interpolates between @v1 and @v2 based on @t.
  ///
  static Vec2 lerp(const Vec2 v1, const Vec2 v2, const f32 t);

 public:
  static const Vec2 ZERO;
  static const Vec2 ONE;

  static const Vec2 X_AXIS;
  static const Vec2 Y_AXIS;

 public:
  f32 x, y;
};

YETI_INLINE Vec2 Vec2::normalize() const {
  const f32 inv_magnitude = 1.f / magnitude();
  return (*this) * inv_magnitude;
}

YETI_INLINE f32 Vec2::length() const {
  return sqrtf((*this).dot(*this));
}

YETI_INLINE f32 Vec2::magnitude() const {
  return sqrtf((*this).dot(*this));
}

YETI_INLINE f32 Vec2::dot(const Vec2 v) const {
  return (x * v.x) + (y * v.y);
}

YETI_INLINE Vec2 operator+(const Vec2 lhs, const Vec2 rhs) {
  return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

YETI_INLINE Vec2 operator-(const Vec2 lhs, const Vec2 rhs) {
  return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

YETI_INLINE Vec2 operator*(const Vec2 lhs, const Vec2 rhs) {
  return Vec2(lhs.x * rhs.x, lhs.y * rhs.y);
}

YETI_INLINE Vec2 operator*(const Vec2 v, const f32 s) {
  return Vec2(v.x * s, v.y * s);
}

YETI_INLINE Vec2 operator/(const Vec2 lhs, const Vec2 rhs) {
  return Vec2(lhs.x / rhs.x, lhs.y / rhs.y);
}

YETI_INLINE Vec2 operator/(const Vec2 v, const f32 s) {
  return Vec2(v.x / s, v.y / s);
}

YETI_INLINE f32 Vec2::distance(const Vec2 v1, const Vec2 v2) {
  return (v1 - v2).length();
}

YETI_INLINE f32 Vec2::angle(const Vec2 v1, const Vec2 v2) {
  // Refer to http://math.stackexchange.com/questions/974178
  return acosf(v1.dot(v2) / (v1.length() * v2.length()));
}

YETI_INLINE Vec2 Vec2::min(const Vec2 v1, const Vec2 v2) {
  return Vec2(
    fminf(v1.x, v2.x),
    fminf(v1.y, v2.y)
  );
}

YETI_INLINE Vec2 Vec2::max(const Vec2 v1, const Vec2 v2) {
  return Vec2(
    fmaxf(v1.x, v2.x),
    fmaxf(v1.y, v2.y)
  );
}

// TODO(mtwilliams): Determine if this is faster.
YETI_INLINE Vec2 Vec2::lerp(const Vec2 v1, const Vec2 v2, const f32 t) {
  // Linear interpolation is `v1*(1-t) + v2*t`, which is equivalent to:
  return v1 + (v2 - v1) * t;
}

} // yeti

#endif // _YETI_MATH_VEC2_H_
