//===-- yeti/script.h -------------------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_SCRIPT_H_
#define _YETI_SCRIPT_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

extern "C" {
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
  #include <luajit.h>
}

namespace yeti {

class ScriptResource;

/// ...
class YETI_PUBLIC Script {
 YETI_DISALLOW_COPYING(Script);

 public:
  enum Type {
    T_UNKNOWN  = 0,
    T_NIL      = 1,
    T_BOOLEAN  = 2,
    T_INTEGER  = 3,
    T_FLOAT    = 4,
    T_STRING   = 5,
    T_ARRAY    = 6,
    T_MAP      = 7,
    T_POINTER  = 8,
    T_FUNCTION = 9
  };

 public:
  Script();
  ~Script();

 private:
  static void *__alloc(Script *script, void *ptr, size_t old_sz, size_t new_sz);
  static int __error_handler(lua_State *L);

 public:
  /// \brief Creates a module called @module.
  void add_module(const char *module);

  /// \brief Adds @fn as @name to the @module.
  void add_module_function(const char *module,
                           const char *name,
                           const lua_CFunction fn);

 public:
  /// \brief Calls the global function @fn with @n number of arguments.
  ///
  /// Each argument is specified by the type preceding the value, except for
  /// T_NIL. For example:
  ///
  ///   call("meaning_of_life", Script::T_INTEGER, 42);
  ///
  bool call(const char *fn, u32 n, ...);

 public:
  /// \brief Injects @script_resource into the global scope.
  void inject(const ScriptResource *script_resource);

 public:
  lua_State *state();

 private:
  lua_State *L;
};

YETI_INLINE lua_State *Script::state() {
  return L;
}

} // yeti

#endif // _YETI_SCRIPT_H_
