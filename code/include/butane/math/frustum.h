// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_FRUSTUM_H_
#define _BUTANE_MATH_FRUSTUM_H_

#include <butane/butane.h>
#include <butane/math/plane.h>
#include <butane/math/mat4.h>
#include <butane/math/constants.h>

#include <math.h>

namespace butane {
  class BUTANE_EXPORT Frustum final {
    public:
      Frustum(
        const Mat4& m
      ) : near(Vec3f(m.m[3] + m.m[2], m.m[7] + m.m[6], m.m[11] + m.m[10]), m.m[15] + m.m[14])
        , far(Vec3f(m.m[3] - m.m[2], m.m[7] - m.m[6], m.m[11] - m.m[10]), m.m[15] - m.m[14])
        , left(Vec3f(m.m[3] + m.m[0], m.m[7] + m.m[4], m.m[11] + m.m[8]), m.m[15] + m.m[12])
        , right(Vec3f(m.m[3] - m.m[0], m.m[7] - m.m[4], m.m[11] - m.m[8]), m.m[15] - m.m[12])
        , top(Vec3f(m.m[3] - m.m[1], m.m[7] - m.m[5], m.m[11] - m.m[9]), m.m[15] - m.m[13])
        , bottom(Vec3f(m.m[3] + m.m[1], m.m[7] + m.m[5], m.m[11] + m.m[9]), m.m[15] + m.m[13])
      {
      }

    public:
      Plane near, far;
      Plane left, right;
      Plane top, bottom;
  };
} // butane

#endif // _BUTANE_MATH_FRUSTUM_H_
