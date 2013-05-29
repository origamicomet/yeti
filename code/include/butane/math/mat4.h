// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_MAT4_H_
#define _BUTANE_MATH_MAT4_H_

#include <butane/butane.h>
#include <butane/math/mat3.h>
#include <butane/math/constants.h>

#include <math.h>

namespace butane {
  class BUTANE_EXPORT Mat4 final {
    public:
      FOUNDATION_INLINE Mat4()
      {
        zero((void*)&m[0], sizeof(m));
        m[0] = m[5] = m[10] = m[15] = 1.0f;
      }

      FOUNDATION_INLINE Mat4(
        const Mat3& m_ )
      {
        zero((void*)&m[0], sizeof(m));
        copy((void*)&m[0], (const void*)&m_.m[0], 3 * sizeof(float));
        copy((void*)&m[4], (const void*)&m_.m[3], 3 * sizeof(float));
        copy((void*)&m[8], (const void*)&m_.m[6], 3 * sizeof(float));
        m[15] = 1.0f;
      }

      FOUNDATION_INLINE Mat4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33 )
      {
        m[0]  = m00;  m[1] = m01; m[2]  = m02; m[3]  = m03;
        m[4]  = m10;  m[5] = m11; m[6]  = m12; m[7]  = m13;
        m[8]  = m20;  m[9] = m21; m[10] = m22; m[11] = m23;
        m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
      }

      FOUNDATION_INLINE Mat4(
        const float m_[] )
      { copy((void*)&m[0], (const void*)&m_[0], sizeof(m)); }

      FOUNDATION_INLINE Mat4(
        const Mat4& m_ )
      { copy((void*)&m[0], (const void*)&m_.m[0], sizeof(m)); }

      FOUNDATION_INLINE Mat4& operator= (
        const Mat4& m_ )
      { copy((void*)&m[0], (const void*)&m_.m[0], sizeof(m)); return *this; }

    public:
      FOUNDATION_INLINE static Mat4 identity()
      { return Mat4(); }

      static Mat4 translate(
        const Vec3f& translation )
      {
        return Mat4(
          1.0f, 0.0f, 0.0f, translation.x,
          0.0f, 1.0f, 0.0f, translation.y,
          0.0f, 0.0f, 1.0f, translation.z,
          0.0f, 0.0f, 0.0f, 1.0f);
      }

      static Mat4 rotate(
        const Quat& q )
      {
        return Mat4(
          1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z,
          2.0f * q.x * q.y - 2.0f * q.z * q.w,
          2.0f * q.x * q.z + 2.0f * q.y * q.w, 0.0f,
          2.0f * q.x * q.y + 2.0f * q.z * q.w,
          1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z,
          2.0f * q.y * q.z - 2.0f * q.x * q.w, 0.0f,
          2.0f * q.x * q.z - 2.0f * q.y * q.w,
          2.0f * q.y * q.z + 2.0f * q.x * q.w,
          1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y, 0.0f,
          0.0f, 0.0f, 0.0f, 1.0f);
      }

      static Mat4 scale( const Vec3f& s ) {
        return Mat4(
          s.x,  0.0f, 0.0f, 0.0f,
          0.0f, s.y,  0.0f, 0.0f,
          0.0f, 0.0f, s.z,  0.0f,
          0.0f, 0.0f, 0.0f, 1.0f
        );
      }

      FOUNDATION_INLINE static Mat4 translate_rotate_scale(
        const Vec3f& translation,
        const Quat& rotation,
        const Vec3f& scale )
      { return Mat4::scale(scale) * Mat4::rotate(rotation) * Mat4::translate(translation); }

      static Mat4 orthographic(
        float min_x, float max_x,
        float min_y, float max_y,
        float near_plane, float far_plane )
      {
        const float dx = max_x - min_x;
        const float dy = max_y - min_y;
        const float dz = near_plane - far_plane;

        return Mat4(
          2.0f / dx, 0.0f, 0.0f, 0.0f,
          0.0f, 2.0f / dy, 0.0f, 0.0f,
          0.0f, 0.0f, 1.0f / dz, 0.0f,
          0.0f, 0.0f, near_plane / dz, 1.0f );
      }

