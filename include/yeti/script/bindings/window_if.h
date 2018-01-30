//===-- yeti/script/bindings/window_if.h ----------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SCRIPT_BINDINGS_WINDOW_IF_H_
#define _YETI_SCRIPT_BINDINGS_WINDOW_IF_H_

#include "yeti/script/binding.h"

namespace yeti {

// See `yeti/world.h`.
class Window;

namespace window_if {

/// \brief Checks if the value on @L's stack at @idx is window.
extern YETI_PUBLIC bool check(lua_State *L, int idx);

/// \brief Checks if the value on @L's stack at @idx is window then returns it.
extern YETI_PUBLIC Window *cast(lua_State *L, int idx);

/// \brief Pushes @window onto @L's stack.
extern YETI_PUBLIC void push(lua_State *L, Window *window);

/// ...
extern YETI_PUBLIC void expose(Script *script);

} // window_if
} // yeti

#endif // _YETI_SCRIPT_BINDINGS_WINDOW_IF_H_
