// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_PLANE_H_
#define _BUTANE_MATH_PLANE_H_

#include <butane/butane.h>
#include <butane/math/vec3.h>
#include <butane/math/constants.h>

#include <math.h>

namespace butane {
  class BUTANE_EXPORT Plane final {
    public:
      Plane(
        const Vec3f& normal,
        const float distance
      ) : normal(normal)
        , d(distance)
      {}

      Plane(
        const Plane& p
      ) : normal(p.normal)
        , d(p.d)
      {}

      Plane& operator= (
        const Plane& p )
      {
        normal = p.normal;
        d = p.d;
        return *this;
      }

    public:
      FOUNDATION_INLINE const float distance(
        const Vec3f& p ) const
      {
        return normal.dot(p) - d;
      }

    public:
      Vec3f normal;
      float d;
  };
} // butane

#endif // _BUTANE_MATH_PLANE_H_