      static Mat4 perspective(
        float field_of_view,
        float aspect_ratio,
        float near_plane,
        float far_plane )
      {
        const float ys = tanf(PI - (field_of_view / 2.0f));
        const float xs = ys / aspect_ratio;
        const float dz = near_plane - far_plane;

        return Mat4(
          xs, 0.0f, 0.0f, 0.0f,
          0.0f, ys, 0.0f, 0.0f,
          0.0f, 0.0f, (far_plane - near_plane) / dz, (2 * far_plane * near_plane) / dz,
          0.0f, 0.0f, -1.0f, 0.0f);
      }

      static Mat4 look_at(
        const Vec3f& eye,
        const Vec3f& target,
        const Vec3f& up )
      {
        const Vec3f z_axis = (eye - target).normalize();
        const Vec3f x_axis = up.cross(z_axis).normalize();
        const Vec3f y_axis = z_axis.cross(x_axis);

        return Mat4(
          x_axis.x, y_axis.x, z_axis.x, 0.0f,
          x_axis.y, y_axis.y, z_axis.y, 0.0f,
          x_axis.z, y_axis.z, z_axis.z, 0.0f,
          -x_axis.dot(eye), -y_axis.dot(eye), -z_axis.dot(eye), 1.0f);
      }

    public:
      FOUNDATION_INLINE Mat4 operator+ (
        const Mat4& m_ ) const
      {
        return Mat4(
          m[0]  + m_.m[0],  m[1]  + m_.m[1],  m[2]  + m_.m[2],  m[3]  + m_.m[3],
          m[4]  + m_.m[4],  m[5]  + m_.m[5],  m[6]  + m_.m[6],  m[7]  + m_.m[7],
          m[8]  + m_.m[8],  m[9]  + m_.m[9],  m[10] + m_.m[10], m[11] + m_.m[11],
          m[12] + m_.m[12], m[13] + m_.m[13], m[14] + m_.m[14], m[15] + m_.m[15]);
      }

      FOUNDATION_INLINE Mat4 operator- (
        const Mat4& m_ ) const
      {
        return Mat4(
          m[0]  - m_.m[0],  m[1]  - m_.m[1],  m[2]  - m_.m[2],  m[3]  - m_.m[3],
          m[4]  - m_.m[4],  m[5]  - m_.m[5],  m[6]  - m_.m[6],  m[7]  - m_.m[7],
          m[8]  - m_.m[8],  m[9]  - m_.m[9],  m[10] - m_.m[10], m[11] - m_.m[11],
          m[12] - m_.m[12], m[13] - m_.m[13], m[14] - m_.m[14], m[15] - m_.m[15]);
      }

      FOUNDATION_INLINE Mat4 operator* (
        const Mat4& m_ ) const
      {
        return Mat4(
          m[0]  * m_.m[0] + m[1]  * m_.m[4] + m[2]  * m_.m[8]  + m[3]  * m_.m[12],
          m[0]  * m_.m[1] + m[1]  * m_.m[5] + m[2]  * m_.m[9]  + m[3]  * m_.m[13],
          m[0]  * m_.m[2] + m[1]  * m_.m[6] + m[2]  * m_.m[10] + m[3]  * m_.m[14],
          m[0]  * m_.m[3] + m[1]  * m_.m[7] + m[2]  * m_.m[11] + m[3]  * m_.m[15],
          m[4]  * m_.m[0] + m[5]  * m_.m[4] + m[6]  * m_.m[8]  + m[7]  * m_.m[12],
          m[4]  * m_.m[1] + m[5]  * m_.m[5] + m[6]  * m_.m[9]  + m[7]  * m_.m[13],
          m[4]  * m_.m[2] + m[5]  * m_.m[6] + m[6]  * m_.m[10] + m[7]  * m_.m[14],
          m[4]  * m_.m[3] + m[5]  * m_.m[7] + m[6]  * m_.m[11] + m[7]  * m_.m[15],
          m[8]  * m_.m[0] + m[9]  * m_.m[4] + m[10] * m_.m[8]  + m[11] * m_.m[12],
          m[8]  * m_.m[1] + m[9]  * m_.m[5] + m[10] * m_.m[9]  + m[11] * m_.m[13],
          m[8]  * m_.m[2] + m[9]  * m_.m[6] + m[10] * m_.m[10] + m[11] * m_.m[14],
          m[8]  * m_.m[3] + m[9]  * m_.m[7] + m[10] * m_.m[11] + m[11] * m_.m[15],
          m[12] * m_.m[0] + m[13] * m_.m[4] + m[14] * m_.m[8]  + m[15] * m_.m[12],
          m[12] * m_.m[1] + m[13] * m_.m[5] + m[14] * m_.m[9]  + m[15] * m_.m[13],
          m[12] * m_.m[2] + m[13] * m_.m[6] + m[14] * m_.m[10] + m[15] * m_.m[14],
          m[12] * m_.m[3] + m[13] * m_.m[7] + m[14] * m_.m[11] + m[15] * m_.m[15]);
      }

