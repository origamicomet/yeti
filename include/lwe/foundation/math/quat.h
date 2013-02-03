// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#ifndef _LWE_FOUNDATION_QUAT_H_
#define _LWE_FOUNDATION_QUAT_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>
#include <lwe/foundation/math/vec3.h>

#include <math.h>

typedef struct quat_t {
  public: /* constructors */
    LWE_INLINE quat_t()
      : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
    {}

    LWE_INLINE quat_t( float x, float y, float z, float w )
      : x(x), y(y), z(z), w(w)
    {}

    LWE_INLINE quat_t( const quat_t& q )
      : x(q.x), y(q.y), z(q.z), w(q.w)
    {}

    LWE_INLINE quat_t( const vec3_t& axis, float angle ) {
      const float s = sin(angle);
      const float c = cos(angle);

      x = axis.x * s;
      y = axis.y * s;
      z = axis.z * s;
      w = axis.z * c;
    }

    LWE_INLINE quat_t& operator=( const quat_t& q ) {
      x = q.x; y = q.y; z = q.z; w = q.w;
      return *this;
    }

    LWE_INLINE quat_t operator+ ( const quat_t& q ) const {
      return quat_t(x + q.x, y + q.y, z + q.z, w + q.w);
    }

    LWE_INLINE quat_t operator- ( const quat_t& q ) const {
      return quat_t(x - q.x, y - q.y, z - q.z, w - q.w);
    }

    LWE_INLINE quat_t operator* ( const quat_t& q ) const {
      return quat_t(
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y + y * q.w + z * q.x - x * q.z,
        w * q.z + z * q.w + x * q.y - y * q.x,
        w * q.w - x * q.x - y * q.y - z * q.z
      );
    }

    LWE_INLINE quat_t operator/ ( const quat_t& q ) const {
      return *this * q.inverse();
    }

    LWE_INLINE quat_t& operator+= ( const quat_t& q ) {
      x += q.x; y += q.y; z += q.z; w += q.w;
      return *this;
    }

    LWE_INLINE quat_t& operator-= ( const quat_t& q ) {
      x -= q.x; y -= q.y; z -= q.z; w -= q.w;
      return *this;
    }

    LWE_INLINE quat_t& operator*= ( const quat_t& q ) {
      x = w * q.x + x * q.w + y * q.z - z * q.y;
      y = w * q.y + y * q.w + z * q.x - x * q.z;
      z = w * q.z + z * q.w + x * q.y - y * q.x;
      w = w * q.w - x * q.x - y * q.y - z * q.z;
      return *this;
    }

    LWE_INLINE quat_t& operator/= ( const quat_t& q ) {
      const quat_t iq = q.inverse();
      x = w * iq.x + x * iq.w + y * iq.z - z * iq.y;
      y = w * iq.y + y * iq.w + z * iq.x - x * iq.z;
      z = w * iq.z + z * iq.w + x * iq.y - y * iq.x;
      w = w * iq.w - x * iq.x - y * iq.y - z * iq.z;
      return *this;
    }

    LWE_INLINE float magnitude() const {
      return sqrt(x * x + y * y + z * z + w * w);
    }

    LWE_INLINE quat_t normalize() const {
      const float im = 1.0f / magnitude();
      return quat_t(x * im, y * im, z * im, w * im);
    }

    LWE_INLINE quat_t conjugate() const {
      return quat_t(-x, -y, -z, w);
    }

    LWE_INLINE quat_t inverse() const {
      const float im = 1.0f / magnitude();
      return quat_t(-x * im, -y * im, -z * im, w * im);
    }

    LWE_INLINE float dot( const quat_t& q ) const {
      return x * q.x + y * q.y + z * q.z + w * q.w;
    }

  public: /* constructor methods */
    /// Constructs a quaternion from an angle-axis representation.
    static quat_t from_angle_axis( const vec3_t& axis, float angle ) {
      const float half_angle = angle * 0.5f;
      const float sin_of_half_angle = sin(half_angle);

      return quat_t(
        sin_of_half_angle * axis.x,
        sin_of_half_angle * axis.y,
        sin_of_half_angle * axis.z,
        cos(half_angle)
      );
    }

    /// Constructs a quaternion from euler angles (in radians),
    static quat_t from_euler_angles( float ex, float ey, float ez ) {
      const quat_t qx = from_angle_axis(vec3_t(1.0f, 0.0f, 0.0f), ex);
      const quat_t qy = from_angle_axis(vec3_t(0.0f, 1.0f, 0.0f), ey);
      const quat_t qz = from_angle_axis(vec3_t(0.0f, 0.0f, 1.0f), ez);
      return qx * qy * qz;
    }

  public:
    float x, y, z, w;
} quat_t;

#endif // _LWE_FOUNDATION_QUAT_H_