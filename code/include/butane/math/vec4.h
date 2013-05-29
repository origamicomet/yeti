// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_VEC4_H_
#define _BUTANE_MATH_VEC4_H_

#include <butane/butane.h>

#include <math.h>

namespace butane {
  class BUTANE_EXPORT Vec4f final {
    public:
      FOUNDATION_INLINE Vec4f(
        const float x = 0.0f,
        const float y = 0.0f,
        const float z = 0.0f,
        const float w = 0.0f
      ) : x(x), y(y), z(z), w(w)
      {}

      FOUNDATION_INLINE Vec4f(
        const Vec4f& v
      ) : x(v.x), y(v.y), z(v.z), w(v.w)
      {}

      FOUNDATION_INLINE Vec4f& operator= (
        const Vec4f& v )
      { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

    public:
      FOUNDATION_INLINE Vec4f operator+ (
        const Vec4f& v ) const
      { return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w); }

      FOUNDATION_INLINE Vec4f operator- (
        const Vec4f& v ) const
      { return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w); }

      FOUNDATION_INLINE Vec4f operator* (
        const Vec4f& v ) const
      { return Vec4f(x * v.x, y * v.y, z * v.z, w * v.w); }

      FOUNDATION_INLINE Vec4f operator/ (
        const Vec4f& v ) const
      { return Vec4f(x / v.x, y / v.y, z / v.z, w / v.w); }

      FOUNDATION_INLINE Vec4f& operator+= (
        const Vec4f& v )
      { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }

      FOUNDATION_INLINE Vec4f& operator-= (
        const Vec4f& v )
      { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

      FOUNDATION_INLINE Vec4f& operator*= (
        const Vec4f& v )
      { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }

      FOUNDATION_INLINE Vec4f& operator/= (
        const Vec4f& v )
      { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

    public:
      FOUNDATION_INLINE float length() const
      { return sqrtf(dot(*this)); }

      FOUNDATION_INLINE float magnitude() const
      { return sqrtf(dot(*this)); }

      FOUNDATION_INLINE float dot(
        const Vec4f& v ) const
      { return (x * v.x + y * v.y + z * v.z + w * v.w); }

      FOUNDATION_INLINE Vec4f normalize() const
      { return (*this) / magnitude(); }

    public:
      float x, y, z, w;
  };
} // butane

#endif // _BUTANE_MATH_VEC4_H_
