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

namespace component_registry {
  namespace {
    static core::Array<Component::Id> ids_(core::global_heap_allocator());
    static core::Array<const Component *> components_(core::global_heap_allocator());

    // Map of identifiers to componments used to accelerate lookups of components by identifier or name.
    static core::Map<Component::Id, const Component *, core::map::IdentityHashFunction<Component::Id>::hash> id_to_component_(core::global_heap_allocator(), 256);

    // Number of registered components.
    static size_t n_ = 0;

    // Component to register during boot.
    struct DeferredRegistration {
      const Component *component;
    };

    // Components to register during boot.
    static DeferredRegistration deferreds_[256];

    // Number of components to register during boot.
    static size_t num_of_deferreds_;
  }

  // Performs all deferred registrations. Called by `yeti::boot`.
  void perform_deferred_registrations();
}

Component::Id component_registry::register_a_component(const Component *component) {
  yeti_assert_debug(component != NULL);

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  // Make sure `component->name` is indeed unique.
  for (const Component **I = components_.begin(); I != components_.end(); ++I)
    if (strcmp((*I)->name, component->name) == 0)
      yeti_assert_with_reason(0, "Already registered a component with the name '%s'!", component->name);
#endif

  const Component::Id id = id_from_name(component->name);

  ids_.push(id);
  components_.push(component);

  id_to_component_.insert(id, component);

  n_ += 1;

  return id;
}

void component_registry::register_during_boot(const Component *component) {
  yeti_assert_debug(component != NULL);

  deferreds_[num_of_deferreds_++].component = component;
}

void component_registry::perform_deferred_registrations() {
  for (unsigned deferred = 0; deferred < num_of_deferreds_; ++deferred)
    register_a_component(deferreds_[deferred].component);
}

Component::Id component_registry::id_from_name(const char *name) {
  yeti_assert_debug(name != NULL);
  return (Component::Id)core::murmur_hash_32(name);
}

const Component *component_registry::component_by_id(Component::Id id) {
  yeti_assert_debug(id != Component::INVALID);

  if (const Component **ptr_to_component = id_to_component_.find(id))
    return *ptr_to_component;

  // No component with the specified identifier.
  return NULL;
}

const Component *component_registry::component_by_name(const char *name) {
  return component_by_id(id_from_name(name));
}

void component_registry::for_each_registered(void (*callback)(Component::Id id, const Component *component, void *context),
                                             void *context) {
  yeti_assert_debug(callback != NULL);

  for (unsigned component = 0; component < n_; ++component)
    callback(ids_[component], components_[component], context);
}

void component_registry::registered(core::Array<Component::Id> *ids,
                                    core::Array<const Component *> *components) {
  if (ids)
    *ids = ids_;
  if (components)
    *components = components_;
}

} // yeti
