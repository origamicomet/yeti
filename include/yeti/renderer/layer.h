//===-- yeti/renderer/layer.h ---------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_RENDERER_LAYER_H_
#define _YETI_RENDERER_LAYER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {
namespace renderer {

///
struct Layer {
  typedef u32 Id;

  enum Order {
    UNKNOWN = 0,

    /// Draw in whatever order is deemed fastest.
    DO_NOT_CARE = 1,

    /// Draw in order of submission.
    SUBMISSION = 2,

    /// Draw in an order that mimimizes state changes.
    STATE = 3,

    /// Draw closer things first.
    FRONT_TO_BACK = 4,

    /// Draw further things first.
    BACK_TO_FRONT = 5,

    /// \internal Force `u32` storage and alignment.
    YETI_RENDERER_LAYER_ORDER_FORCE_U32 = 0x7fffffff
  };

  Id id;

  const char *name;

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  const char *scope;
#endif

  Order order;

  u32 render_target_ids[8];
  u32 depth_stencil_target_id;
};

namespace layer {
  extern YETI_PUBLIC Layer::Id id_from_name(const char *name);
  extern YETI_PUBLIC Layer::Order order_from_string(const char *order);
  extern YETI_PUBLIC const char *order_to_string(const Layer::Order order);
}

} // renderer
} // yeti

#endif // _YETI_RENDERER_LAYER_H_
