//===-- yeti/script/enviornment.h -----------------------*- mode: C++11 -*-===//
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

#include "yeti/core.h"

// A little backwards, because
#include "yeti/script.h"

#include "yeti/math.h"

namespace yeti {

/// ...
class YETI_PUBLIC ScriptEnvironment {
 YETI_DISALLOW_COPYING(ScriptEnvironment)

 public:
  typedef u32 Type;

 private:
  friend Script;

 private:
  static const unsigned NUM_OF_TEMPORARIES = 8192;

 private:
  ScriptEnvironment();
  ~ScriptEnvironment();

 public:
  /// Allocates storage for a temporary.
  template <typename T>
  T *allocate();

  /// Invalidates all temporaries, reclaiming all storage.
  void reset();

  /// Determines if @temporary is of type @T and not been invalidated.
  template <typename T>
  bool valid(const T *temporary);

  /// Guesses type pointed to by @ptr.
  Script::Type type(void *ptr) const;

 private:
  // Pointers below are not a temporary.
  void *lower_bounds_of_pointers_;

  // Pointers above are not a temporary.
  void *upper_bounds_of_pointers_;

  // Descended to determine type of a temporary.
  void *lower_bounds_of_pointers_by_type_[4];

  struct {
    unsigned vec2;
    unsigned vec3;
    unsigned vec4;
    unsigned quaternion;
  } counts;

  struct {
    Vec2 vec2[NUM_OF_TEMPORARIES];
    Vec3 vec3[NUM_OF_TEMPORARIES];
    Vec4 vec4[NUM_OF_TEMPORARIES];
    Quaternion quaternion[NUM_OF_TEMPORARIES];
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

template <> YETI_INLINE Quaternion *ScriptEnvironment::allocate() {
  yeti_assert_development(this->counts.quaternion < NUM_OF_TEMPORARIES);
  return &this->storage.quaternion[this->counts.quaternion++];
}

template <> YETI_INLINE bool ScriptEnvironment::valid(const Vec2 *temporary) {
  return (temporary >= &this->storage.vec2[0])
      && (temporary <= &this->storage.vec2[NUM_OF_TEMPORARIES]);
}

template <> YETI_INLINE bool ScriptEnvironment::valid(const Vec3 *temporary) {
  return (temporary >= &this->storage.vec3[0])
      && (temporary <= &this->storage.vec3[NUM_OF_TEMPORARIES]);
}

template <> YETI_INLINE bool ScriptEnvironment::valid(const Vec4 *temporary) {
  return (temporary >= &this->storage.vec4[0])
      && (temporary <= &this->storage.vec4[NUM_OF_TEMPORARIES]);
}

template <> YETI_INLINE bool ScriptEnvironment::valid(const Quaternion *temporary) {
  return (temporary >= &this->storage.quaternion[0])
      && (temporary <= &this->storage.quaternion[NUM_OF_TEMPORARIES]);
}

// PERF(mtwilliams): Overlap storage for all temporaries, padding to largest
// type to force alignment when necessary. Then, maintain a look-aside array
// to recover type. Should compile down to a subtraction, shift, and load.

YETI_INLINE Script::Type ScriptEnvironment::type(void *ptr) const {
  if ((ptr > upper_bounds_of_pointers_) || (ptr < lower_bounds_of_pointers_))
    // Not a temporary.
    return Script::T_UNKNOWN;

  if (ptr >= lower_bounds_of_pointers_by_type_[0])
    return Script::T_QUATERNION;
  if (ptr >= lower_bounds_of_pointers_by_type_[1])
    return Script::T_VEC4;
  if (ptr >= lower_bounds_of_pointers_by_type_[2])
    return Script::T_VEC3;
  if (ptr >= lower_bounds_of_pointers_by_type_[3])
    return Script::T_VEC2;

  YETI_UNREACHABLE();

  return Script::T_UNKNOWN;
}

} // yeti

#endif // _YETI_SCRIPT_ENVIRONMENT_H_
