//===-- yeti/resources/render_config_compiler.h ---------*- mode: C++11 -*-===//
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

#ifndef _YETI_RENDER_CONFIG_COMPILER_H_
#define _YETI_RENDER_CONFIG_COMPILER_H_

#include "yeti/resource.h"
#include "yeti/resource_compiler.h"

#include "render_config_format.h"

namespace yeti {

class YETI_PRIVATE RenderConfigCompiler {
 YETI_DISALLOW_COPYING(RenderConfigCompiler);

 private:
  // Size, in bytes, dedicated to tokenization and parsing.
  static const size_t SIZE_OF_BUFFER = 1048575 /* 1 MiB */;

 private:
  typedef render_config_format::Variable Variable;
  typedef render_config_format::Resource Resource;
  typedef render_config_format::Generator Generator;
  typedef render_config_format::Modifier Modifier;
  typedef render_config_format::Layer Layer;

 public:
  RenderConfigCompiler(const resource_compiler::Environment *env,
                       const resource_compiler::Input *input,
                       const resource_compiler::Output *output);

  ~RenderConfigCompiler();

 public:
  bool run();

 private:
  bool parse();
  bool compile();

 private:
  const resource_compiler::Environment *env_;
  const resource_compiler::Input *input_;
  const resource_compiler::Output *output_;

  core::Array<Variable> variables_;
  core::Array<Resource> global_resources_;
  core::Array<Resource> local_resources_;
  core::Array<Generator> generators_;
  core::Array<Modifier> modifiers_;
  core::Array<Layer> layers_;
};

} // yeti

#endif // _YETI_RENDER_CONFIG_COMPILER_H_
