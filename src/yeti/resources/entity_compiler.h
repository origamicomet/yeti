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

 private:
  static const size_t SIZE_OF_SCRATCH = 64 * 1024 - 1;
  static const size_t SIZE_OF_HEAP = 1 * 1024 * 1024;

 public:
  EntityCompiler(const resource_compiler::Environment *env,
                 const resource_compiler::Input *input,
                 const resource_compiler::Output *output);

  ~EntityCompiler();

 public:
  bool run();

 private:
  bool load();
  bool validate();
  bool compile();
  bool bake();

 private:
  const resource_compiler::Environment *env_;
  const resource_compiler::Input *input_;
  const resource_compiler::Output *output_;

  // Buffer storing entire document.
  core::Array<u8> document_;

  // Heap passed to parser for tree.
  core::Array<u8> heap_;

  xml_element_t *root_;

  bool document_has_declaration_;
};

} // yeti

#endif // _YETI_ENTITY_COMPILER_H_
