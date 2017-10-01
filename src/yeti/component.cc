//===-- yeti/component.cc -------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/component.h"

namespace yeti {

namespace {
  static core::Array<Component::Id> id_for_components_(core::global_heap_allocator());
  static core::Array<const Component *> components_(core::global_heap_allocator());
  static core::Map<const char *, const Component *> name_to_component_(core::global_heap_allocator(), 256);
}

Component::Id component::id_from_name(const char *name) {
  yeti_assert_debug(name != NULL);
  return (Component::Id)core::murmur_hash_32(name);
}

Component::Id register_a_component(const Component *component) {
  yeti_assert_debug(component != NULL);

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  // Make sure `component->name` is indeed unique.
  for (const Component **I = components_.begin(); I != components_.end(); ++I)
    if (strcmp((*I)->name, component->name) == 0)
      yeti_assert_with_reason(0, "Already registered a component with the name '%s'!", component->name);
#endif

  const Component::Id id = component::id_from_name(component->name);

  id_for_components_.push(id);
  components_.push(component);
  name_to_component_.insert(component->name, component);

  return id;
}

const Component *component_from_id(Component::Id id) {
  yeti_assert_debug(id != Component::INVALID);

  unsigned index = 0;

  for (const Component::Id *I = id_for_components_.begin(); I != id_for_components_.end(); ++I, ++index)
    if (*I == id)
      return components_[index];

  // Not registered.
  return NULL;
}

const Component *component_from_name(const char *name) {
  yeti_assert_debug(name != NULL);

  if (const Component **component = name_to_component_.find(name))
    return *component;

  // Not registered.
  return NULL;
}

void components(core::Array<Component::Id> &ids,
                core::Array<const Component *> &components) {
  ids = id_for_components_;
  components = components_;
}

} // yeti
