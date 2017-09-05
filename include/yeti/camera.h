//===-- yeti/camera.h -----------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_CAMERA_H_
#define _YETI_CAMERA_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/ecs.h"

namespace yeti {

struct Camera {
  enum Type {
    UNKNOWN      = 0,
    PERSPECTIVE  = 1,
    ORTHOGRAPHIC = 2
  };

  struct Handle {
    u32 instance;
  };

  Type type;

  union {
    struct {
      f32 fov;
    } perspective;

    struct {
      f32 top;
      f32 left;
      f32 bottom;
      f32 right;
    } orthographic;
  };

  f32 near;
  f32 far;
};

namespace camera {
  extern YETI_PUBLIC Camera::Type type_to_string(const char *type);
  extern YETI_PUBLIC const char *type_to_string(const Camera::Type type);
}

} // yeti

#endif // _YETI_CAMERA_H_