      FOUNDATION_INLINE Mat4 operator/ (
        const Mat4& m_ ) const
      { return (*this) * m_.inverse(); }

    public:
      FOUNDATION_INLINE Vec3f operator* (
        const Vec3f& v ) const
      {
        const float w = (m[12] * v.x + m[13] * v.y * v.z + m[15]);
        return Vec3f(
          (m[0] * v.x + m[1] * v.y + m[2]  * v.z + m[3])  / w,
          (m[4] * v.x + m[5] * v.y + m[6]  * v.z + m[7])  / w,
          (m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11]) / w);
      }

    public:
      FOUNDATION_INLINE Mat4 transpose() const
      {
        return Mat4(
          m[0], m[4], m[8],  m[12],
          m[1], m[5], m[9],  m[13],
          m[2], m[6], m[10], m[14],
          m[3], m[7], m[11], m[15]);
      }

      FOUNDATION_INLINE float determinant() const
      {
        return (
          m[0]  * m[5]  * m[10] * m[15] - m[0]  * m[5]  * m[14] * m[11] + m[0]  * m[9] * m[14] * m[7]  - m[0]  * m[9] * m[6]  * m[15] +
          m[0]  * m[13] * m[6]  * m[11] - m[0]  * m[13] * m[10] * m[7]  - m[4]  * m[9] * m[14] * m[3]  + m[4]  * m[9] * m[2]  * m[15] -
          m[4]  * m[13] * m[2]  * m[11] + m[4]  * m[13] * m[10] * m[3]  - m[4]  * m[1] * m[10] * m[15] + m[4]  * m[1] * m[14] * m[11] +
          m[8]  * m[13] * m[2]  * m[7]  - m[8]  * m[13] * m[6]  * m[3]  + m[8]  * m[1] * m[6]  * m[15] - m[8]  * m[1] * m[14] * m[7] +
          m[8]  * m[5]  * m[14] * m[3]  - m[8]  * m[5]  * m[2]  * m[15] - m[12] * m[1] * m[6]  * m[11] + m[12] * m[1] * m[10] * m[7] -
          m[12] * m[5]  * m[10] * m[3]  + m[12] * m[5]  * m[2]  * m[11] - m[12] * m[9] * m[2]  * m[7]  + m[12] * m[9] * m[6]  * m[3]);
      }

      FOUNDATION_INLINE Mat4 inverse() const
      {
        const float d = determinant();
        return Mat4(
          (m[5] * (m[10] * m[15] - m[14] * m[11]) + m[9] * (m[14] * m[7] - m[6] * m[15]) + m[13] * (m[6] * m[11] - m[10] * m[7])) / d,
          (m[4] * (m[10] * m[15] - m[14] * m[11]) + m[8] * (m[14] * m[7] - m[6] * m[15]) + m[12] * (m[6] * m[11] - m[10] * m[7])) / -d,
          (m[4] * (m[9]  * m[15] - m[13] * m[11]) + m[8] * (m[13] * m[7] - m[5] * m[15]) + m[12] * (m[5] * m[11] - m[9]  * m[7])) / d,
          (m[4] * (m[9]  * m[14] - m[13] * m[10]) + m[8] * (m[13] * m[6] - m[5] * m[14]) + m[12] * (m[5] * m[10] - m[9]  * m[6])) / -d,
          (m[1] * (m[10] * m[15] - m[14] * m[11]) + m[9] * (m[14] * m[3] - m[2] * m[15]) + m[13] * (m[2] * m[11] - m[10] * m[3])) / -d,
          (m[0] * (m[10] * m[15] - m[14] * m[11]) + m[8] * (m[14] * m[3] - m[2] * m[15]) + m[12] * (m[2] * m[11] - m[10] * m[3])) / d,
          (m[0] * (m[9]  * m[15] - m[13] * m[11]) + m[8] * (m[13] * m[3] - m[1] * m[15]) + m[12] * (m[1] * m[11] - m[9]  * m[3])) / -d,
          (m[0] * (m[9]  * m[14] - m[13] * m[10]) + m[8] * (m[13] * m[2] - m[1] * m[14]) + m[12] * (m[1] * m[10] - m[9]  * m[2])) / d,
          (m[1] * (m[6]  * m[15] - m[14] * m[7])  + m[5] * (m[14] * m[3] - m[2] * m[15]) + m[13] * (m[2] * m[7]  - m[6]  * m[3])) / d,
          (m[0] * (m[6]  * m[15] - m[14] * m[7])  + m[4] * (m[14] * m[3] - m[2] * m[15]) + m[12] * (m[2] * m[7]  - m[6]  * m[3])) / -d,
          (m[0] * (m[5]  * m[15] - m[13] * m[7])  + m[4] * (m[13] * m[3] - m[1] * m[15]) + m[12] * (m[1] * m[7]  - m[5]  * m[3])) / d,
          (m[0] * (m[5]  * m[14] - m[13] * m[6])  + m[4] * (m[13] * m[2] - m[1] * m[14]) + m[12] * (m[1] * m[6]  - m[5]  * m[2])) / -d,
          (m[1] * (m[6]  * m[11] - m[10] * m[7])  + m[5] * (m[10] * m[3] - m[2] * m[11]) + m[9]  * (m[2] * m[7]  - m[6]  * m[3])) / -d,
          (m[0] * (m[6]  * m[11] - m[10] * m[7])  + m[4] * (m[10] * m[3] - m[2] * m[11]) + m[8]  * (m[2] * m[7]  - m[6]  * m[3])) / d,
          (m[0] * (m[5]  * m[11] - m[9]  * m[7])  + m[4] * (m[9]  * m[3] - m[1] * m[11]) + m[8]  * (m[1] * m[7]  - m[5]  * m[3])) / -d,
          (m[0] * (m[5]  * m[10] - m[9]  * m[6])  + m[4] * (m[9]  * m[2] - m[1] * m[10]) + m[8]  * (m[1] * m[6]  - m[5]  * m[2])) / d);
      }

    public:
      void decompose(
        Vec3f& translation,
        Quat& rotation,
        Vec3f& scale ) const
      {
        translation.x = m[3];
        translation.y = m[7];
        translation.z = m[11];

        scale.x = Vec3f(m[0], m[1], m[2]).magnitude();
        scale.y = Vec3f(m[4], m[5], m[6]).magnitude();
        scale.z = Vec3f(m[8], m[9], m[10]).magnitude();

        const float det =
          m[0] * (m[5] * m[10] - m[6] * m[9]) +
          m[1] * (m[6] * m[8] - m[4] * m[10]) +
          m[2] * (m[4] * m[9] - m[5] * m[8]);

        const float aq2 = pow(det, (1.0f / 3.0f)) /* cube root */;

        rotation.w = sqrtf(max(0.0f, aq2 + m[0] + m[5] + m[10])) / 2.0f;
        rotation.x = sqrtf(max(0.0f, aq2 + m[0] - m[5] - m[10])) / 2.0f;
        rotation.y = sqrtf(max(0.0f, aq2 - m[0] + m[5] - m[10])) / 2.0f;
        rotation.z = sqrtf(max(0.0f, aq2 - m[0] - m[5] + m[10])) / 2.0f;
      }

    public:
      float m[16];
  };
} // butane

#endif // _BUTANE_MATH_MAT4_H_
