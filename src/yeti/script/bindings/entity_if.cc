//===-- yeti/script/bindings/entity_if.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/entity_if.h"

#include "yeti/script.h"

namespace yeti {

namespace entity_if {
  namespace {
  }
} // entity_if

void entity_if::expose(Script *script) {
  script->add_module("Entity");
}

} // yeti
