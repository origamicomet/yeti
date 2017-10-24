//===-- yeti/script/bindings/math_if.cc -----------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/math_if.h"

#include "yeti/script.h"

#include "yeti/math.h"

namespace yeti {

template <> void Script::push<Vec2>(Vec2 v) {
  // Allocate temporary storage.
  Vec2 *storage = E->allocate<Vec2>();

  // Copy value over.
  *storage = v;

  // Cast to appropriate type.
  lua_getfield(L, LUA_REGISTRYINDEX, "_Vec2.cast");
  lua_pushlightuserdata(L, (void *)storage);
  lua_call(L, 1, 1);
}

template <> bool Script::is_a<Vec2>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    return false;

  const Vec2 *ptr_to_v = *(Vec2 **)lua_topointer(L, index);

  return E->valid<Vec2>(ptr_to_v);
}

template <> Vec2 Script::to_a<Vec2>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    luaL_typerror(L, index, "Vec2");

  const Vec2 *ptr_to_v = *(Vec2 **)lua_topointer(L, index);

  if (!E->valid<Vec2>(ptr_to_v))
    luaL_typerror(L, index, "Vec2");

  return *ptr_to_v;
}

template <> void Script::push<Vec3>(Vec3 v) {
  // Allocate temporary storage.
  Vec3 *storage = E->allocate<Vec3>();

  // Copy value over.
  *storage = v;

  // Cast to appropriate type.
  lua_getfield(L, LUA_REGISTRYINDEX, "_Vec3.cast");
  lua_pushlightuserdata(L, (void *)storage);
  lua_call(L, 1, 1);
}

template <> bool Script::is_a<Vec3>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    return false;

  const Vec3 *ptr_to_v = *(Vec3 **)lua_topointer(L, index);

  return E->valid<Vec3>(ptr_to_v);
}

template <> Vec3 Script::to_a<Vec3>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    luaL_typerror(L, index, "Vec3");

  const Vec3 *ptr_to_v = *(Vec3 **)lua_topointer(L, index);

  if (!E->valid<Vec3>(ptr_to_v))
    luaL_typerror(L, index, "Vec3");

  return *ptr_to_v;
}

template <> void Script::push<Vec4>(Vec4 v) {
  // Allocate temporary storage.
  Vec4 *storage = E->allocate<Vec4>();

  // Copy value over.
  *storage = v;

  // Cast to appropriate type.
  lua_getfield(L, LUA_REGISTRYINDEX, "_Vec4.cast");
  lua_pushlightuserdata(L, (void *)storage);
  lua_call(L, 1, 1);
}

template <> bool Script::is_a<Vec4>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    return false;

  const Vec4 *ptr_to_v = *(Vec4 **)lua_topointer(L, index);

  return E->valid<Vec4>(ptr_to_v);
}

template <> Vec4 Script::to_a<Vec4>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    luaL_typerror(L, index, "Vec4");

  const Vec4 *ptr_to_v = *(Vec4 **)lua_topointer(L, index);

  if (!E->valid<Vec4>(ptr_to_v))
    luaL_typerror(L, index, "Vec4");

  return *ptr_to_v;
}

template <> void Script::push<Quaternion>(Quaternion v) {
  // Allocate temporary storage.
  Quaternion *storage = E->allocate<Quaternion>();

  // Copy value over.
  *storage = v;

  // Cast to appropriate type.
  lua_getfield(L, LUA_REGISTRYINDEX, "_Quaternion.cast");
  lua_pushlightuserdata(L, (void *)storage);
  lua_call(L, 1, 1);
}

template <> bool Script::is_a<Quaternion>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    return false;

  const Quaternion *ptr_to_v = *(Quaternion **)lua_topointer(L, index);

  return E->valid<Quaternion>(ptr_to_v);
}

template <> Quaternion Script::to_a<Quaternion>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    luaL_typerror(L, index, "Quaternion");

  const Quaternion *ptr_to_v = *(Quaternion **)lua_topointer(L, index);

  if (!E->valid<Quaternion>(ptr_to_v))
    luaL_typerror(L, index, "Quaternion");

  return *ptr_to_v;
}

template <> void Script::push<Mat4>(Mat4 v) {
  // Allocate temporary storage.
  Mat4 *storage = E->allocate<Mat4>();

  // Copy value over.
  *storage = v;

  // Cast to appropriate type.
  lua_getfield(L, LUA_REGISTRYINDEX, "_Mat4.cast");
  lua_pushlightuserdata(L, (void *)storage);
  lua_call(L, 1, 1);
}

template <> bool Script::is_a<Mat4>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    return false;

  const Mat4 *ptr_to_v = *(Mat4 **)lua_topointer(L, index);

  return E->valid<Mat4>(ptr_to_v);
}

template <> Mat4 Script::to_a<Mat4>(int index) {
  if (lua_type(L, index) != LUA_TCDATA)
    luaL_typerror(L, index, "Mat4");

  const Mat4 *ptr_to_v = *(Mat4 **)lua_topointer(L, index);

  if (!E->valid<Mat4>(ptr_to_v))
    luaL_typerror(L, index, "Mat4");

  return *ptr_to_v;
}

// TODO(mtwilliams): Use `ffi.C` instead of manually binding helpers.
//
// PERF(mtwilliams): Investigate using `ffi.new` instead of custom pools.
//
// PERF(mtwilliams): Move all boxing/unboxing to `ffi.copy`.
//
// PERF(mtwilliams): LuaJIT performs internal conversion between floats and
// doubles. This may be a bottleneck. It may be faster to forward to C to
// perform math. Also, we may be able to store double width types and convert
// only at boundary (calling into and from C/C++.)
//
// See https://github.com/LuaJIT/LuaJIT/issues/32

