// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_VEC2_H_
#define _BUTANE_MATH_VEC2_H_

#include <butane/butane.h>

#include <math.h>

namespace butane {
  class BUTANE_EXPORT Vec2f final {
    public:
      FOUNDATION_INLINE Vec2f(
        const float x = 0.0f,
        const float y = 0.0f
      ) : x(x), y(y)
      {}

      FOUNDATION_INLINE Vec2f(
        const Vec2f& v
      ) : x(v.x), y(v.y)
      {}

      FOUNDATION_INLINE Vec2f& operator= (
        const Vec2f& v )
      { x = v.x; y = v.y; return *this; }

    public:
      FOUNDATION_INLINE Vec2f operator+ (
        const Vec2f& v ) const
      { return Vec2f(x + v.x, y + v.y); }

      FOUNDATION_INLINE Vec2f operator- (
        const Vec2f& v ) const
      { return Vec2f(x - v.x, y - v.y); }

      FOUNDATION_INLINE Vec2f operator* (
        const Vec2f& v ) const
      { return Vec2f(x * v.x, y * v.y); }

      FOUNDATION_INLINE Vec2f operator/ (
        const Vec2f& v ) const
      { return Vec2f(x / v.x, y / v.y); }

      FOUNDATION_INLINE Vec2f& operator+= (
        const Vec2f& v )
      { x += v.x; y += v.y; return *this; }

      FOUNDATION_INLINE Vec2f& operator-= (
        const Vec2f& v )
      { x -= v.x; y -= v.y; return *this; }

      FOUNDATION_INLINE Vec2f& operator*= (
        const Vec2f& v )
      { x *= v.x; y *= v.y; return *this; }

      FOUNDATION_INLINE Vec2f& operator/= (
        const Vec2f& v )
      { x /= v.x; y /= v.y; return *this; }

    public:
      FOUNDATION_INLINE float length() const
      { return sqrtf(dot(*this)); }

      FOUNDATION_INLINE float magnitude() const
      { return sqrtf(dot(*this)); }

      FOUNDATION_INLINE float dot(
        const Vec2f& v ) const
      { return (x * v.x + y * v.y); }

      FOUNDATION_INLINE Vec2f normalize() const
      { return (*this) / magnitude(); }

    public:
      float x, y;
  };
} // butane

#endif // _BUTANE_MATH_VEC2_H_
