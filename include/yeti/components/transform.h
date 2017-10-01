//===-- yeti/components/transform.h ---------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_COMPONENTS_TRANSFORM_H_
#define _YETI_COMPONENTS_TRANSFORM_H_

#include "yeti/core.h"

#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

#include "yeti/math.h"

namespace yeti {

struct Pose {
#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  Vec3 position;
  Quaternion rotation;
  Vec3 scale;
#else
  Mat4 matrix;
#endif
};

} // yeti

#endif // _YETI_COMPONENTS_TRANSFORM_H_
