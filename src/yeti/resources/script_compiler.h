//===-- yeti/resources/script_compiler.h ----------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SCRIPT_COMPILER_H_
#define _YETI_SCRIPT_COMPILER_H_

#include "yeti/resource.h"
#include "yeti/resource_compiler.h"

extern "C" {
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
}

namespace yeti {

class YETI_PRIVATE ScriptCompiler {
 YETI_DISALLOW_COPYING(ScriptCompiler);

 public:
  ScriptCompiler(const resource_compiler::Environment *env,
                 const resource_compiler::Input *input,
                 const resource_compiler::Output *output);

  ~ScriptCompiler();

 public:
  bool run();

 private:
  bool compile();
  bool dump();

 private:
  bool empty() const;

 private:
  const resource_compiler::Environment *env_;
  const resource_compiler::Input *input_;
  const resource_compiler::Output *output_;

  lua_State *lua_;

  core::Array<u8> source_;
  core::Array<u8> bytecode_;
};

} // yeti

#endif // _YETI_SCRIPT_COMPILER_H_
