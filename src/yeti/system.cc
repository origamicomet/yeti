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

SystemManager::SystemManager()
  : systems_(core::global_heap_allocator(), 256)
  , components_(core::global_heap_allocator())
{
  // Grab every registered component.
  core::Array<Component::Id> ids(core::global_heap_allocator());
  yeti::components(&ids, &components_);

  // Create a system to manage components and insert into our hash table.
  for (unsigned component = 0; component < components_.size(); ++component) {
    void *system = components_[component]->create_a_system(NULL);
    yeti_assert_debug(system != NULL);
    systems_.insert(ids[component], system);
  }
}

SystemManager::~SystemManager() {
  // PERF(mtwilliams): Don't hash name to recover identifiers.
  for (unsigned component = 0; component < components_.size(); ++component) {
    const Component::Id id = component::id_from_name(components_[component]->name);
    void *system = systems_.get(id);
    components_[component]->destroy_a_system(system);
  }
}

void *SystemManager::get(Component::Id id) {
  return systems_.get(id);
}

} // yeti
