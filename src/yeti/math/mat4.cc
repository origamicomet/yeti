//===-- yeti/math/mat4.cc -------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/math/mat4.h"

#include "yeti/math/constants.h"
#include "yeti/math/conversions.h"

namespace yeti {

const Mat4 Mat4::IDENTITY = Mat4(1.f, 0.f, 0.f, 0.f,
                                 0.f, 1.f, 0.f, 0.f,
                                 0.f, 0.f, 1.f, 0.f,
                                 0.f, 0.f, 0.f, 1.f);

const Mat4 Mat4::ZERO = Mat4(0.f, 0.f, 0.f, 0.f,
                             0.f, 0.f, 0.f, 0.f,
                             0.f, 0.f, 0.f, 0.f,
                             0.f, 0.f, 0.f, 0.f);

Mat4 Mat4::perspective(const f32 field_of_view,
                       const f32 aspect,
                       const f32 near,
                       const f32 far)
{
  yeti_assert_with_reason_debug(field_of_view > 0.f, "Horizontal field-of-view must be greater than 0°!");
  yeti_assert_with_reason_debug(field_of_view <= 180.f, "Horizontal field-of-view must be no greater than 180°!");

  // https://www.gamedev.net/articles/programming/graphics/perspective-projections-in-lh-and-rh-systems-r3598/
  // https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/building-basic-perspective-projection-matrix
  // http://vec3.ca/code/math/projection-direct3d/

  const f32 A = 1.f / tanf(degrees_to_radians(field_of_view) * 0.5f);
  const f32 B = A * aspect;
  const f32 C = far / (far - near);
  const f32 D = -1.f;
  const f32 E = near * (far / (far - near));

  return Mat4( A , 0.f, 0.f, 0.f,
              0.f,  B , 0.f, 0.f,
              0.f, 0.f,  C ,  D ,
              0.f, 0.f,  E , 0.f);
}

Mat4 Mat4::orthographic(const f32 top,
                        const f32 left,
                        const f32 bottom,
                        const f32 right,
                        const f32 near,
                        const f32 far)
{
  const f32 dx = right - left;
  const f32 dy = bottom - top;
  const f32 dz = near - far;

  const f32 A = 2.f / dx;
  const f32 B = 2.f / dy;
  const f32 C = 1.f / dz;
  const f32 D = near / dz;

  return Mat4( A , 0.f, 0.f, 0.f,
              0.f,  B , 0.f, 0.f,
              0.f, 0.f,  C , 0.f,
              0.f, 0.f,  D , 1.f);
}

Mat4 Mat4::translate(const Vec3 &t) {
  return Mat4(1.f, 0.f, 0.f, t.x,
              0.f, 1.f, 0.f, t.y,
              0.f, 0.f, 1.f, t.z,
              0.f, 0.f, 0.f, 1.f);
}

Mat4 Mat4::rotate(const Quaternion &r) {
  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/

  Mat4 matrix;

  matrix.M[0][0] = 1.f - 2.f * (r.y * r.y) - 2.f * (r.z * r.z);
  matrix.M[1][0] = 2.f * (r.x * r.y) - 2.f * (r.z * r.w);
  matrix.M[2][0] = 2.f * (r.x * r.z) + 2.f * (r.y * r.w);
  matrix.M[3][0] = 0.f;

  matrix.M[0][1] = 2.f * (r.x * r.y) + 2.f * (r.z * r.w);
  matrix.M[1][1] = 1.f - 2.f * (r.x * r.x) - 2.f * (r.z * r.z);
  matrix.M[2][1] = 2.f * (r.y * r.z) - 2.f * (r.x * r.w);
  matrix.M[3][1] = 0.f;

  matrix.M[0][2] = 2.f * (r.x * r.z) - 2.f * (r.y * r.w);
  matrix.M[1][2] = 2.f * (r.y * r.z) + 2.f * (r.x * r.w);
  matrix.M[2][2] = 1.f - 2.f * (r.x * r.x) - 2.f * (r.y * r.y);
  matrix.M[3][2] = 0.f;

  matrix.M[0][3] = 0.f;
  matrix.M[1][3] = 0.f;
  matrix.M[2][3] = 0.f;
  matrix.M[3][3] = 1.f;

  return matrix;
}

Mat4 Mat4::scale(const Vec3 &s) {
  return Mat4(s.x, 0.f, 0.f, 0.f,
              0.f, s.y, 0.f, 0.f,
              0.f, 0.f, s.z, 0.f,
              0.f, 0.f, 0.f, 1.f);
}

Mat4 Mat4::compose(const Vec3 &translation,
                   const Quaternion &rotation,
                   const Vec3 &scale) {
  Mat4 matrix;

  matrix.M[0][0] = scale.x * (1.f - 2.f * (rotation.y * rotation.y) - 2.f * (rotation.z * rotation.z));
  matrix.M[1][0] = scale.y * (2.f * (rotation.x * rotation.y) - 2.f * (rotation.z * rotation.w));
  matrix.M[2][0] = scale.z * (2.f * (rotation.x * rotation.z) + 2.f * (rotation.y * rotation.w));
  matrix.M[3][0] = translation.x;

  matrix.M[0][1] = scale.x * (2.f * (rotation.x * rotation.y) + 2.f * (rotation.z * rotation.w));
  matrix.M[1][1] = scale.y * (1.f - 2.f * (rotation.x * rotation.x) - 2.f * (rotation.z * rotation.z));
  matrix.M[2][1] = scale.z * (2.f * (rotation.y * rotation.z) - 2.f * (rotation.x * rotation.w));
  matrix.M[3][1] = translation.y;

  matrix.M[0][2] = scale.x * (2.f * (rotation.x * rotation.z) - 2.f * (rotation.y * rotation.w));
  matrix.M[1][2] = scale.y * (2.f * (rotation.y * rotation.z) + 2.f * (rotation.x * rotation.w));
  matrix.M[2][2] = scale.z * (1.f - 2.f * (rotation.x * rotation.x) - 2.f * (rotation.y * rotation.y));
  matrix.M[3][2] = translation.z;

  matrix.M[3][0] = 0.f;
  matrix.M[3][1] = 0.f;
  matrix.M[3][2] = 0.f;
  matrix.M[3][3] = 1.f;

  return matrix;
}

void Mat4::decompose(const Mat4 &matrix,
                     Vec3 *translation,
                     Quaternion *rotation,
                     Vec3 *scale) {
  yeti_assert_debug(translation != NULL);
  yeti_assert_debug(rotation != NULL);
  yeti_assert_debug(scale != NULL);

  *translation = translation_from_matrix(matrix);
  *scale = scale_from_matrix(matrix);
  *rotation = rotation_from_matrix(matrix);
}

#if 0
f32 Mat4::determinant() const {
}

Mat4 Mat4::inverse() const {
}
#endif

Mat4 Mat4::transpose() const {
  return Mat4(M[0][0], M[0][1], M[0][2], M[0][3],
              M[1][0], M[1][1], M[1][2], M[1][3],
              M[2][0], M[2][1], M[2][2], M[2][3],
              M[3][0], M[3][1], M[3][2], M[3][3]);
}

Vec3 translation_from_matrix(const Mat4 &m) {
  return Vec3(m(0,3), m(1,3), m(2,3));
}

Vec3 scale_from_matrix(const Mat4 &m) {
  Vec3 scale;

  scale.x = Vec3(m(0,0), m(0,1), m(0,2)).magnitude();
  scale.y = Vec3(m(1,0), m(1,1), m(1,2)).magnitude();
  scale.z = Vec3(m(2,0), m(2,1), m(2,2)).magnitude();

  return scale;
}

Quaternion rotation_from_matrix(const Mat4 &m) {
  Quaternion rotation;

  const f32 determinant = m(0,0) * (m(1,1) * m(2,2) - m(1,2) * m(2,1))
                        + m(0,1) * (m(1,2) * m(2,0) - m(1,0) * m(2,2))
                        + m(0,2) * (m(1,0) * m(2,1) - m(1,1) * m(2,0));

  const f32 cube_root_of_determinant = powf(determinant, 1.f / 3.f);

  rotation.x = sqrtf(YETI_MAX(0.0f, cube_root_of_determinant + m(0,0) - m(1,1) - m(2,2))) * 0.5f;
  rotation.y = sqrtf(YETI_MAX(0.0f, cube_root_of_determinant - m(0,0) + m(1,1) - m(2,2))) * 0.5f;
  rotation.z = sqrtf(YETI_MAX(0.0f, cube_root_of_determinant - m(0,0) - m(1,1) + m(2,2))) * 0.5f;
  rotation.w = sqrtf(YETI_MAX(0.0f, cube_root_of_determinant + m(0,0) + m(1,1) + m(2,2))) * 0.5f;

  return rotation;
}

} // yeti
