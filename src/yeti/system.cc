//===-- yeti/system.cc ----------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/system.h"

namespace yeti {

System::System(const Component *component)
  : type_(component_registry::id_from_name(component->name))
{
}

System::~System() {
}

void System::created(Entity entity) {
}

void System::destroyed(Entity entity) {
}

SystemManager::SystemManager(EntityManager *entities)
  : entities_(entities)
  , component_to_index_(core::global_heap_allocator(), 256)
  , components_(core::global_heap_allocator())
  , systems_(core::global_heap_allocator())
  , callbacks_(core::global_heap_allocator())
{
  // Grab every registered component.
  core::Array<Component::Id> ids(core::global_heap_allocator());
  component_registry::registered(&ids, &components_);
  count_ = components_.size();

  // Allocate storage for systems.
  systems_.resize(count_);
  callbacks_.resize(count_);

  // Create 'em, hook up 'em up, and insert 'em into our map.
  for (unsigned i = 0; i < count_; ++i) {
    component_to_index_.insert(ids[i], i);

    systems_[i] = (System *)components_[i]->create_a_system(entities);

    // Automatically register lifecycle callbacks on system's behalf.
    callbacks_[i] = entities->register_lifecycle_callback(&entity_lifecycle_callback_shim,
                                                          (void *)systems_[i]);
  }
}

SystemManager::~SystemManager() {
  // Tear down and destroy systems.
  for (unsigned i = 0; i < count_; ++i) {
    const Component *component = components_[i];

    System *system = systems_[i];

    // Automatically unregister lifecycle callbacks on system's behalf.
    entities_->unregister_lifecycle_callback(callbacks_[i]);

    component->destroy_a_system((void *)system);
  }
}

System *SystemManager::lookup(const char *name) {
  const Component::Id id = component_registry::id_from_name(name);
  return this->lookup(id);
}

System *SystemManager::lookup(Component::Id component) {
  const u32 index = component_to_index_.get(component);
  return systems_[index];
}

void SystemManager::entity_lifecycle_callback_shim(Entity::LifecycleEvent event,
                                                   Entity entity,
                                                   void *system) {
  switch (event) {
    case Entity::CREATED: ((System *)system)->created(entity); break;
    case Entity::DESTROYED: ((System *)system)->destroyed(entity); break;
  }
}

} // yeti
