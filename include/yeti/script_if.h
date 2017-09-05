//===-- yeti/script_if.h --------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SCRIPT_IF_H_
#define _YETI_SCRIPT_IF_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

extern "C" {
  struct lua_State;
}

namespace yeti {

class Script;

namespace script_if {

/// \internal
template <typename T>
extern bool is_a(lua_State *L, int idx);

/// \internal
template <typename T>
extern T *to_a(lua_State *L, int idx);

} // script_if

} // yeti

#endif // _YETI_SCRIPT_IF_H_
