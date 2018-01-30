//===-- yeti/script/bindingsentity_if.h -------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SCRIPT_BINDINGS_ENTITY_IF_H_
#define _YETI_SCRIPT_BINDINGS_ENTITY_IF_H_

#include "yeti/script/binding.h"

#include "yeti/entity.h"

namespace yeti {

// Forward declared for `entity_if::Handle`.
class World;

namespace entity_if {

struct Handle {
  World *world;
  Entity entity;
};

/// \brief Checks if the value on @L's stack at @idx is a handle to an entity.
extern YETI_PUBLIC bool check(lua_State *L, int idx);

/// \brief Checks if the value on @L's stack at @idx is a handle to an entity
/// then returns it.
extern YETI_PUBLIC const Handle &cast(lua_State *L, int idx);

/// \brief Pushes @handle onto @L's stack.
extern YETI_PUBLIC void push(lua_State *L, const Handle &handle);

/// ...
extern YETI_PUBLIC void expose(Script *script);

} // entity_if

} // yeti

#endif // _YETI_SCRIPT_BINDINGS_ENTITY_IF_H_