namespace math_if {
  namespace {
    // Math is reimplemented using LuaJIT's FFI for a few reasons. First, it
    // allows us to assign metatables to the types, aiding code readability.
    // Second, it increases type safety. Third, it allows LuaJIT to optimize
    // math much further than equivalent calls into C.
    static const char *GLUE =
      "local ffi = require('ffi')                                          \n"
      "                                                                    \n"
      "ffi.cdef([[                                                         \n"
      "  typedef struct Vec2 { float x, y; } Vec2;                         \n"
      "  typedef struct Vec3 { float x, y, z; } Vec3;                      \n"
      "  typedef struct Vec4 { float x, y, z, w; } Vec4;                   \n"
      "  typedef struct Quaternion { float x, y, z, w; } Quaternion;       \n"
      "  typedef struct Mat4 { float m[4][4]; } Mat4;                      \n"
      "]])                                                                 \n"
      "                                                                    \n"
      "ffi.cdef([[                                                         \n"
      "  typedef struct { Vec2 contents; } Vec2Box;                        \n"
      "  typedef struct { Vec3 contents; } Vec3Box;                        \n"
      "  typedef struct { Vec4 contents; } Vec4Box;                        \n"
      "  typedef struct { Quaternion contents; } QuaternionBox;            \n"
      "  typedef struct { Mat4 contents; } Mat4Box;                        \n"
      "]])                                                                 \n"
      "                                                                    \n"
      "local T_Vec2 = ffi.typeof('Vec2 *')                                 \n"
      "local T_Vec3 = ffi.typeof('Vec3 *')                                 \n"
      "local T_Vec4 = ffi.typeof('Vec4 *')                                 \n"
      "                                                                    \n"
      "local T_Quaternion = ffi.typeof('Quaternion *')                     \n"
      "                                                                    \n"
      "local T_Mat4 = ffi.typeof('Mat4 *')                                 \n"
      "                                                                    \n"
      "local T_Vec2Box = ffi.typeof('Vec2Box')                             \n"
      "local T_Vec3Box = ffi.typeof('Vec3Box')                             \n"
      "local T_Vec4Box = ffi.typeof('Vec4Box')                             \n"
      "                                                                    \n"
      "local T_QuaternionBox = ffi.typeof('QuaternionBox')                 \n"
      "                                                                    \n"
      "local T_Mat4Box = ffi.typeof('Mat4Box')                             \n"
      "                                                                    \n"
      "function _Vec2.cast(ptr) return ffi.cast(T_Vec2, ptr) end           \n"
      "function _Vec3.cast(ptr) return ffi.cast(T_Vec3, ptr) end           \n"
      "function _Vec4.cast(ptr) return ffi.cast(T_Vec4, ptr) end           \n"
      "                                                                    \n"
      "function _Quaternion.cast(ptr)                                      \n"
      "  return ffi.cast(T_Quaternion, ptr)                                \n"
      "end                                                                 \n"
      "                                                                    \n"
      "function _Mat4.cast(ptr)                                            \n"
      "  return ffi.cast(T_Mat4, ptr)                                      \n"
      "end                                                                 \n"
      "                                                                    \n"
      "_G.Vec2 = ffi.metatype('Vec2', {                                    \n"
      "  __new = function(ct, x, y)                                        \n"
      "    return ffi.cast(T_Vec2, _Vec2.allocate(x, y))                   \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __index = {                                                       \n"
      "    x = function(v) return v.x end,                                 \n"
      "    y = function(v) return v.y end,                                 \n"
      "                                                                    \n"
      "    normalize = function(v)                                         \n"
      "      local f = 1.0 / math.sqrt(v.x*v.x+v.y*v.y)                    \n"
      "      return ffi.cast(T_Vec2, _Vec2.allocate(v.x*f, v.y*f))         \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    length = function(v)                                            \n"
      "      return math.sqrt(v.x*v.x+v.y*v.y)                             \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    magnitude = function(v)                                         \n"
      "      return math.sqrt(v.x*v.x+v.y*v.y)                             \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    dot = function(v1, v2)                                          \n"
      "      return v1.x*v2.x+v1.y*v2.y                                    \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    cross = function(v1, v2)                                        \n"
      "      -- Just compute the Z-component.                              \n"
      "      return v1.x*v2.y-v1.y*v2.x                                    \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    distance = function(v1, v2)                                     \n"
      "      return (v1 - v2):length()                                     \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    angle = function(v1, v2)                                        \n"
      "      return math.asin(math.abs(v1.x*v2.y-v1.y*v2.x))               \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    min = function(v1, v2)                                          \n"
      "      local x = math.min(v1.x, v2.x)                                \n"
      "      local y = math.min(v1.y, v2.y)                                \n"
      "      return ffi.cast(T_Vec2, _Vec2.allocate(x, y))                 \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    max = function(v1, v2)                                          \n"
      "      local x = math.max(v1.x, v2.x)                                \n"
      "      local y = math.max(v1.y, v2.y)                                \n"
      "      return ffi.cast(T_Vec2, _Vec2.allocate(x, y))                 \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    lerp = function(v1, v2, t)                                      \n"
      "      local x = v1.x + (v2.x - v2.x) * t                            \n"
      "      local y = v1.y + (v2.y - v2.y) * t                            \n"
      "      return ffi.cast(T_Vec2, _Vec2.allocate(x, y))                 \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    box = function(v)                                               \n"
      "      local box = ffi.new(T_Vec2Box)                                \n"
      "      box.contents.x = v.x                                          \n"
      "      box.contents.y = v.y                                          \n"
      "      return box                                                    \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __unm = function(v)                                               \n"
      "    return ffi.cast(T_Vec2, _Vec2.allocate(-v.x, -v.y))             \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __add = function(lhs, rhs)                                        \n"
      "    local x = lhs.x + rhs.x                                         \n"
      "    local y = lhs.y + rhs.y                                         \n"
      "    return ffi.cast(T_Vec2, _Vec2.allocate(x, y))                   \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __sub = function(lhs, rhs)                                        \n"
      "    local x = lhs.x - rhs.x                                         \n"
      "    local y = lhs.y - rhs.y                                         \n"
      "    return ffi.cast(T_Vec2, _Vec2.allocate(x, y))                   \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __mul = function(lhs, rhs)                                        \n"
      "    local x = lhs.x                                                 \n"
      "    local y = lhs.y                                                 \n"
      "                                                                    \n"
      "    if ffi.istype(T_Vec2, rhs) then                                 \n"
      "      x = x * rhs.x                                                 \n"
      "      y = y * rhs.y                                                 \n"
      "    else                                                            \n"
      "      x = x * rhs                                                   \n"
      "      y = y * rhs                                                   \n"
      "    end                                                             \n"
      "                                                                    \n"
      "    return ffi.cast(T_Vec2, _Vec2.allocate(x, y))                   \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __div = function(lhs, rhs)                                        \n"
      "    local x = lhs.x                                                 \n"
      "    local y = lhs.y                                                 \n"
      "                                                                    \n"
      "    if ffi.istype(T_Vec2, rhs) then                                 \n"
      "      x = x / rhs.x                                                 \n"
      "      y = y / rhs.y                                                 \n"
      "    else                                                            \n"
      "      x = x / rhs                                                   \n"
      "      y = y / rhs                                                   \n"
      "    end                                                             \n"
      "                                                                    \n"
      "    return ffi.cast(T_Vec2, _Vec2.allocate(x, y))                   \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __tostring = function(v)                                          \n"
      "    return string.format('Vec2(%g, %g)', v.x, v.y)                  \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "ffi.metatype('Vec2Box', {                                           \n"
      "  __index = {                                                       \n"
      "    unbox = function(box)                                           \n"
      "      local boxed = box.contents                                    \n"
      "      return ffi.cast(T_Vec2, _Vec2.allocate(boxed.x, boxed.y))     \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __tostring = function(box)                                        \n"
      "    local v = box.contents                                          \n"
      "    return string.format('Vec2[%g, %g]', v.x, v.y)                  \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "_G.Vec3 = ffi.metatype('Vec3', {                                    \n"
      "  __new = function(ct, x, y, z)                                     \n"
      "    return ffi.cast(T_Vec3, _Vec3.allocate(x, y, z))                \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __index = {                                                       \n"
      "    x = function(v) return v.x end,                                 \n"
      "    y = function(v) return v.y end,                                 \n"
      "    z = function(v) return v.z end,                                 \n"
      "                                                                    \n"
      "    normalize = function(v)                                         \n"
      "      local f = 1.0 / math.sqrt(v.x*v.x+v.y*v.y+v.z*v.z)            \n"
      "      return ffi.cast(T_Vec3, _Vec3.allocate(v.x*f, v.y*f, v.z*f))  \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    length = function(v)                                            \n"
      "      return math.sqrt(v.x*v.x+v.y*v.y+v.z*v.z)                     \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    magnitude = function(v)                                         \n"
      "      return math.sqrt(v.x*v.x+v.y*v.y+v.z*v.z)                     \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    dot = function(v1, v2)                                          \n"
      "      return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z                          \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    cross = function(v1, v2)                                        \n"
      "      return ffi.cast(T_Vec3, _Vec3.allocate(v1.y*v2.z-v1.z*v2.y,   \n"
      "                                             v1.z*v2.x-v1.x*v2.z,   \n"
      "                                             v1.x*v2.y-v1.y*v2.x))  \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    distance = function(v1, v2)                                     \n"
      "      return (v1 - v2):length()                                     \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    angle = function(v1, v2)                                        \n"
      "      local length = math.sqrt(v1:dot(v1) * v2:dot(v2))             \n"
      "      return math.acos(v1:dot(v2) / length)                         \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    min = function(v1, v2)                                          \n"
      "      local x = math.min(v1.x, v2.x)                                \n"
      "      local y = math.min(v1.y, v2.y)                                \n"
      "      local z = math.min(v1.z, v2.z)                                \n"
      "      return ffi.cast(T_Vec3, _Vec3.allocate(x, y, z))              \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    max = function(v1, v2)                                          \n"
      "      local x = math.max(v1.x, v2.x)                                \n"
      "      local y = math.max(v1.y, v2.y)                                \n"
      "      local z = math.max(v1.z, v2.z)                                \n"
      "      return ffi.cast(T_Vec3, _Vec3.allocate(x, y, z))              \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    lerp = function(v1, v2, t)                                      \n"
      "      local x = v1.x + (v2.x - v2.x) * t                            \n"
      "      local y = v1.y + (v2.y - v2.y) * t                            \n"
      "      local z = v1.z + (v2.z - v2.z) * t                            \n"
      "      return ffi.cast(T_Vec3, _Vec3.allocate(x, y, z))              \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    box = function(v)                                               \n"
      "      local box = ffi.new(T_Vec3Box)                                \n"
      "      box.contents.x = v.x                                          \n"
      "      box.contents.y = v.y                                          \n"
      "      box.contents.z = v.z                                          \n"
      "      return box                                                    \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __unm = function(v)                                               \n"
      "    return ffi.cast(T_Vec3, _Vec3.allocate(-v.x, -v.y, -v.z))       \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __add = function(lhs, rhs)                                        \n"
      "    local x = lhs.x + rhs.x                                         \n"
      "    local y = lhs.y + rhs.y                                         \n"
      "    local z = lhs.z + rhs.z                                         \n"
      "    return ffi.cast(T_Vec3, _Vec3.allocate(x, y, z))                \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __sub = function(lhs, rhs)                                        \n"
      "    local x = lhs.x - rhs.x                                         \n"
      "    local y = lhs.y - rhs.y                                         \n"
      "    local z = lhs.z - rhs.z                                         \n"
      "    return ffi.cast(T_Vec3, _Vec3.allocate(x, y, z))                \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __mul = function(lhs, rhs)                                        \n"
      "    local x = lhs.x                                                 \n"
      "    local y = lhs.y                                                 \n"
      "    local z = lhs.z                                                 \n"
      "                                                                    \n"
      "    if ffi.istype(T_Vec3, rhs) then                                 \n"
      "      x = x * rhs.x                                                 \n"
      "      y = y * rhs.y                                                 \n"
      "      z = z * rhs.z                                                 \n"
      "    else                                                            \n"
      "      x = x * rhs                                                   \n"
      "      y = y * rhs                                                   \n"
      "      z = z * rhs                                                   \n"
      "    end                                                             \n"
      "                                                                    \n"
      "    return ffi.cast(T_Vec3, _Vec3.allocate(x, y, z))                \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __div = function(lhs, rhs)                                        \n"
      "    local x = lhs.x                                                 \n"
      "    local y = lhs.y                                                 \n"
      "    local z = lhs.z                                                 \n"
      "                                                                    \n"
      "    if ffi.istype(T_Vec3, rhs) then                                 \n"
      "      x = x / rhs.x                                                 \n"
      "      y = y / rhs.y                                                 \n"
      "      z = z / rhs.z                                                 \n"
      "    else                                                            \n"
      "      x = x / rhs                                                   \n"
      "      y = y / rhs                                                   \n"
      "      z = z / rhs                                                   \n"
      "    end                                                             \n"
      "                                                                    \n"
      "    return ffi.cast(T_Vec3, _Vec3.allocate(x, y, z))                \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __tostring = function(v)                                          \n"
      "    return string.format('Vec3(%g, %g, %g)', v.x, v.y, v.z)         \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "ffi.metatype('Vec3Box', {                                           \n"
      "  __index = {                                                       \n"
      "    unbox = function(box)                                           \n"
      "      local boxed = box.contents                                    \n"
      "      return ffi.cast(T_Vec3, _Vec3.allocate(boxed.x,               \n"
      "                                             boxed.y,               \n"
      "                                             boxed.z))              \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __tostring = function(box)                                        \n"
      "    local v = box.contents                                          \n"
      "    return string.format('Vec3[%g, %g, %g]', v.x, v.y, v.z)         \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "_G.Vec4 = ffi.metatype('Vec4', {                                    \n"
      "  __new = function(ct, x, y, z, w)                                  \n"
      "    return ffi.cast(T_Vec4, _Vec4.allocate(x, y, z, w))             \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __index = {                                                       \n"
      "    x = function(v) return v.x end,                                 \n"
      "    y = function(v) return v.y end,                                 \n"
      "    z = function(v) return v.z end,                                 \n"
      "    w = function(v) return v.w end,                                 \n"
      "                                                                    \n"
      "    normalize = function(v)                                         \n"
      "      local f = 1.0 / math.sqrt(v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w)    \n"
      "                                                                    \n"
      "      return ffi.cast(T_Vec4, _Vec4.allocate(v.x*f,                 \n"
      "                                             v.y*f,                 \n"
      "                                             v.z*f,                 \n"
      "                                             v.w*f))                \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    length = function(v)                                            \n"
      "      return math.sqrt(v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w)             \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    magnitude = function(v)                                         \n"
      "      return math.sqrt(v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w)             \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    dot = function(v1, v2)                                          \n"
      "      return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z+v1.w*v2.w                \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    distance = function(v1, v2)                                     \n"
      "      return (v1 - v2):length()                                     \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    min = function(v1, v2)                                          \n"
      "      local x = math.min(v1.x, v2.x)                                \n"
      "      local y = math.min(v1.y, v2.y)                                \n"
      "      local z = math.min(v1.z, v2.z)                                \n"
      "      local w = math.min(v1.w, v2.w)                                \n"
      "      return ffi.cast(T_Vec4, _Vec4.allocate(x, y, z, w))           \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    max = function(v1, v2)                                          \n"
      "      local x = math.max(v1.x, v2.x)                                \n"
      "      local y = math.max(v1.y, v2.y)                                \n"
      "      local z = math.max(v1.z, v2.z)                                \n"
      "      local w = math.max(v1.w, v2.w)                                \n"
      "      return ffi.cast(T_Vec4, _Vec4.allocate(x, y, z, w))           \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    lerp = function(v1, v2, t)                                      \n"
      "      local x = v1.x + (v2.x - v2.x) * t                            \n"
      "      local y = v1.y + (v2.y - v2.y) * t                            \n"
      "      local z = v1.z + (v2.z - v2.z) * t                            \n"
      "      local w = v1.w + (v2.w - v2.w) * t                            \n"
      "      return ffi.cast(T_Vec4, _Vec4.allocate(x, y, z, w))           \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    box = function(v)                                               \n"
      "      local box = ffi.new(T_Vec4Box)                                \n"
      "      box.contents.x = v.x                                          \n"
      "      box.contents.y = v.y                                          \n"
      "      box.contents.z = v.z                                          \n"
      "      box.contents.w = v.w                                          \n"
      "      return box                                                    \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __unm = function(v)                                               \n"
      "    return ffi.cast(T_Vec4, _Vec4.allocate(-v.x, -v.y, -v.z, -v.w)) \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __add = function(lhs, rhs)                                        \n"
      "    local x = lhs.x + rhs.x                                         \n"
      "    local y = lhs.y + rhs.y                                         \n"
      "    local z = lhs.z + rhs.z                                         \n"
      "    local w = lhs.w + rhs.w                                         \n"
      "    return ffi.cast(T_Vec4, _Vec4.allocate(x, y, z, w))             \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __sub = function(lhs, rhs)                                        \n"
      "    local x = lhs.x - rhs.x                                         \n"
      "    local y = lhs.y - rhs.y                                         \n"
      "    local z = lhs.z - rhs.z                                         \n"
      "    local w = lhs.w - rhs.w                                         \n"
      "    return ffi.cast(T_Vec4, _Vec4.allocate(x, y, z, w))             \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __mul = function(lhs, rhs)                                        \n"
      "    local x = lhs.x                                                 \n"
      "    local y = lhs.y                                                 \n"
      "    local z = lhs.z                                                 \n"
      "    local w = lhs.w                                                 \n"
      "                                                                    \n"
      "    if ffi.istype(T_Vec4, rhs) then                                 \n"
      "      x = x * rhs.x                                                 \n"
      "      y = y * rhs.y                                                 \n"
      "      z = z * rhs.z                                                 \n"
      "      w = w * rhs.w                                                 \n"
      "    else                                                            \n"
      "      x = x * rhs                                                   \n"
      "      y = y * rhs                                                   \n"
      "      z = z * rhs                                                   \n"
      "      w = w * rhs                                                   \n"
      "    end                                                             \n"
      "                                                                    \n"
      "    return ffi.cast(T_Vec4, _Vec4.allocate(x, y, z, w))             \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __div = function(lhs, rhs)                                        \n"
      "    local x = lhs.x                                                 \n"
      "    local y = lhs.y                                                 \n"
      "    local z = lhs.z                                                 \n"
      "    local w = lhs.w                                                 \n"
      "                                                                    \n"
      "    if ffi.istype(T_Vec4, rhs) then                                 \n"
      "      x = x / rhs.x                                                 \n"
      "      y = y / rhs.y                                                 \n"
      "      z = z / rhs.z                                                 \n"
      "      w = w / rhs.w                                                 \n"
      "    else                                                            \n"
      "      x = x / rhs                                                   \n"
      "      y = y / rhs                                                   \n"
      "      z = z / rhs                                                   \n"
      "      w = w / rhs                                                   \n"
      "    end                                                             \n"
      "                                                                    \n"
      "    return ffi.cast(T_Vec4, _Vec4.allocate(x, y, z, w))             \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __tostring = function(v)                                          \n"
      "    return string.format('Vec4(%g, %g, %g, %g)',                    \n"
      "                         v.x, v.y, v.z, v.w)                        \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "ffi.metatype('Vec4Box', {                                           \n"
      "  __index = {                                                       \n"
      "    unbox = function(box)                                           \n"
      "      local boxed = box.contents                                    \n"
      "      return ffi.cast(T_Vec4, _Vec4.allocate(boxed.x,               \n"
      "                                             boxed.y,               \n"
      "                                             boxed.z,               \n"
      "                                             boxed.w))              \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __tostring = function(box)                                        \n"
      "    local v = box.contents                                          \n"
      "    return string.format('Vec4[%g, %g, %g, %g]',                    \n"
      "                         v.x, v.y, v.z, v.w)                        \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "_G.Quaternion = ffi.metatype('Quaternion', {                        \n"
      "  __new = function(ct, x, y, z, w)                                  \n"
      "    return ffi.cast(T_Quaternion, _Quaternion.allocate(x, y, z, w)) \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __index = {                                                       \n"
      "    -- Forward to C.                                                \n"
      "    from_axis_angle = _Quaternion.from_axis_angle,                  \n"
      "    from_euler_angles = _Quaternion.from_euler_angles,              \n"
      "                                                                    \n"
      "    x = function(q) return q.x end,                                 \n"
      "    y = function(q) return q.y end,                                 \n"
      "    z = function(q) return q.z end,                                 \n"
      "    w = function(q) return q.w end,                                 \n"
      "                                                                    \n"
      "    normalize = function(q)                                         \n"
      "      local f = 1.0 / math.sqrt(q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w)    \n"
      "                                                                    \n"
      "      return ffi.cast(T_Quaternion, _Quaternion.allocate(q.x*f,     \n"
      "                                                         q.y*f,     \n"
      "                                                         q.z*f,     \n"
      "                                                         q.w*f))    \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    length = function(q)                                            \n"
      "      return math.sqrt(q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w)             \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    magnitude = function(q)                                         \n"
      "      return math.sqrt(q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w)             \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    conjugate = function(q)                                         \n"
      "      return ffi.cast(T_Quaternion, _Quaternion.allocate(-q.x,      \n"
      "                                                         -q.y,      \n"
      "                                                         -q.z,      \n"
      "                                                          q.w))     \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    inverse = function(q)                                           \n"
      "      local f = 1.0 / math.sqrt(q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w)    \n"
      "                                                                    \n"
      "      return ffi.cast(T_Quaternion, _Quaternion.allocate(-q.x*f,    \n"
      "                                                         -q.y*f,    \n"
      "                                                         -q.z*f,    \n"
      "                                                          q.w*f))   \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    dot = function(q1, q2)                                          \n"
      "      return q1.x*q2.x+q1.y*q2.y+q1.z*q2.z+q1.w*q2.w                \n"
      "    end,                                                            \n"
      "                                                                    \n"
      "    -- Forward to C.                                                \n"
      "    nlerp = _Quaternion.nlerp,                                      \n"
      "    slerp = _Quaternion.slerp,                                      \n"
      "                                                                    \n"
      "    box = function(q)                                               \n"
      "      local box = ffi.new(T_QuaternionBox)                          \n"
      "      box.contents.x = q.x                                          \n"
      "      box.contents.y = q.y                                          \n"
      "      box.contents.z = q.z                                          \n"
      "      box.contents.w = q.w                                          \n"
      "      return box                                                    \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __add = function(lhs, rhs)                                        \n"
      "    local x = lhs.x + rhs.x                                         \n"
      "    local y = lhs.y + rhs.y                                         \n"
      "    local z = lhs.z + rhs.z                                         \n"
      "    local w = lhs.w + rhs.w                                         \n"
      "    return ffi.cast(T_Quaternion, _Quaternion.allocate(x, y, z, w)) \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __sub = function(lhs, rhs)                                        \n"
      "    local x = lhs.x - rhs.x                                         \n"
      "    local y = lhs.y - rhs.y                                         \n"
      "    local z = lhs.z - rhs.z                                         \n"
      "    local w = lhs.w - rhs.w                                         \n"
      "    return ffi.cast(T_Quaternion, _Quaternion.allocate(x, y, z, w)) \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __mul = function(lhs, rhs)                                        \n"
      "    if ffi.istype(T_Quaternion, rhs) then                           \n"
      "      return _Quaternion.multiply(lhs, rhs)                         \n"
      "    elseif ffi.istype(T_Vec3, rhs) then                             \n"
      "      return _Quaternion.rotate(lhs, rhs)                           \n"
      "    end                                                             \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __tostring = function(v)                                          \n"
      "    return string.format('Quaternion(%g, %g, %g, %g)',              \n"
      "                         v.x, v.y, v.z, v.w)                        \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "ffi.metatype('QuaternionBox', {                                     \n"
      "  __index = {                                                       \n"
      "    unbox = function(box)                                           \n"
      "      local boxed = box.contents                                    \n"
      "      return ffi.cast(T_Quaternion, _Quaternion.allocate(boxed.x,   \n"
      "                                                         boxed.y,   \n"
      "                                                         boxed.z,   \n"
      "                                                         boxed.w))  \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __tostring = function(box)                                        \n"
      "    local v = box.contents                                          \n"
      "    return string.format('Quaternion[%g, %g, %g, %g]',              \n"
      "                         v.x, v.y, v.z, v.w)                        \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "_G.Mat4 = ffi.metatype('Mat4', {                                    \n"
      "  __new = function(ct)                                              \n"
      "    return ffi.cast(T_Mat4, _Mat4.allocate())                       \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __index = {                                                       \n"
      "    -- Forward almost everything to C.                              \n"
      "                                                                    \n"
      "    identity = _Mat4.identity,                                      \n"
      "                                                                    \n"
      "    perspective = _Mat4.perspective,                                \n"
      "    orthographic = _Mat4.orthographic,                              \n"
      "                                                                    \n"
      "    translation = _Mat4.translation,                                \n"
      "    rotation = _Mat4.rotation,                                      \n"
      "    scale = _Mat4.scale,                                            \n"
      "                                                                    \n"
      "    compose = _Mat4.compose,                                        \n"
      "    decompose = _Mat4.decompose,                                    \n"
      "                                                                    \n"
      "    determinant = _Mat4.determinant,                                \n"
      "    inverse = _Mat4.inverse,                                        \n"
      "    transpose = _Mat4.transpose,                                    \n"
      "                                                                    \n"
      "    box = function(matrix)                                          \n"
      "      local box = ffi.new(T_Mat4Box)                                \n"
      "      ffi.copy(box.contents, matrix, ffi.sizeof('Mat4'))            \n"
      "      return box                                                    \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __mul = function(lhs, rhs)                                        \n"
      "    if ffi.istype(T_Mat4, rhs) then                                 \n"
      "      return _Mat4.multiply(lhs, rhs)                               \n"
      "    elseif ffi.istype(T_Vec3, rhs) then                             \n"
      "      return _Mat4.rotate(lhs, rhs)                                 \n"
      "    end                                                             \n"
      "  end,                                                              \n"
      "                                                                    \n"
      "  __tostring = function(matrix)                                     \n"
      "    local m = matrix.m                                              \n"
      "                                                                    \n"
      "    return string.format('Mat4(%g, %g, %g, %g, ' ..                 \n"
      "                              '%g, %g, %g, %g, ' ..                 \n"
      "                              '%g, %g, %g, %g, ' ..                 \n"
      "                              '%g, %g, %g, %g)',                    \n"
      "                                                                    \n"
      "                         m[0][0], m[0][1], m[0][2], m[0][3],        \n"
      "                         m[1][0], m[1][1], m[1][2], m[1][3],        \n"
      "                         m[2][0], m[2][1], m[2][2], m[2][3],        \n"
      "                         m[3][0], m[3][1], m[3][2], m[3][3])        \n"
      "  end                                                               \n"
      "})                                                                  \n"
      "                                                                    \n"
      "ffi.metatype('Mat4Box', {                                           \n"
      "  __index = {                                                       \n"
      "    unbox = function(box)                                           \n"
      "      local matrix = _Mat4.allocate()                               \n"
      "      ffi.copy(matrix, box.contents, ffi.sizeof('Mat4'))            \n"
      "      return ffi.cast(T_Mat4, matrix)                               \n"
      "    end                                                             \n"
      "  },                                                                \n"
      "                                                                    \n"
      "  __tostring = function(box)                                        \n"
      "    local m = box.contents.m                                        \n"
      "                                                                    \n"
      "    return string.format('Mat4[%g, %g, %g, %g, ' ..                 \n"
      "                              '%g, %g, %g, %g, ' ..                 \n"
      "                              '%g, %g, %g, %g, ' ..                 \n"
      "                              '%g, %g, %g, %g]',                    \n"
      "                                                                    \n"
      "                         m[0][0], m[0][1], m[0][2], m[0][3],        \n"
      "                         m[1][0], m[1][1], m[1][2], m[1][3],        \n"
      "                         m[2][0], m[2][1], m[2][2], m[2][3],        \n"
      "                         m[3][0], m[3][1], m[3][2], m[3][3])        \n"
      "  end                                                               \n"
      "})                                                                  \n";

    static int allocate_a_vec2(lua_State *L) {
      ScriptEnvironment *env = (ScriptEnvironment *)lua_touserdata(L, lua_upvalueindex(1));
      Vec2 *v = env->allocate<Vec2>();
      v->x = lua_tonumber(L, 1);
      v->y = lua_tonumber(L, 2);
      lua_pushlightuserdata(L, (void *)v);
      return 1;
    }

    static int allocate_a_vec3(lua_State *L) {
      ScriptEnvironment *env = (ScriptEnvironment *)lua_touserdata(L, lua_upvalueindex(1));
      Vec3 *v = env->allocate<Vec3>();
      v->x = lua_tonumber(L, 1);
      v->y = lua_tonumber(L, 2);
      v->z = lua_tonumber(L, 3);
      lua_pushlightuserdata(L, (void *)v);
      return 1;
    }

    static int allocate_a_vec4(lua_State *L) {
      ScriptEnvironment *env = (ScriptEnvironment *)lua_touserdata(L, lua_upvalueindex(1));
      Vec4 *v = env->allocate<Vec4>();
      v->x = lua_tonumber(L, 1);
      v->y = lua_tonumber(L, 2);
      v->z = lua_tonumber(L, 3);
      v->w = lua_tonumber(L, 4);
      lua_pushlightuserdata(L, (void *)v);
      return 1;
    }

    static int allocate_a_quaternion(lua_State *L) {
      ScriptEnvironment *env = (ScriptEnvironment *)lua_touserdata(L, lua_upvalueindex(1));
      Quaternion *q = env->allocate<Quaternion>();
      q->x = lua_tonumber(L, 1);
      q->y = lua_tonumber(L, 2);
      q->z = lua_tonumber(L, 3);
      q->w = lua_tonumber(L, 4);
      lua_pushlightuserdata(L, (void *)q);
      return 1;
    }

    static int quaternion_from_axis_angle(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Quaternion q = Quaternion::from_axis_angle(script->to_a<Vec3>(1),
                                                       (f32)luaL_checknumber(L, 2));

      script->push<Quaternion>(q);

      return 1;
    }

    static int quaternion_from_euler_angles(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Quaternion q = Quaternion::from_euler_angles((f32)luaL_checknumber(L, 1),
                                                         (f32)luaL_checknumber(L, 2),
                                                         (f32)luaL_checknumber(L, 3));

      script->push<Quaternion>(q);

      return 1;
    }

    static int quaternion_multiply(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Quaternion lhs = script->to_a<Quaternion>(1);
      const Quaternion rhs = script->to_a<Quaternion>(2);

      script->push<Quaternion>(lhs * rhs);

      return 1;
    }

    static int quaternion_rotate(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Quaternion lhs = script->to_a<Quaternion>(1);
      const Vec3 rhs = script->to_a<Vec3>(2);

      script->push<Vec3>(lhs * rhs);

      return 1;
    }

    static int quaternion_nlerp(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Quaternion a = script->to_a<Quaternion>(1);
      const Quaternion b = script->to_a<Quaternion>(2);

      const f32 t = (f32)luaL_checknumber(L, 3);

      script->push<Quaternion>(Quaternion::nlerp(a, b, t));

      return 1;
    }

    static int quaternion_slerp(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Quaternion a = script->to_a<Quaternion>(1);
      const Quaternion b = script->to_a<Quaternion>(2);

      const f32 t = (f32)luaL_checknumber(L, 3);

      script->push<Quaternion>(Quaternion::slerp(a, b, t));

      return 1;
    }

    static int allocate_a_mat4(lua_State *L) {
      ScriptEnvironment *env = (ScriptEnvironment *)lua_touserdata(L, lua_upvalueindex(1));
      Mat4 *m = env->allocate<Mat4>();
      *m = Mat4::IDENTITY;
      lua_pushlightuserdata(L, (void *)m);
      return 1;
    }

    // PERF(mtwilliams): Reduce copies introduced by `push` and `to_a`.

    static int mat4_identity(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));
      script->push<Mat4>(Mat4::IDENTITY);
      return 1;
    }

    static int mat4_perspective(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Mat4 matrix = Mat4::perspective((f32)luaL_checknumber(L, 1),
                                            (f32)luaL_checknumber(L, 2),
                                            (f32)luaL_checknumber(L, 3),
                                            (f32)luaL_checknumber(L, 4));

      script->push<Mat4>(matrix);

      return 1;
    }

    static int mat4_orthographic(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Mat4 matrix = Mat4::orthographic((f32)luaL_checknumber(L, 1),
                                             (f32)luaL_checknumber(L, 2),
                                             (f32)luaL_checknumber(L, 3),
                                             (f32)luaL_checknumber(L, 4),
                                             (f32)luaL_checknumber(L, 5),
                                             (f32)luaL_checknumber(L, 6));

      script->push<Mat4>(matrix);

      return 1;
    }

    static int mat4_translation(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Mat4 matrix = Mat4::translation(script->to_a<Vec3>(1));

      script->push<Mat4>(matrix);

      return 1;
    }

    static int mat4_rotation(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Mat4 matrix = Mat4::rotation(script->to_a<Quaternion>(1));

      script->push<Mat4>(matrix);

      return 1;
    }

    static int mat4_scale(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Mat4 matrix = Mat4::scale(script->to_a<Vec3>(1));

      script->push<Mat4>(matrix);

      return 1;
    }

    static int mat4_compose(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Mat4 matrix = Mat4::compose(script->to_a<Vec3>(1),
                                        script->to_a<Quaternion>(2),
                                        script->to_a<Vec3>(3));

      script->push<Mat4>(matrix);

      return 1;
    }

    static int mat4_decompose(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      Vec3 position;
      Quaternion rotation;
      Vec3 scale;

      Mat4::decompose(script->to_a<Mat4>(1), &position, &rotation, &scale);

      script->push<Vec3>(position);
      script->push<Quaternion>(rotation);
      script->push<Vec3>(scale);

      return 3;
    }

    static int mat4_determinant(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));
      lua_pushnumber(L, (lua_Number)script->to_a<Mat4>(1).determinant());
      return 1;
    }

    static int mat4_inverse(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));
      script->push<Mat4>(script->to_a<Mat4>(1).inverse());
      return 1;
    }

    static int mat4_transpose(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));
      script->push<Mat4>(script->to_a<Mat4>(1).transpose());
      return 1;
    }

    static int mat4_multiply(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Mat4 lhs = script->to_a<Mat4>(1);
      const Mat4 rhs = script->to_a<Mat4>(2);

      script->push<Mat4>(lhs * rhs);

      return 1;
    }

    static int mat4_rotate(lua_State *L) {
      Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));

      const Mat4 lhs = script->to_a<Mat4>(1);
      const Vec3 rhs = script->to_a<Vec3>(2);

      script->push<Vec3>(lhs * rhs);

      return 1;
    }
  }
} // math_if

static int error_handler(lua_State *L) {
  core::logf(core::log::SCRIPT, core::log::ERROR, lua_tostring(L, -1));
  return 0;
}

void math_if::expose(Script *script) {
  lua_State *L = script->state();

  #define TYPES(_) _("Vec2",       &allocate_a_vec2) \
                   _("Vec3",       &allocate_a_vec3) \
                   _("Vec4",       &allocate_a_vec4) \
                   _("Quaternion", &allocate_a_quaternion) \
                   _("Mat4",       &allocate_a_mat4)

  #define BIND_TO_LUA(Type, Allocator)                                       \
    lua_createtable(L, 0, 1);                                                \
    lua_pushlightuserdata(L, (void *)script->environment());                 \
    lua_pushcclosure(L, Allocator, 1);                                       \
    lua_setfield(L, -2, "allocate");                                         \
    lua_setglobal(L, "_" Type);

  #define BIND_TO_C(Type, _)                                                 \
    lua_getglobal(L, "_" Type);                                              \
    lua_getfield(L, -1, "cast");                                             \
    lua_setfield(L, LUA_REGISTRYINDEX, "_" Type ".cast");                    \
    lua_pop(L, 1);

  //
  // Bind hidden helpers used to interface with `ScriptEnvironment`.
  //

  TYPES(BIND_TO_LUA);

  //
  // Bind additional helpers we don't want to inline.
  //

  script->add_module_function("_Quaternion", "from_axis_angle", &quaternion_from_axis_angle);
  script->add_module_function("_Quaternion", "from_euler_angles", &quaternion_from_euler_angles);
  script->add_module_function("_Quaternion", "multiply", &quaternion_multiply);
  script->add_module_function("_Quaternion", "rotate", &quaternion_rotate);
  script->add_module_function("_Quaternion", "nlerp", &quaternion_nlerp);
  script->add_module_function("_Quaternion", "slerp", &quaternion_slerp);

  script->add_module_function("_Mat4", "identity", &mat4_identity);
  script->add_module_function("_Mat4", "perspective", &mat4_perspective);
  script->add_module_function("_Mat4", "orthographic", &mat4_orthographic);
  script->add_module_function("_Mat4", "translation", &mat4_translation);
  script->add_module_function("_Mat4", "rotation", &mat4_rotation);
  script->add_module_function("_Mat4", "scale", &mat4_scale);
  script->add_module_function("_Mat4", "compose", &mat4_compose);
  script->add_module_function("_Mat4", "decompose", &mat4_decompose);
  script->add_module_function("_Mat4", "determinant", &mat4_determinant);
  script->add_module_function("_Mat4", "inverse", &mat4_inverse);
  script->add_module_function("_Mat4", "transpose", &mat4_transpose);
  script->add_module_function("_Mat4", "multiply", &mat4_multiply);
  script->add_module_function("_Mat4", "rotate", &mat4_rotate);

  //
  // Everything else is implemented in Lua, using FFI.
  //

  lua_pushcfunction(L, &error_handler);
  luaL_loadstring(L, GLUE);
  lua_pcall(L, 0, 0, -2);

  //
  // Setup cheap references to type casting helpers used by C/C++.
  //

  TYPES(BIND_TO_C);

  #undef TYPES
  #undef BIND_TO_LUA
  #undef BIND_TO_C
}

} // yeti
