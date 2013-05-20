// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_MAT3_H_
#define _BUTANE_MATH_MAT3_H_

#include <butane/foundation.h>
#include <butane/config.h>
#include <butane/math/quat.h>

#include <math.h>

namespace butane {
  class BUTANE_EXPORT Mat3 final {
    public:
      FOUNDATION_INLINE Mat3()
      {
        zero((void*)&m[0], sizeof(m));
        m[0] = m[4] = m[8] = 1.0f;
      }

      FOUNDATION_INLINE Mat3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22 )
      {
        m[0] = m00; m[1] = m01; m[2] = m02;
        m[3] = m10; m[4] = m11; m[5] = m12;
        m[6] = m20; m[7] = m21; m[8] = m22;
      }

      FOUNDATION_INLINE Mat3(
        const float m_[] )
      { copy((void*)&m[0], (const void*)&m_[0], sizeof(m)); }

      FOUNDATION_INLINE Mat3(
        const Mat3& m_ )
      { copy((void*)&m[0], (const void*)&m_.m[0], sizeof(m)); }

      FOUNDATION_INLINE Mat3& operator= (
        const Mat3& m_ )
      { copy((void*)&m[0], (const void*)&m_.m[0], sizeof(m)); return *this; }

    public:
      static Mat3 from_euler_angles(
        const float x_in_radians,
        const float y_in_radians,
        const float z_in_radians )
      {
        const Mat3 x = Mat3(
          1.0f, 0.0f, 0.0f,
          0.0f, cosf(x_in_radians), -sinf(x_in_radians),
          0.0f, sinf(x_in_radians), cosf(x_in_radians));

        const Mat3 y = Mat3(
          cosf(y_in_radians), 0.0f, sinf(y_in_radians),
          0.0f, 1.0f, 0.0f,
          -sinf(y_in_radians), 0.0f, cosf(y_in_radians));

        const Mat3 z = Mat3(
          cosf(z_in_radians), sinf(z_in_radians), 0.0f,
          -sinf(z_in_radians), cosf(z_in_radians), 0.0f,
          0.0f, 0.0f, 1.0f);

        return (x * y * z);
      }

      static Mat3 from_quat(
        const Quat& q )
      {
        return Mat3(
          1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z,
          2.0f * q.x * q.y - 2.0f * q.z * q.w,
          2.0f * q.x * q.z + 2.0f * q.y * q.w,
          2.0f * q.x * q.y + 2.0f * q.z * q.w,
          1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z,
          2.0f * q.y * q.z - 2.0f * q.x * q.w,
          2.0f * q.x * q.z - 2.0f * q.y * q.w,
          2.0f * q.y * q.z + 2.0f * q.x * q.w,
          1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y);
      }

    public:
      FOUNDATION_INLINE Mat3 operator+ (
        const Mat3& m_ ) const
      {
        return Mat3(
          m[0] + m_.m[0], m[1] + m_.m[1], m[2] + m_.m[2],
          m[3] + m_.m[3], m[4] + m_.m[4], m[5] + m_.m[5],
          m[6] + m_.m[6], m[7] + m_.m[7], m[8] + m_.m[8]);
      }

      FOUNDATION_INLINE Mat3 operator- ( const Mat3& m_ ) const {
        return Mat3(
          m[0] - m_.m[0], m[1] - m_.m[1], m[2] - m_.m[2],
          m[3] - m_.m[3], m[4] - m_.m[4], m[5] - m_.m[5],
          m[6] - m_.m[6], m[7] - m_.m[7], m[8] - m_.m[8]);
      }

      FOUNDATION_INLINE Mat3 operator* ( const Mat3& m_ ) const {
        return Mat3(
          m[0] * m_.m[0] + m[1] * m_.m[3] + m[2] + m_.m[6],
          m[0] * m_.m[1] + m[1] * m_.m[4] + m[2] * m_.m[7],
          m[0] * m_.m[2] + m[1] * m_.m[5] + m[2] * m_.m[8],
          m[3] * m_.m[0] + m[4] * m_.m[3] + m[5] + m_.m[6],
          m[3] * m_.m[1] + m[4] * m_.m[4] + m[5] + m_.m[7],
          m[3] * m_.m[2] + m[4] * m_.m[5] + m[5] + m_.m[8],
          m[6] * m_.m[0] + m[7] * m_.m[3] + m[8] + m_.m[6],
          m[6] * m_.m[1] + m[7] * m_.m[4] + m[8] + m_.m[7],
          m[6] * m_.m[2] + m[7] * m_.m[5] + m[8] + m_.m[8]);
      }

      FOUNDATION_INLINE Mat3 operator/ (
        const Mat3& m_ ) const
      { return ((*this) * m_.inverse()); }

    public:
      FOUNDATION_INLINE Mat3 transpose() const
      {
        return Mat3(
          m[0], m[3], m[6],
          m[1], m[4], m[7],
          m[2], m[5], m[8]);
      }

      FOUNDATION_INLINE float determinant() const
      {
        return (
          m[0] * (m[4] * m[8] - m[5] * m[7]) +
          m[1] * (m[5] * m[6] - m[3] * m[8]) +
          m[2] * (m[3] * m[7] - m[4] * m[6]));
      }

      FOUNDATION_INLINE Mat3 inverse() const
      {
        const float d = determinant();
        return Mat3(
          (m[4] * m[8] - m[5] * m[7]) * d,
          (m[2] * m[7] - m[1] * m[8]) * d,
          (m[1] * m[5] - m[2] * m[4]) * d,
          (m[5] * m[6] - m[3] * m[8]) * d,
          (m[0] * m[8] - m[2] * m[6]) * d,
          (m[2] * m[3] - m[0] * m[5]) * d,
          (m[3] * m[7] - m[4] * m[6]) * d,
          (m[1] * m[6] - m[0] * m[7]) * d,
          (m[0] * m[4] - m[1] * m[3]) * d);
      }

    public:
      float m[9];
  };
} // butane

#endif // _BUTANE_MATH_MAT3_H_
