//===-- yeti/script/bindings/component_if.cc ------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/component_if.h"

#include "yeti/script.h"

namespace yeti {

namespace component_if {
  namespace {
  }
} // component_if

void component_if::expose(Script *script) {
  script->add_module("Component");
}

} // yeti
