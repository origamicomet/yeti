//===-- yeti/light.h ------------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_LIGHT_H_
#define _YETI_LIGHT_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/ecs.h"

#include "yeti/math.h"
#include "yeti/color.h"

namespace yeti {

// TODO(mtwilliams): Temperature based parameterization.
// TODO(mtwilliams): Wave length based parameterization.
// TODO(mtwilliams): Area lights.
// TODO(mtwilliams): IES.

struct DirectionalLight {
  /// Direction of rays.
  Vec3 direction;

  /// Color of light emitted.
  Color color;

  /// Incident energy in lux.
  f32 intensity;
};

struct PointLight {
  /// Emanating point.
  Vec3 position;

  /// Distance up to which light affects.
  f32 radius;

  /// Color of light emitted.
  Color color;

  /// Energy emitted in lumens.
  f32 intensity;
};

struct SpotLight {
  /// Emanating point.
  Vec3 position;

  /// Distance up to which light affects.
  f32 radius;

  /// Yaw, pitch, and roll.
  Vec3 direction;

  /// Inner angle expressed in radians.
  f32 angle;

  /// Color of light emitted.
  Color color;

  /// Energy emitted in lumens.
  f32 intensity;
};

struct Light {
  typedef u32 Id;

  enum Kind {
    DIRECTIONAL = 1,
    POINT       = 2,
    SPOT        = 3
  };

  enum Flags {
    /// Contributes light.
    ENABLED = 1 << 0,

    /// Does not contribute to indirect lighting.
    VIRTUAL = 1 << 1,

    /// Shadow casting.
    SHADOWS = 1 << 2,

    /// Volumetric.
    VOLUMETRIC = 1 << 3
  };

  Light::Kind kind;

  union {
    DirectionalLight directional;
    PointLight point;
    SpotLight spot;
  };

  u32 flags;
};

} // yeti

#endif // _YETI_LIGHT_H_
