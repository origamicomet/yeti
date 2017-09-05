//===-- yeti/renderer/resource_set.h --------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_RENDER_RESOURCE_SET_H_
#define _YETI_RENDER_RESOURCE_SET_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/renderer/resource.h"

namespace yeti {
namespace renderer {

/// ...
class ResourceSet {
 YETI_DISALLOW_COPYING(ResourceSet)

 private:
  // TODO(mtwilliams): Overload hash to prevent useless mixing.
  foundation::HashMap<Resource::Id, u32> id_to_index_;
  foundation::Array<Resource> resources_;
};

} // renderer
} // yeti

#endif // _YETI_RENDER_RESOURCE_SET_H_
