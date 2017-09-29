//===-- yeti/script/environment.cc ----------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script.h"
#include "yeti/script/environment.h"

namespace yeti {

ScriptEnvironment::ScriptEnvironment() {
  this->reset();
}

ScriptEnvironment::~ScriptEnvironment() {
}

void ScriptEnvironment::reset() {
  this->counts.vec2 = 0;
  this->counts.vec3 = 0;
  this->counts.vec4 = 0;
  this->counts.quaternion = 0;
}

} // yeti
