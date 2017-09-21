//===-- yeti/world.h ------------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_WORLD_H_
#define _YETI_WORLD_H_

#include "yeti/core.h"

namespace yeti {

class YETI_PUBLIC World {
 YETI_DISALLOW_COPYING(World)

 private:
  World();
  ~World();

 public:
  static World *create();

  void update(const f32 delta_time);

  void destroy();
};

} // yeti

#endif // _YETI_WORLD_H_
