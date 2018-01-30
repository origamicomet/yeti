//===-- yeti/script/bindings/component_if.h -------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SCRIPT_BINDINGS_COMPONENT_IF_H_
#define _YETI_SCRIPT_BINDINGS_COMPONENT_IF_H_

#include "yeti/script/binding.h"

#include "yeti/component.h"

namespace yeti {

namespace component_if {

// TODO(mtwilliams): Expose handle and instances dichotomy to Lua.

struct Handle {
  Component::Id type;
  Component::Handle handle;
};

/// \brief Checks if the value on @L's stack at @index is a handle to a
/// component of @type.
extern YETI_PUBLIC bool check(lua_State *L, int index, Component::Id type);

/// \brief Checks if the value on @L's stack at @index is a handle to the right
/// @type of component then returns it.
extern YETI_PUBLIC const Handle &cast(lua_State *L, int index, Component::Id type);

/// \brief Pushes @handle onto @L's stack.
extern YETI_PUBLIC void push(lua_State *L, Component::Id type, Component::Handle handle);

/// ...
extern YETI_PUBLIC void expose(Script *script);

} // component_if

} // yeti

#endif // _YETI_SCRIPT_BINDINGS_COMPONENT_IF_H_
