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

namespace yeti {

struct Transform {
#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEUBG
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  Vec3 position;
  Quat rotation;
  Vec3 scale;
#else
  Mat4 matrix;
#endif
};

} // yeti

#endif // _YETI_COMPONENTS_TRANSFORM_H_
