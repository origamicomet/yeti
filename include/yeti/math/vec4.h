//===-- yeti/math/vec4.h ----------------------------------*- mode: C++ -*-===//
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
/// \brief Four-dimensional vector mathematics.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_MATH_VEC4_H_
#define _YETI_MATH_VEC4_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {

/// \brief Represents a point or direction in four-dimensional space.
///
class YETI_PUBLIC Vec4 {
 public:
  Vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
  Vec4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
  Vec4(const Vec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
  Vec4 operator=(const Vec4 &v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

 public:
  friend Vec4 operator+(const Vec4 lhs, const Vec4 rhs);
  friend Vec4 operator-(const Vec4 lhs, const Vec4 rhs);
  friend Vec4 operator*(const Vec4 lhs, const Vec4 rhs);
  friend Vec4 operator*(const Vec4 v, const f32 s);
  friend Vec4 operator/(const Vec4 lhs, const Vec4 rhs);
  friend Vec4 operator/(const Vec4 v, const f32 s);

 public:
  /// \brief Returns a normalized version of this vector.
  ///
  Vec4 normalize() const;

  /// \brief Returns the length (or magnitude) of this vector.
  ///
  f32 length() const;

  /// \brief Returns the magnitude (or length) of this vector.
  ///
  f32 magnitude() const;

  /// \brief Calculates the dot-product between this vector and @v.
  ///
  f32 dot(const Vec4 v) const;

 public:
  /// \brief Returns the distance of @v2 from @v1.
  ///
  static f32 distance(const Vec4 v1, const Vec4 v2);

  /// \brief Returns the angle between @v1 and @v2.
  ///
  static f32 angle(const Vec4 v1, const Vec4 v2);

  /// \brief Returns a new vector with the smallest value taken from either @v1
  /// or @v2 for each dimension.
  ///
  static Vec4 min(const Vec4 v1, const Vec4 v2);

  /// \brief Returns a new vector with the largest value taken from either @v1
  /// or @v2 for each dimension.
  ///
  static Vec4 max(const Vec4 v1, const Vec4 v2);

  /// \brief Linearly interpolates between @v1 and @v2 based on @t.
  ///
  static Vec4 lerp(const Vec4 v1, const Vec4 v2, const f32 t);

 public:
  static const Vec4 ZERO;
  static const Vec4 ONE;

  static const Vec4 X_AXIS;
  static const Vec4 Y_AXIS;
  static const Vec4 Z_AXIS;
  static const Vec4 W_AXIS;

 public:
  f32 x, y, z, w;
};

YETI_INLINE Vec4 Vec4::normalize() const {
  const f32 inv_magnitude = 1.f / magnitude();
  return (*this) * inv_magnitude;
}

YETI_INLINE f32 Vec4::length() const {
  return sqrtf((*this).dot(*this));
}

YETI_INLINE f32 Vec4::magnitude() const {
  return sqrtf((*this).dot(*this));
}

YETI_INLINE f32 Vec4::dot(const Vec4 v) const {
  return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w);
}

YETI_INLINE Vec4 operator+(const Vec4 lhs, const Vec4 rhs) {
  return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

YETI_INLINE Vec4 operator-(const Vec4 lhs, const Vec4 rhs) {
  return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

YETI_INLINE Vec4 operator*(const Vec4 lhs, const Vec4 rhs) {
  return Vec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

YETI_INLINE Vec4 operator*(const Vec4 v, const f32 s) {
  return Vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

YETI_INLINE Vec4 operator/(const Vec4 lhs, const Vec4 rhs) {
  return Vec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}

YETI_INLINE Vec4 operator/(const Vec4 v, const f32 s) {
  return Vec4(v.x / s, v.y / s, v.z / s, v.w / s);
}

YETI_INLINE f32 Vec4::distance(const Vec4 v1, const Vec4 v2) {
  return (v1 - v2).length();
}

YETI_INLINE f32 Vec4::angle(const Vec4 v1, const Vec4 v2) {
  // Refer to http://math.stackexchange.com/questions/974178
  return acosf(v1.dot(v2) / (v1.length() * v2.length()));
}

YETI_INLINE Vec4 Vec4::min(const Vec4 v1, const Vec4 v2) {
  return Vec4(
    fminf(v1.x, v2.x),
    fminf(v1.y, v2.y),
    fminf(v1.z, v2.z),
    fminf(v1.w, v2.w)
  );
}

YETI_INLINE Vec4 Vec4::max(const Vec4 v1, const Vec4 v2) {
  return Vec4(
    fmaxf(v1.x, v2.x),
    fmaxf(v1.y, v2.y),
    fmaxf(v1.z, v2.z),
    fmaxf(v1.w, v2.w)
  );
}

// TODO(mtwilliams): Determine if this is faster.
YETI_INLINE Vec4 Vec4::lerp(const Vec4 v1, const Vec4 v2, const f32 t) {
  // Linear interpolation is `v1*(1-t) + v2*t`, which is equivalent to:
  return v1 + (v2 - v1) * t;
}

} // yeti

#endif // _YETI_MATH_VEC4_H_
