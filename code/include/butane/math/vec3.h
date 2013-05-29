// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_VEC3_H_
#define _BUTANE_MATH_VEC3_H_

#include <butane/butane.h>

#include <math.h>

namespace butane {
  class BUTANE_EXPORT Vec3f final {
    public:
      FOUNDATION_INLINE Vec3f(
        const float x = 0.0f,
        const float y = 0.0f,
        const float z = 0.0f
      ) : x(x), y(y), z(z)
      {}

      FOUNDATION_INLINE Vec3f(
        const Vec3f& v
      ) : x(v.x), y(v.y), z(v.z)
      {}

      FOUNDATION_INLINE Vec3f& operator= (
        const Vec3f& v )
      { x = v.x; y = v.y; z = v.z; return *this; }

    public:
      FOUNDATION_INLINE Vec3f operator+ (
        const Vec3f& v ) const
      { return Vec3f(x + v.x, y + v.y, z + v.z); }

      FOUNDATION_INLINE Vec3f operator- (
        const Vec3f& v ) const
      { return Vec3f(x - v.x, y - v.y, z - v.z); }

      FOUNDATION_INLINE Vec3f operator* (
        const Vec3f& v ) const
      { return Vec3f(x * v.x, y * v.y, z * v.z); }

      FOUNDATION_INLINE Vec3f operator/ (
        const Vec3f& v ) const
      { return Vec3f(x / v.x, y / v.y, z / v.z); }

      FOUNDATION_INLINE Vec3f& operator+= (
        const Vec3f& v )
      { x += v.x; y += v.y; z += v.z; return *this; }

      FOUNDATION_INLINE Vec3f& operator-= (
        const Vec3f& v )
      { x -= v.x; y -= v.y; z -= v.z; return *this; }

      FOUNDATION_INLINE Vec3f& operator*= (
        const Vec3f& v )
      { x *= v.x; y *= v.y; z *= v.z; return *this; }

      FOUNDATION_INLINE Vec3f& operator/= (
        const Vec3f& v )
      { x /= v.x; y /= v.y; z /= v.z; return *this; }

    public:
      FOUNDATION_INLINE float length() const
      { return sqrtf(dot(*this)); }

      FOUNDATION_INLINE float magnitude() const
      { return sqrtf(dot(*this)); }

      FOUNDATION_INLINE float dot(
        const Vec3f& v ) const
      { return (x * v.x + y * v.y + z * v.z); }

      FOUNDATION_INLINE Vec3f cross(
        const Vec3f& v ) const
      { return Vec3f((y * v.z) - (z * v.y), (x * v.z) - (z * v.x), (x * v.y) + (y * v.x)); }

      FOUNDATION_INLINE Vec3f normalize() const
      { return (*this) / magnitude(); }

    public:
      float x, y, z;
  };
} // butane

#endif // _BUTANE_MATH_VEC3_H_
