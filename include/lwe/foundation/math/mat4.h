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

#ifndef _LWE_FOUNDATION_MAT4_H_
#define _LWE_FOUNDATION_MAT4_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>
#include <lwe/foundation/util.h>
#include <lwe/foundation/math/constants.h>
#include <lwe/foundation/math/vec3.h>
#include <lwe/foundation/math/mat3.h>
#include <lwe/foundation/math/quat.h>

#include <math.h>

typedef struct mat4_t {
  public: /* constructors */
    LWE_INLINE mat4_t() {
      memset((void*)&m[0], 0, 16 * sizeof(float));
      m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    LWE_INLINE mat4_t( const mat3_t& mat ) {
      memset((void*)&m[0], 0, 16 * sizeof(float));
      memcpy((void*)&m[0], (void*)&mat.m[0], 3 * sizeof(float));
      memcpy((void*)&m[4], (void*)&mat.m[3], 3 * sizeof(float));
      memcpy((void*)&m[8], (void*)&mat.m[6], 3 * sizeof(float));
      m[15] = 1.0f;
    }

    LWE_INLINE mat4_t( float m00, float m01, float m02, float m03,
                       float m10, float m11, float m12, float m13,
                       float m20, float m21, float m22, float m23,
                       float m30, float m31, float m32, float m33 )
    {
      m[0]  = m00;  m[1] = m01; m[2]  = m02; m[3]  = m03;
      m[4]  = m10;  m[5] = m11; m[6]  = m12; m[7]  = m13;
      m[8]  = m20;  m[9] = m21; m[10] = m22; m[11] = m23;
      m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
    }

    LWE_INLINE mat4_t( const float m_[] ) {
      memcpy((void*)&m[0], (void*)&m_[0], 16 * sizeof(float));
    }

    LWE_INLINE mat4_t( const mat4_t& mat ) {
      memcpy((void*)&m[0], (void*)&mat.m[0], 16 * sizeof(float));
    }

    LWE_INLINE mat4_t& operator= ( const mat4_t& mat ) {
      memcpy((void*)&m[0], (void*)&mat.m[0], 16 * sizeof(float));
      return *this;
    }

  public: /* operator overloads */
    LWE_INLINE mat4_t operator+ ( const mat4_t& mat ) const {
      return mat4_t(
        m[0]  + mat.m[0],  m[1]  + mat.m[1],  m[2]  + mat.m[2],  m[3]  + mat.m[3],
        m[4]  + mat.m[4],  m[5]  + mat.m[5],  m[6]  + mat.m[6],  m[7]  + mat.m[7],
        m[8]  + mat.m[8],  m[9]  + mat.m[9],  m[10] + mat.m[10], m[11] + mat.m[11],
        m[12] + mat.m[12], m[13] + mat.m[13], m[14] + mat.m[14], m[15] + mat.m[15]
      );
    }

    LWE_INLINE mat4_t operator- ( const mat4_t& mat ) const {
      return mat4_t(
        m[0]  - mat.m[0],  m[1]  - mat.m[1],  m[2]  - mat.m[2],  m[3]  - mat.m[3],
        m[4]  - mat.m[4],  m[5]  - mat.m[5],  m[6]  - mat.m[6],  m[7]  - mat.m[7],
        m[8]  - mat.m[8],  m[9]  - mat.m[9],  m[10] - mat.m[10], m[11] - mat.m[11],
        m[12] - mat.m[12], m[13] - mat.m[13], m[14] - mat.m[14], m[15] - mat.m[15]
      );
    }

    LWE_INLINE mat4_t operator* ( const mat4_t& mat ) const {
      return mat4_t(
        m[0]  * mat.m[0] + m[1]  * mat.m[4] + m[2]  * mat.m[8]  + m[3]  * mat.m[12],
        m[0]  * mat.m[1] + m[1]  * mat.m[5] + m[2]  * mat.m[9]  + m[3]  * mat.m[13],
        m[0]  * mat.m[2] + m[1]  * mat.m[6] + m[2]  * mat.m[10] + m[3]  * mat.m[14],
        m[0]  * mat.m[3] + m[1]  * mat.m[7] + m[2]  * mat.m[11] + m[3]  * mat.m[15],
        m[4]  * mat.m[0] + m[5]  * mat.m[4] + m[6]  * mat.m[8]  + m[7]  * mat.m[12],
        m[4]  * mat.m[1] + m[5]  * mat.m[5] + m[6]  * mat.m[9]  + m[7]  * mat.m[13],
        m[4]  * mat.m[2] + m[5]  * mat.m[6] + m[6]  * mat.m[10] + m[7]  * mat.m[14],
        m[4]  * mat.m[3] + m[5]  * mat.m[7] + m[6]  * mat.m[11] + m[7]  * mat.m[15],
        m[8]  * mat.m[0] + m[9]  * mat.m[4] + m[10] * mat.m[8]  + m[11] * mat.m[12],
        m[8]  * mat.m[1] + m[9]  * mat.m[5] + m[10] * mat.m[9]  + m[11] * mat.m[13],
        m[8]  * mat.m[2] + m[9]  * mat.m[6] + m[10] * mat.m[10] + m[11] * mat.m[14],
        m[8]  * mat.m[3] + m[9]  * mat.m[7] + m[10] * mat.m[11] + m[11] * mat.m[15],
        m[12] * mat.m[0] + m[13] * mat.m[4] + m[14] * mat.m[8]  + m[15] * mat.m[12],
        m[12] * mat.m[1] + m[13] * mat.m[5] + m[14] * mat.m[9]  + m[15] * mat.m[13],
        m[12] * mat.m[2] + m[13] * mat.m[6] + m[14] * mat.m[10] + m[15] * mat.m[14],
        m[12] * mat.m[3] + m[13] * mat.m[7] + m[14] * mat.m[11] + m[15] * mat.m[15]
      );
    }

    LWE_INLINE mat4_t operator/ ( const mat4_t& mat ) const {
      return (*this) * mat.inverse();
    }

    LWE_INLINE vec3_t operator* ( const vec3_t& v ) const {
      const float iw = 1.0f / (m[12] * v.x + m[13] * v.y * v.z + m[15]);

      return vec3_t(
        (m[0] * v.x + m[1] * v.y + m[2]  * v.z + m[3])  * iw,
        (m[4] * v.x + m[5] * v.y + m[6]  * v.z + m[7])  * iw,
        (m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11]) * iw
      );
    }

  public: /* methods */
    LWE_INLINE mat4_t transpose() const {
      return mat4_t(
        m[0], m[4], m[8],  m[12],
        m[1], m[5], m[9],  m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
      );
    }

    LWE_INLINE float determinant() const {
      return m[0]  * m[5]  * m[10] * m[15] - m[0]  * m[5]  * m[14] * m[11] + m[0]  * m[9] * m[14] * m[7]  - m[0]  * m[9] * m[6]  * m[15]
           + m[0]  * m[13] * m[6]  * m[11] - m[0]  * m[13] * m[10] * m[7]  - m[4]  * m[9] * m[14] * m[3]  + m[4]  * m[9] * m[2]  * m[15]
           - m[4]  * m[13] * m[2]  * m[11] + m[4]  * m[13] * m[10] * m[3]  - m[4]  * m[1] * m[10] * m[15] + m[4]  * m[1] * m[14] * m[11]
           + m[8]  * m[13] * m[2]  * m[7]  - m[8]  * m[13] * m[6]  * m[3]  + m[8]  * m[1] * m[6]  * m[15] - m[8]  * m[1] * m[14] * m[7]
           + m[8]  * m[5]  * m[14] * m[3]  - m[8]  * m[5]  * m[2]  * m[15] - m[12] * m[1] * m[6]  * m[11] + m[12] * m[1] * m[10] * m[7]
           - m[12] * m[5]  * m[10] * m[3]  + m[12] * m[5]  * m[2]  * m[11] - m[12] * m[9] * m[2]  * m[7]  + m[12] * m[9] * m[6]  * m[3];
    }

    LWE_INLINE mat4_t inverse() const {
      const float id = 1.0f / determinant();

      return mat4_t(
        id  * (m[5] * (m[10] * m[15] - m[14] * m[11]) + m[9] * (m[14] * m[7] - m[6] * m[15]) + m[13] * (m[6] * m[11] - m[10] * m[7])),
        -id * (m[4] * (m[10] * m[15] - m[14] * m[11]) + m[8] * (m[14] * m[7] - m[6] * m[15]) + m[12] * (m[6] * m[11] - m[10] * m[7])),
        id  * (m[4] * (m[9]  * m[15] - m[13] * m[11]) + m[8] * (m[13] * m[7] - m[5] * m[15]) + m[12] * (m[5] * m[11] - m[9]  * m[7])),
        -id * (m[4] * (m[9]  * m[14] - m[13] * m[10]) + m[8] * (m[13] * m[6] - m[5] * m[14]) + m[12] * (m[5] * m[10] - m[9]  * m[6])),
        -id * (m[1] * (m[10] * m[15] - m[14] * m[11]) + m[9] * (m[14] * m[3] - m[2] * m[15]) + m[13] * (m[2] * m[11] - m[10] * m[3])),
        id  * (m[0] * (m[10] * m[15] - m[14] * m[11]) + m[8] * (m[14] * m[3] - m[2] * m[15]) + m[12] * (m[2] * m[11] - m[10] * m[3])),
        -id * (m[0] * (m[9]  * m[15] - m[13] * m[11]) + m[8] * (m[13] * m[3] - m[1] * m[15]) + m[12] * (m[1] * m[11] - m[9]  * m[3])),
        id  * (m[0] * (m[9]  * m[14] - m[13] * m[10]) + m[8] * (m[13] * m[2] - m[1] * m[14]) + m[12] * (m[1] * m[10] - m[9]  * m[2])),
        id  * (m[1] * (m[6]  * m[15] - m[14] * m[7])  + m[5] * (m[14] * m[3] - m[2] * m[15]) + m[13] * (m[2] * m[7]  - m[6]  * m[3])),
        -id * (m[0] * (m[6]  * m[15] - m[14] * m[7])  + m[4] * (m[14] * m[3] - m[2] * m[15]) + m[12] * (m[2] * m[7]  - m[6]  * m[3])),
        id  * (m[0] * (m[5]  * m[15] - m[13] * m[7])  + m[4] * (m[13] * m[3] - m[1] * m[15]) + m[12] * (m[1] * m[7]  - m[5]  * m[3])),
        -id * (m[0] * (m[5]  * m[14] - m[13] * m[6])  + m[4] * (m[13] * m[2] - m[1] * m[14]) + m[12] * (m[1] * m[6]  - m[5]  * m[2])),
        -id * (m[1] * (m[6]  * m[11] - m[10] * m[7])  + m[5] * (m[10] * m[3] - m[2] * m[11]) + m[9]  * (m[2] * m[7]  - m[6]  * m[3])),
        id  * (m[0] * (m[6]  * m[11] - m[10] * m[7])  + m[4] * (m[10] * m[3] - m[2] * m[11]) + m[8]  * (m[2] * m[7]  - m[6]  * m[3])),
        -id * (m[0] * (m[5]  * m[11] - m[9]  * m[7])  + m[4] * (m[9]  * m[3] - m[1] * m[11]) + m[8]  * (m[1] * m[7]  - m[5]  * m[3])),
        id  * (m[0] * (m[5]  * m[10] - m[9]  * m[6])  + m[4] * (m[9]  * m[2] - m[1] * m[10]) + m[8]  * (m[1] * m[6]  - m[5]  * m[2]))
      );
    }

    /// Decomposes a matrix into translation, rotation, an scale components.
    void decompose( vec3_t& translation, quat_t& rotation, vec3_t& scale ) const
    {
      // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
      // http://www.gamedev.net/topic/441695-transform-matrix-decomposition/
      
      translation.x = m[3];
      translation.y = m[7];
      translation.z = m[11];

      scale.x = vec3_t(m[0], m[1], m[2]).magnitude();
      scale.y = vec3_t(m[4], m[5], m[6]).magnitude();
      scale.z = vec3_t(m[8], m[9], m[10]).magnitude();

      const float det =
        m[0] * (m[5] * m[10] - m[6] * m[9]) +
        m[1] * (m[6] * m[8] - m[4] * m[10]) +
        m[2] * (m[4] * m[9] - m[5] * m[8]);

      const float aq2 = pow(det, (1.0f / 3.0f));

      rotation.w = sqrt(lwe_max(0.0f, aq2 + m[0] + m[5] + m[10])) / 2.0f;
      rotation.x = sqrt(lwe_max(0.0f, aq2 + m[0] - m[5] - m[10])) / 2.0f;
      rotation.y = sqrt(lwe_max(0.0f, aq2 - m[0] + m[5] - m[10])) / 2.0f;
      rotation.z = sqrt(lwe_max(0.0f, aq2 - m[0] - m[5] + m[10])) / 2.0f;
    }

  public: /* constructor methods */
    LWE_INLINE static mat4_t identity() {
      return mat4_t();
    }

    /// Creates a translation matrix.
    static mat4_t translate( const vec3_t& t ) {
      return mat4_t(
        1.0f, 0.0f, 0.0f, t.x,
        0.0f, 1.0f, 0.0f, t.y,
        0.0f, 0.0f, 1.0f, t.z,
        0.0f, 0.0f, 0.0f, 1.0f
      );
    }

    /// Creates a rotation matrix from a quaternion.
    static mat4_t rotate( const quat_t& q ) {
      return mat4_t(
        1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z, 2.0f * q.x * q.y - 2.0f * q.z * q.w, 2.0f * q.x * q.z + 2.0f * q.y * q.w, 0.0f,
        2.0f * q.x * q.y + 2.0f * q.z * q.w, 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z, 2.0f * q.y * q.z - 2.0f * q.x * q.w, 0.0f,
        2.0f * q.x * q.z - 2.0f * q.y * q.w, 2.0f * q.y * q.z + 2.0f * q.x * q.w, 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
      );
    }

    /// Creates a scale matrix.
    static mat4_t scale( const vec3_t& s ) {
      return mat4_t(
        s.x,  0.0f, 0.0f, 0.0f,
        0.0f, s.y,  0.0f, 0.0f,
        0.0f, 0.0f, s.z,  0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
      );
    }

    /// Creates a translation, rotation, and scale matrix.
    LWE_INLINE static mat4_t translate_rotate_scale(
      const vec3_t& translation,
      const quat_t& rotation,
      const vec3_t& scale )
    {
      return mat4_t::scale(scale) *
             mat4_t::rotate(rotation) *
             mat4_t::translate(translation);
    }

    /// Creates a right-handed orthographic projection matrix.
    static mat4_t orthographic(
      float min_x, float max_x,
      float min_y, float max_y,
      float near_plane, float far_plane )
    {
      const float dx = max_x - min_x;
      const float dy = max_y - min_y;
      const float dz = near_plane - far_plane;

      return mat4_t(
        2.0f / dx, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / dy, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f / dz, 0.0f,
        0.0f, 0.0f, near_plane / dz, 1.0f
      );
    }

    /// Creates a right-handed perspective projection matrix.
    static mat4_t perspective(
      float field_of_view,
      float aspect_ratio,
      float near_plane,
      float far_plane )
    {
      const float ys = tan(LWE_PI_2 - (field_of_view / 2.0f));
      const float xs = ys / aspect_ratio;
      const float dz = near_plane - far_plane;

      return mat4_t(
        xs, 0.0f, 0.0f, 0.0f,
        0.0f, ys, 0.0f, 0.0f,
        0.0f, 0.0f, (far_plane - near_plane) / dz, (2 * far_plane * near_plane) / dz,
        0.0f, 0.0f, -1.0f, 0.0f
      );
    }

    /// Creates a right-handed look-at matrix.
    static mat4_t look_at(
      const vec3_t& eye,
      const vec3_t& target,
      const vec3_t& up )
    {
      const vec3_t z_axis = (eye - target).normalize();
      const vec3_t x_axis = up.cross(z_axis).normalize();
      const vec3_t y_axis = z_axis.cross(x_axis);

      return mat4_t(
        x_axis.x, y_axis.x, z_axis.x, 0.0f,
        x_axis.y, y_axis.y, z_axis.y, 0.0f,
        x_axis.z, y_axis.z, z_axis.z, 0.0f,
        -x_axis.dot(eye), -y_axis.dot(eye), -z_axis.dot(eye), 1.0f
      );
    }

  public:
    float m[16];
} mat4_t;

#endif // _LWE_FOUNDATION_MAT4_H_