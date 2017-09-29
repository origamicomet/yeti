//===-- yeti/resources/render_config_resource.h ---------*- mode: C++11 -*-===//
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

#ifndef _YETI_RENDER_CONFIG_RESOURCE_H_
#define _YETI_RENDER_CONFIG_RESOURCE_H_

#include "yeti/core.h"

#include "yeti/resource.h"

namespace yeti {

class YETI_PUBLIC RenderConfigResource : public Resource {
 YETI_DISALLOW_COPYING(RenderConfigResource)

 private:
  RenderConfigResource(Resource::Id id);
  ~RenderConfigResource();

 public:
  /// \internal
  static const Resource::Type *type();

 private:
  static Resource *prepare(Resource::Id id);

  static void load(Resource *resource, const Resource::Data &data);
  static void unload(Resource *resource);

  static bool compile(const resource_compiler::Environment *env,
                      const resource_compiler::Input *input,
                      const resource_compiler::Output *output);

  static bool compatible(u32 version);

 private:
  void *memory_resident_data_;
};

} // yeti

#endif // _YETI_RENDER_CONFIG_RESOURCE_H_
