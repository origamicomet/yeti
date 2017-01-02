//===-- yeti/resources/script_resource.h ------------------*- mode: C++ -*-===//
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

#ifndef _YETI_SCRIPT_RESOURCE_H_
#define _YETI_SCRIPT_RESOURCE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/resource.h"

namespace yeti {

class Script;

class YETI_PUBLIC ScriptResource : public Resource {
 YETI_DISALLOW_COPYING(ScriptResource);

 private:
  friend Script;

 public:
  ScriptResource(Resource::Id id);
  ~ScriptResource();

 public:
  /// \internal
  static const Resource::Type *type();

 private:
  static Resource *prepare(Resource::Id id);

  static void load(Resource *resource, const Resource::Data &data);
  static void unload(Resource *resource);

  static bool compile(const resource_compiler::Input *input,
                      const resource_compiler::Output *output);

 private:
  char path_[256];
  const char *bytecode_;
  size_t bytecode_len_;
};

} // yeti

#endif // _YETI_SCRIPT_RESOURCE_H_
