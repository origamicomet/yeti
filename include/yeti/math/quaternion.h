//===-- yeti/math/quaternion.h --------------------------*- mode: C++11 -*-===//
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
/// \brief Quaternions!
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_MATH_QUATERNION_H_
#define _YETI_MATH_QUATERNION_H_

#include "yeti/core.h"

#include "yeti/math/vec3.h"

#include <math.h>

namespace yeti {

/// \brief Represents a direction.
///
class YETI_PUBLIC Quaternion {
 public:
  Quaternion() : x(0.f), y(0.f), z(0.f), w(1.f) {}
  Quaternion(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
  Quaternion(const Quaternion &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
  Quaternion operator=(const Quaternion &v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

 public:
  friend Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);
  friend Quaternion operator*(const Quaternion &q, const f32 s);

 public:
  /// Derives a unit quaternion from a rotation of @angle radians around @axis.
  static Quaternion from_axis_angle(const Vec3 &axis, const f32 angle);

  /// Derives a unit quaternion from rotations in radians around principle axes.
  static Quaternion from_euler_angles(const f32 x, const f32 y, const f32 z);

 public:
  /// \brief Returns a normalized version of this quaternion.
  Quaternion normalize() const;

  /// \brief Returns the length (or magnitude) of this quaternion.
  f32 length() const;

  /// \brief Returns the magnitude (or length) of this quaternion.
  f32 magnitude() const;

  /// \brief Returns conjugate of this quaternion.
  Quaternion conjugate() const;

  /// \brief Returns inverse of this quaternion.
  Quaternion inverse() const;

  /// \brief Calculates the dot-product between this quaternion and @q.
  f32 dot(const Quaternion &q) const;

 public:
  /// \brief Linearly interpolates between @a and @b based on @t.
  static Quaternion lerp(const Quaternion &a, const Quaternion &b, const f32 t);

  /// \brief Linearly interpolates between @a and @b based on @t with uniform
  /// angular velocity.
  static Quaternion slerp(const Quaternion &a, const Quaternion &b, const f32 t);

 public:
  f32 x, y, z, w;
};

YETI_INLINE Quaternion Quaternion::normalize() const {
  const f32 inverse_of_magnitude = 1.f / magnitude();
  return (*this) * inverse_of_magnitude;
}

YETI_INLINE f32 Quaternion::length() const {
  return sqrtf((*this).dot(*this));
}

YETI_INLINE f32 Quaternion::magnitude() const {
  return sqrtf((*this).dot(*this));
}

YETI_INLINE Quaternion Quaternion::conjugate() const {
  return Quaternion(-x, -y, -z, w);
}

YETI_INLINE Quaternion Quaternion::inverse() const {
  const f32 inverse_of_magnitude = 1.f / magnitude();

  return Quaternion(
    -x * inverse_of_magnitude,
    -y * inverse_of_magnitude,
    -z * inverse_of_magnitude,
     w * inverse_of_magnitude
  );
}

YETI_INLINE f32 Quaternion::dot(const Quaternion &q) const {
  return (x * q.x) + (y * q.y) + (z * q.z) + (w * q.w);
}

YETI_INLINE Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs) {
  return Quaternion(
    (lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y),
    (lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x),
    (lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w),
    (lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z)
  );
}

YETI_INLINE Quaternion operator*(const Quaternion &q, const f32 s) {
  return Quaternion(q.x * s, q.y * s, q.z * s, q.w * s);
}

YETI_INLINE Quaternion Quaternion::from_axis_angle(const Vec3 &axis, const f32 angle) {
  const f32 half_angle = angle * 0.5f;

  const f32 sin_of_half_angle = sinf(half_angle);
  const f32 cos_of_half_angle = cosf(half_angle);

  return Quaternion(
    sin_of_half_angle * axis.x,
    sin_of_half_angle * axis.y,
    sin_of_half_angle * axis.z,
    cos_of_half_angle
  );
}

YETI_INLINE Quaternion Quaternion::from_euler_angles(const f32 x, const f32 y, const f32 z) {
  // PERF(mtwilliams): Derive optimized constructor.

  const Quaternion qx = from_axis_angle(Vec3::X_AXIS, x);
  const Quaternion qy = from_axis_angle(Vec3::Y_AXIS, y);
  const Quaternion qz = from_axis_angle(Vec3::Z_AXIS, z);

  return (qx * qy * qz);
}

YETI_INLINE Quaternion Quaternion::lerp(const Quaternion &a, const Quaternion &b, const f32 t) {
  const f32 one_minus_t =  1.f - t;

  const Quaternion q = Quaternion(
    a.x * one_minus_t + b.x * t,
    a.y * one_minus_t + b.y * t,
    a.z * one_minus_t + b.z * t,
    a.w * one_minus_t + b.w * t
  );

  return q.normalize();
}

YETI_INLINE Quaternion Quaternion::slerp(const Quaternion &a, const Quaternion &b, const f32 t) {
  const f32 one_minus_t = 1.f - t;

  const f32 theta = acosf(a.dot(b));
  const f32 sin_of_theta = sinf(theta);
  const f32 inverse_of_sin_of_theta = 1.f / sin_of_theta;

  const f32 Wa = sinf(one_minus_t * theta) * inverse_of_sin_of_theta;
  const f32 Wb = sinf(t * theta) * inverse_of_sin_of_theta;

  const Quaternion q = Quaternion(
    Wa * a.x + Wb * b.x,
    Wa * a.y + Wb * b.y,
    Wa * a.z + Wb * b.z,
    Wa * a.w + Wb * b.w
  );

  return q.normalize();
}

} // yeti

#endif // _YETI_MATH_QUATERNION_H_
