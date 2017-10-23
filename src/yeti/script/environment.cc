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
  // Pointers below are not temporary.
  lower_bounds_of_pointers_ = (void *)&this->storage;

  // Pointers above are not temporary.
  upper_bounds_of_pointers_ = (void *)(uintptr_t(&this->storage) + sizeof(this->storage));

  // Descended to determine type of a temporary.
  lower_bounds_of_pointers_by_type_[0] = (void *)&this->storage.mat4[0];
  lower_bounds_of_pointers_by_type_[1] = (void *)&this->storage.mat3[0];
  lower_bounds_of_pointers_by_type_[2] = (void *)&this->storage.quaternion[0];
  lower_bounds_of_pointers_by_type_[3] = (void *)&this->storage.vec4[0];
  lower_bounds_of_pointers_by_type_[4] = (void *)&this->storage.vec3[0];
  lower_bounds_of_pointers_by_type_[5] = (void *)&this->storage.vec2[0];

  this->reset();
}

ScriptEnvironment::~ScriptEnvironment() {
}

void ScriptEnvironment::reset() {
  this->counts.vec2 = 0;
  this->counts.vec3 = 0;
  this->counts.vec4 = 0;
  this->counts.quaternion = 0;
  this->counts.mat3 = 0;
  this->counts.mat4 = 0;
}

} // yeti
