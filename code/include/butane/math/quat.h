// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_QUAT_H_
#define _BUTANE_MATH_QUAT_H_

#include <butane/butane.h>
#include <butane/math/vec3.h>

#include <math.h>

namespace butane {
  class BUTANE_EXPORT Quatf final {
    public:
      FOUNDATION_INLINE Quatf(
        const float w = 1.0f,
        const float x = 0.0f,
        const float y = 0.0f,
        const float z = 0.0f
      ) : w(w), x(x), y(y), z(z)
      {}

      FOUNDATION_INLINE Quatf(
        const Quatf& q
      ) : w(q.w), x(q.x), y(q.y), z(q.z)
      {}

      FOUNDATION_INLINE Quatf& operator= (
        const Quatf& q )
      { w = q.w; x = q.x; y = q.y; z = q.z; return *this; }

    public:
      static Quatf from_axis_angle(
        const Vec3f& axis,
        const float angle_in_radians )
      {
        const float half_angle = angle_in_radians / 2.0f;
        const float sin_of_half_angle = sinf(half_angle);
        const float cos_of_half_angle = cosf(half_angle);

        return Quatf(
          cos_of_half_angle,
          axis.x * sin_of_half_angle,
          axis.y * sin_of_half_angle,
          axis.z * sin_of_half_angle);
      }

      static Quatf from_euler_angles(
        const float x_in_radians,
        const float y_in_radians,
        const float z_in_radians )
      {
        const Quatf qx = from_axis_angle(Vec3f(1.0f, 0.0f, 0.0f), x_in_radians);
        const Quatf qy = from_axis_angle(Vec3f(0.0f, 1.0f, 0.0f), y_in_radians);
        const Quatf qz = from_axis_angle(Vec3f(0.0f, 0.0f, 1.0f), z_in_radians);
        return (qx * qy * qz);
      }

    public:
      FOUNDATION_INLINE Quatf operator+ (
        const Quatf& q ) const
      { return Quatf(w + q.w, x + q.x, y + q.y, z + q.z); }

      FOUNDATION_INLINE Quatf operator- (
        const Quatf& q ) const
      { return Quatf(w - q.w, x - q.x, y - q.y, z - q.z); }

      FOUNDATION_INLINE Quatf operator* (
        const Quatf& q ) const
      {
        return Quatf(
          (w * q.w - x * q.x - y * q.y - z * q.z),
          (w * q.x + x * q.w + y * q.z - z * q.y),
          (w * q.y - x * q.z + y * q.w + z * q.x),
          (w * q.z + x * q.y - y * q.x + z * q.w));
      }

      FOUNDATION_INLINE Quatf operator/ (
        const Quatf& q ) const
      { return (*this * q.inverse()); }

      FOUNDATION_INLINE Quatf& operator+= (
        const Quatf& q )
      { w += q.w; x += q.x; y += q.y; z += q.z; return *this; }

      FOUNDATION_INLINE Quatf& operator-= (
        const Quatf& q )
      { w -= q.w; x -= q.x; y -= q.y; z -= q.z; return *this; }

      FOUNDATION_INLINE Quatf& operator*= (
        const Quatf& q )
      {
        w = (w * q.w - x * q.x - y * q.y - z * q.z);
        x = (w * q.x + x * q.w + y * q.z - z * q.y);
        y = (w * q.y - x * q.z + y * q.w + z * q.x);
        z = (w * q.z + x * q.y - y * q.x + z * q.w);

        return *this;
      }

      FOUNDATION_INLINE Quatf& operator/= (
        const Quatf& q )
      {
        (*this) *= q.inverse();
        return *this;
      }

    public:
      FOUNDATION_INLINE float length() const
      { return sqrtf(dot(*this)); }

      FOUNDATION_INLINE float magnitude() const
      { return sqrtf(dot(*this)); }

      FOUNDATION_INLINE float dot(
        const Quatf& q ) const
      { return (w * q.w + x * q.x + y * q.y + z); }

      FOUNDATION_INLINE Quatf normalize() const
      { return (*this) / magnitude(); }

      FOUNDATION_INLINE Quatf conjugate() const
      { return Quatf(w, -x, -y, -z); }

      FOUNDATION_INLINE Quatf inverse() const
      {
        const float m = magnitude();
        return Quatf(w/m, -x/m, -y/m, -z/m);
      }

    public:
      float w, x, y, z;
  };
} // butane

#endif // _BUTANE_MATH_QUAT_H_
