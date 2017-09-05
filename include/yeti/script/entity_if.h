//===-- yeti/script/entity_if.h -------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SCRIPT_ENTITY_IF_H_
#define _YETI_SCRIPT_ENTITY_IF_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/script_if.h"

#include "yeti/entity.h"

namespace yeti {

namespace entity_if {

/// ...
extern YETI_PUBLIC void expose(Script *script);

} // entity_if

} // yeti

#endif // _YETI_SCRIPT_ENTITY_IF_H_
