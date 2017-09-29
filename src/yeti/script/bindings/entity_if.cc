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

#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

namespace yeti {

template <> bool Script::is_a<Entity>(int index) {
  if (!is_a<Reference>(index))
    return false;

  // Assume it's a reference to an entity.
  return true;
}

template <> Entity Script::to_a<Entity>(int index) {
  if (!is_a<Entity>(index))
    luaL_argerror(L, index, "Expected a reference to an `Entity`.");

  return Entity(to_a<Reference>(index).opaque);
}

template <> void Script::push<Entity>(Entity entity) {
  push<Reference>({entity.id});
}

namespace entity_if {
  namespace {
  }
} // entity_if

void entity_if::expose(Script *script) {
  script->add_module("Entity");
}

} // yeti
