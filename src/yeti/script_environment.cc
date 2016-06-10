//===-- yeti/script_environment.cc ------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script_environment.h"

#include "yeti/script.h"

namespace yeti {

ScriptEnvironment::ScriptEnvironment() {
  this->reset();
}

ScriptEnvironment::~ScriptEnvironment() {
}

template <>
Vec2 *ScriptEnvironment::allocate() {
  yeti_assert_development(this->counts.vec2 < NUM_OF_TEMPORARIES);
  return &this->storage.vec2[this->counts.vec2++];
}

template <>
Vec3 *ScriptEnvironment::allocate() {
  yeti_assert_development(this->counts.vec3 < NUM_OF_TEMPORARIES);
  return &this->storage.vec3[this->counts.vec3++];
}

template <>
Vec4 *ScriptEnvironment::allocate() {
  yeti_assert_development(this->counts.vec4 < NUM_OF_TEMPORARIES);
  return &this->storage.vec4[this->counts.vec4++];
}

void ScriptEnvironment::reset() {
  this->counts.vec2 = 0;
  this->counts.vec3 = 0;
  this->counts.vec4 = 0;
}

} // yeti
