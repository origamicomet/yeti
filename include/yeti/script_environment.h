//===-- yeti/script_enviornment.h -----------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SCRIPT_ENVIRONMENT_H_
#define _YETI_SCRIPT_ENVIRONMENT_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

extern "C" {
  struct lua_State;
}

#include "yeti/math/vec2.h"
#include "yeti/math/vec3.h"
#include "yeti/math/vec4.h"

namespace yeti {

class Script;

/// ...
class YETI_PUBLIC ScriptEnvironment {
 YETI_DISALLOW_COPYING(ScriptEnvironment)

 private:
  friend Script;

 private:
  static const u32 NUM_OF_TEMPORARIES = 4096ul;

 private:
  ScriptEnvironment();
  ~ScriptEnvironment();

 public:
  /// Allocates storage for a temporary.
  template <typename T>
  T *allocate();

  /// Invalidates all temporaries, reclaiming all storage.
  void reset();

 private:
  struct {
    u32 vec2;
    u32 vec3;
    u32 vec4;
  } counts;

  struct {
    Vec2 vec2[NUM_OF_TEMPORARIES];
    Vec3 vec3[NUM_OF_TEMPORARIES];
    Vec4 vec4[NUM_OF_TEMPORARIES];
  } storage;
};

template <> YETI_INLINE Vec2 *ScriptEnvironment::allocate() {
  yeti_assert_development(this->counts.vec2 < NUM_OF_TEMPORARIES);
  return &this->storage.vec2[this->counts.vec2++];
}

template <> YETI_INLINE Vec3 *ScriptEnvironment::allocate() {
  yeti_assert_development(this->counts.vec3 < NUM_OF_TEMPORARIES);
  return &this->storage.vec3[this->counts.vec3++];
}

template <> YETI_INLINE Vec4 *ScriptEnvironment::allocate() {
  yeti_assert_development(this->counts.vec4 < NUM_OF_TEMPORARIES);
  return &this->storage.vec4[this->counts.vec4++];
}

} // yeti

#endif // _YETI_SCRIPT_ENVIRONMENT_H_
