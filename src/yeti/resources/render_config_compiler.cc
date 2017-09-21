//===-- yeti/resources/render_config_compiler.cc --------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "render_config_compiler.h"

namespace yeti {

RenderConfigCompiler::RenderConfigCompiler(const resource_compiler::Environment *env,
                                           const resource_compiler::Input *input,
                                           const resource_compiler::Output *output)
  : env_(env)
  , input_(input)
  , output_(output)
  , variables_(core::global_heap_allocator())
  , global_resources_(core::global_heap_allocator())
  , local_resources_(core::global_heap_allocator())
  , generators_(core::global_heap_allocator())
  , modifiers_(core::global_heap_allocator())
  , layers_(core::global_heap_allocator())
{
  yeti_assert_debug(input_ != NULL);
  yeti_assert_debug(output_ != NULL);
}

RenderConfigCompiler::~RenderConfigCompiler() {
}

bool RenderConfigCompiler::run() {
  return this->parse()
      && this->compile();
}

// Plan
 // (1) Manually build the typed, in-memory representation of `render_config`
 // (2) Implement `RenderConfigCompiler::compile`
 // (3) Build runtime components and get things choochin'
 // (4) Write (and rewrite until nice) `origamicomet/jolly`
 // (5) Implement `RenderConfigCompiler::parse`

bool RenderConfigCompiler::parse() {
  // variables_.push(...);

  return false;
}

bool RenderConfigCompiler::compile() {
  // Header
  // Variables
  // Global Resources
  // Local Resources
  // Generators
  // Modifiers
  // Layers

  return false;
}

} // yeti
