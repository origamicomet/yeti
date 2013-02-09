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

#ifndef _LWE_FOUNDATION_MAT3_H_
#define _LWE_FOUNDATION_MAT3_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>
#include <lwe/foundation/math/quat.h>

typedef struct mat3_t {
  public: /* constructors */
    LWE_INLINE mat3_t() {
      memset((void*)&m[0], 0, 9 * sizeof(float));
      m[0] = m[4] = m[8] = 1.0f;
    }

    LWE_INLINE mat3_t( float m00, float m01, float m02,
                       float m10, float m11, float m12,
                       float m20, float m21, float m22 )
    {
      m[0] = m00; m[1] = m01; m[2] = m02;
      m[3] = m10; m[4] = m11; m[5] = m12;
      m[6] = m20; m[7] = m21; m[8] = m22;
    }

    LWE_INLINE mat3_t( const float m_[] ) {
      memcpy((void*)&m[0], (void*)&m_[0], 9 * sizeof(float));
    }

    LWE_INLINE mat3_t( const mat3_t& mat ) {
      memcpy((void*)&m[0], (void*)&mat.m[0], 9 * sizeof(float));
    }

    LWE_INLINE mat3_t& operator= ( const mat3_t& mat ) {
      memcpy((void*)&m[0], (void*)&mat.m[0], 9 * sizeof(float));
      return *this;
    }

  public: /* operator overloads */
    LWE_INLINE mat3_t operator+ ( const mat3_t& mat ) const {
      return mat3_t(
        m[0] + mat.m[0], m[1] + mat.m[1], m[2] + mat.m[2],
        m[3] + mat.m[3], m[4] + mat.m[4], m[5] + mat.m[5],
        m[6] + mat.m[6], m[7] + mat.m[7], m[8] + mat.m[8]
      );
    }

    LWE_INLINE mat3_t operator- ( const mat3_t& mat ) const {
      return mat3_t(
        m[0] - mat.m[0], m[1] - mat.m[1], m[2] - mat.m[2],
        m[3] - mat.m[3], m[4] - mat.m[4], m[5] - mat.m[5],
        m[6] - mat.m[6], m[7] - mat.m[7], m[8] - mat.m[8]
      );
    }

    LWE_INLINE mat3_t operator* ( const mat3_t& mat ) const {
      return mat3_t(
        m[0] * mat.m[0] + m[1] * mat.m[3] + m[2] + mat.m[6],
        m[0] * mat.m[1] + m[1] * mat.m[4] + m[2] * mat.m[7],
        m[0] * mat.m[2] + m[1] * mat.m[5] + m[2] * mat.m[8],
        m[3] * mat.m[0] + m[4] * mat.m[3] + m[5] + mat.m[6],
        m[3] * mat.m[1] + m[4] * mat.m[4] + m[5] + mat.m[7],
        m[3] * mat.m[2] + m[4] * mat.m[5] + m[5] + mat.m[8],
        m[6] * mat.m[0] + m[7] * mat.m[3] + m[8] + mat.m[6],
        m[6] * mat.m[1] + m[7] * mat.m[4] + m[8] + mat.m[7],
        m[6] * mat.m[2] + m[7] * mat.m[5] + m[8] + mat.m[8]
      );
    }

    LWE_INLINE mat3_t operator/ ( const mat3_t& mat ) const {
      return (*this) * mat.inverse();
    }

  public: /* methods */
    LWE_INLINE mat3_t transpose() const {
      return mat3_t(
        m[0], m[3], m[6],
        m[1], m[4], m[7],
        m[2], m[5], m[8]
      );
    }

    LWE_INLINE float determinant() const {
      return m[0] * (m[4] * m[8] - m[5] * m[7]) + m[1] * (m[5] * m[6] - m[3] * m[8]) + m[2] * (m[3] * m[7] - m[4] * m[6]);
    }

    LWE_INLINE mat3_t inverse() const {
      const float id = 1.0f / determinant();

      return mat3_t(
        (m[4] * m[8] - m[5] * m[7]) * id,
        (m[2] * m[7] - m[1] * m[8]) * id,
        (m[1] * m[5] - m[2] * m[4]) * id,
        (m[5] * m[6] - m[3] * m[8]) * id,
        (m[0] * m[8] - m[2] * m[6]) * id,
        (m[2] * m[3] - m[0] * m[5]) * id,
        (m[3] * m[7] - m[4] * m[6]) * id,
        (m[1] * m[6] - m[0] * m[7]) * id,
        (m[0] * m[4] - m[1] * m[3]) * id
      );
    }

  public: /* constructor methods */
    /// Creates a matrix representing a rotation on the x-axis in radians.
    static mat3_t rotate_x( float r ) {
      return mat3_t(
        1.0f, 0.0f, 0.0f,
        0.0f, cos(r), -sin(r),
        0.0f, sin(r), cos(r)
      );
    }

    /// Creates a matrix representing a rotation on the y-axis in radians.
    static mat3_t rotate_y( float r ) {
      return mat3_t(
        cos(r), 0.0f, sin(r),
        0.0f, 1.0f, 0.0f,
        -sin(r), 0.0f, cos(r)
      );
    }

    /// Creates a matrix representing a rotation on the z-axis in radians.
    static mat3_t rotate_z( float r ) {
      return mat3_t(
        cos(r), sin(r), 0.0f,
        -sin(r), cos(r), 0.0f,
        0.0f, 0.0f, 1.0f
      );
    }

    /// Creates a rotation matrix from a quaternion.
    static mat3_t rotate( const quat_t& q ) {
      return mat3_t(
        1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z, 2.0f * q.x * q.y - 2.0f * q.z * q.w, 2.0f * q.x * q.z + 2.0f * q.y * q.w,
        2.0f * q.x * q.y + 2.0f * q.z * q.w, 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z, 2.0f * q.y * q.z - 2.0f * q.x * q.w,
        2.0f * q.x * q.z - 2.0f * q.y * q.w, 2.0f * q.y * q.z + 2.0f * q.x * q.w, 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y
      );
    }

  public:
    float m[9];
} mat3_t;

#endif // _LWE_FOUNDATION_MAT3_H_