//===-- yeti/resources/entity_compiler.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "entity_compiler.h"

namespace yeti {

// TODO(mtwilliams): Inheritance.

EntityCompiler::EntityCompiler(const resource_compiler::Environment *env,
                               const resource_compiler::Input *input,
                               const resource_compiler::Output *output)
  : env_(env)
  , input_(input)
  , output_(output)
  , document_(core::global_heap_allocator())
  , heap_(core::global_heap_allocator(), SIZE_OF_HEAP)
  , root_(NULL)
{
  yeti_assert_debug(env_ != NULL);
  yeti_assert_debug(input_ != NULL);
  yeti_assert_debug(output_ != NULL);
}

EntityCompiler::~EntityCompiler() {
}

bool EntityCompiler::run() {
  // Load description into memory then parse into a tree.
  if (!load())
    return false;

  // Walk freshly minted tree to validate.
  if (!validate())
    return false;

  // Walk again to compile into an intermediate representation.
  if (!compile())
    return false;

  // Convert intermediate representation to optimized runtime format.
  if (!bake())
    return false;

  return true;
}

bool EntityCompiler::load() {
  // Load markup into memory.
  const u64 size = core::fs::size(input_->source);
  document_.resize(size+1);
  core::fs::read(input_->source, (void *)document_.raw(), size);

  // Null-terminate for parser.
  document_[size] = '\0';

  // Use stack as scratch buffer is we don't need a lot of memory.
  u8 scratch[SIZE_OF_SCRATCH];

  // Parse into tree.
  if (xml_parse_into_memory((const char *)document_.raw(),
                            (void *)scratch,
                            SIZE_OF_SCRATCH,
                            (void *)heap_.raw(),
                            SIZE_OF_HEAP,
                            &root_) != XML_OK)
    return false;

  return true;
}

bool EntityCompiler::validate() {
  // TODO(mtwilliams): Walk tree and make sure everything is in order.
  return true;
}

bool EntityCompiler::compile() {
  return false;
}

bool EntityCompiler::bake() {
  return false;
}

} // yeti
