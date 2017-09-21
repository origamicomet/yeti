//===-- yeti/script.h -----------------------------------*- mode: C++11 -*-===//
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

#include "yeti/core.h"

extern "C" {
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
  #include <luajit.h>
}

#include "yeti/script/environment.h"

namespace yeti {

// See `yeti/resources/script_resource.h`.
class ScriptResource;

// A virtual machine and environment for executing Lua.
class YETI_PUBLIC Script {
 YETI_DISALLOW_COPYING(Script)

 public:
  enum Type {
    T_UNKNOWN   = 0,
    T_NIL       = 1,
    T_BOOLEAN   = 2,
    T_INTEGER   = 3,
    T_FLOAT     = 4,
    T_STRING    = 5,
    T_ARRAY     = 6,
    T_MAP       = 7,
    T_POINTER   = 8,
    T_FUNCTION  = 9,
    T_TEMPORARY = 10,
    T_REF       = 11
  };

 public:
  Script();
  ~Script();

 private:
  static void *__alloc(Script *script, void *ptr, size_t, size_t size);
  static int __error_handler(lua_State *L);

 public:
  /// \internal Recovers encompassing `Script` from @L.
  static Script *recover(lua_State *L);

 public:
  /// \brief Injects @script_resource into the global scope.
  void inject(const ScriptResource *script_resource);

 public:
  /// \brief Binds @fn to @name.
  void add_function(const char *name,
                    const lua_CFunction fn);

  /// \brief Creates a module called @module.
  void add_module(const char *module);

  /// \brief Adds @fn so that is called whenever @module is called.
  void add_module_constructor(const char *module,
                              const lua_CFunction fn);

  /// \brief Binds @fn to @name in @module.
  void add_module_function(const char *module,
                           const char *name,
                           const lua_CFunction fn);

 public:
  /// \brief Determines if value at @index is convertible to @T.
  template <typename T> bool is_a(int index);

  /// \brief Returns value at @index as type @T.
  template <typename T> T to_a(int index);

 public:
  /// \brief Calls the global function @fn with @n number of arguments.
  ///
  /// Each argument is specified by the type preceding the value, except for
  /// `T_NIL`. For example:
  ///
  ///   call("meaning_of_life", Script::T_INTEGER, 42);
  ///
  bool call(const char *fn, u32 n, ...);

 public:
  lua_State *state();
  ScriptEnvironment *environment();

 private:
  lua_State *L;
  ScriptEnvironment E;
};

YETI_INLINE lua_State *Script::state() {
  return L;
}

YETI_INLINE ScriptEnvironment *Script::environment() {
  return &E;
}

} // yeti

#endif // _YETI_SCRIPT_H_
