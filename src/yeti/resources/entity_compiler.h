//===-- yeti/resources/entity_compiler.h ----------------*- mode: C++11 -*-===//
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

#ifndef _YETI_ENTITY_COMPILER_H_
#define _YETI_ENTITY_COMPILER_H_

#include "yeti/resource.h"
#include "yeti/resource_compiler.h"

namespace yeti {

class YETI_PRIVATE EntityCompiler {
 YETI_DISALLOW_COPYING(EntityCompiler);

 public:
  EntityCompiler(const resource_compiler::Environment *env,
                 const resource_compiler::Input *input,
                 const resource_compiler::Output *output);

  ~EntityCompiler();

 public:
  bool run();

 private:
  const resource_compiler::Environment *env_;
  const resource_compiler::Input *input_;
  const resource_compiler::Output *output_;
};

} // yeti

#endif // _YETI_ENTITY_COMPILER_H_
