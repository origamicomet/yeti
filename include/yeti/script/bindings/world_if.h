//===-- yeti/script/bindings/world_if.h -----------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SCRIPT_BINDINGS_WORLD_IF_H_
#define _YETI_SCRIPT_BINDINGS_WORLD_IF_H_

#include "yeti/script/binding.h"

namespace yeti {

// See `yeti/world.h`.
class World;

namespace world_if {

/// \brief Checks if the value on @L's stack at @idx is world.
extern YETI_PUBLIC bool check(lua_State *L, int idx);

/// \brief Checks if the value on @L's stack at @idx is world then returns it.
extern YETI_PUBLIC World *cast(lua_State *L, int idx);

/// \brief Pushes @world onto @L's stack.
extern YETI_PUBLIC void push(lua_State *L, World *world);

/// ...
extern YETI_PUBLIC void expose(Script *script);

} // world_if
} // yeti

#endif // _YETI_SCRIPT_BINDINGS_WORLD_IF_H_
