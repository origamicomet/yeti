//===-- yeti/script/light_if.cc -------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/light_if.h"

#include "yeti/script.h"

namespace yeti {

namespace light_if {
  namespace {
} // light_if

void light_if::expose(Script *script) {
  script->add_module("Light");
}

} // yeti
