//===-- yeti/script/bindings/transform_if.cc ------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/transform_if.h"

#include "yeti/script.h"

namespace yeti {

namespace transform_if {
  namespace {
  }
} // transform_if

void transform_if::expose(Script *script) {
  script->add_module("Transform");
}

} // yeti
