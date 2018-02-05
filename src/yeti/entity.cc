//===-- yeti/entity.cc ----------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/entity.h"

namespace yeti {

EntityManager::EntityManager(unsigned size)
  : limit_(size)
  , n_(0)
  , entities_(core::global_page_allocator(), size)
  , names_(core::global_page_allocator(), size)
  , name_to_entity_(core::global_page_allocator(), size)
  , parent_(core::global_page_allocator(), size)
  , children_(core::global_page_allocator(), size)
  , next_(core::global_page_allocator(), size)
  , previous_(core::global_page_allocator(), size)
  , cookies_(core::global_page_allocator(), size)
  , free_(core::global_page_allocator(), size)
  , callbacks_(core::global_heap_allocator())
{
  // PERF(mtwilliams): Optimize bulk insertions.
  for (unsigned index = 0; index < size; ++index) {
    entities_[index] = Entity(index, 0);
    names_[index]    = 0;
    parent_[index]   = -1;
    children_[index] = -1;
    next_[index]     = -1;
    previous_[index] = -1;
    cookies_[index]  = 0;
    free_.push(index);
  }
}

EntityManager::~EntityManager() {
}

Entity EntityManager::create() {
  yeti_assert_debug(n_ <= limit_);

  u32 slot;

  // Grab next available slot.
  free_.pop(&slot);

  n_ += 1;

  const Entity entity = entities_[slot];

  this->notify(Entity::CREATED, entity);

  return entity;
}

void EntityManager::create(Entity *entities, unsigned n) {
  yeti_assert_debug(entities != NULL);
  yeti_assert_debug(n_ + n <= limit_);

  for (unsigned entity = 0; entity < n; ++entity) {
    u32 slot;

    // Grab next available slot.
    free_.pop(&slot);

    entities[entity] = entities_[slot];
  }

  n_ += n;

  // Deferred to reduce instruction cache evictions and improve branch prediction.
  this->notify(Entity::CREATED, entities, n);
}

void EntityManager::destroy(Entity entity) {
  if (alive(entity))
    this->destroy_for_real(entity);
}

// PERF(mtwilliams): Defer notifications so we can process in a batch and to
// reduce cache thrashing.

void EntityManager::destroy_for_real(Entity entity) {
  const u32 index = entity.index();
  const u8 generation = entity.generation();

  // Generation is bumped when destroyed, rather than when allocated, to
  // catch dead entities right away.
  entities_[index] = Entity(index, generation + 1);

  if (const u32 name = names_[index])
    name_to_entity_.remove(name);

  names_[index] = 0;

  if (parent_[index] != -1) {
    // Unlink to prevent collateral damage when parent is destroyed.
    if (previous_[index] != -1)
      next_[previous_[index]] = next_[index];
    else
      children_[parent_[index]] = next_[index];

    // No longer a part of the hierarchy.
    parent_[index] = next_[index] = previous_[index] = -1;
  }

  if (children_[index] != -1)
    // Destroy logical children.
    this->destroy_all_children(index);

  cookies_[index] = 0;

  // Slot will be reused after any children.
  free_.push(index);

  n_ -= 1;

  this->notify(Entity::DESTROYED, entity);
}

void EntityManager::destroy_all_children(u32 index_of_parent) {
  for (u32 index = children_[index_of_parent]; index != -1; index = next_[index]) {
    const Entity child = entities_[index];

    // Generation is bumped when destroyed, rather than when allocated, to
    // catch dead entities right away.
    entities_[index] = Entity(index, child.generation() + 1);

    if (const u32 name = names_[index])
      name_to_entity_.remove(name);

    names_[index] = 0;

    // We know we have a parent since we're being destroyed because of it.
    parent_[index] = -1;

    if (children_[index] != -1) {
      // Destroy any logical children of our own.
      this->destroy_all_children(index);

      // No longer have children.
      children_[index] = -1;
    }

    cookies_[index] = 0;

    // Slot will be reused after any children.
    free_.push(index);

    n_ -= 1;

    // Let listeners know about our path of destruction.
    this->notify(Entity::DESTROYED, child);
  }
}

bool EntityManager::alive(Entity entity) const {
  // PERF(mtwilliams): Check without shifting.
  return (entities_[entity.index()].generation() == entity.generation());
}

bool EntityManager::dead(Entity entity) const {
  // PERF(mtwilliams): Check without shifting.
  return (entities_[entity.index()].generation() != entity.generation());
}

void EntityManager::name(Entity entity, const char *name) {
  yeti_assert_debug(name != NULL);

  const u32 hash_of_name = core::murmur_hash_32(name);

  this->name(entity, hash_of_name);
}

// TODO(mtwilliams): Handle renames.

void EntityManager::name(Entity entity, const u32 hash_of_name) {
  yeti_assert_debug(alive(entity));
  yeti_assert_debug(hash_of_name != 0);

  const u32 index = entity.index();

  if (const u32 previous = names_[index])
    // Rename.
    name_to_entity_.remove(previous);

  name_to_entity_.insert(hash_of_name, entity);
  names_[index] = hash_of_name;
}

void EntityManager::name(const Entity *entities, const u32 *hashes_of_names, unsigned n) {
  yeti_assert_debug(entities != NULL);
  yeti_assert_debug(hashes_of_names != NULL);

  for (unsigned entity = 0; entity < n; ++entity) {
    const u32 index = entities[entity].index();

    if (const u32 previous = names_[index])
      // Rename.
      name_to_entity_.remove(previous);

    name_to_entity_.insert(hashes_of_names[entity], entities[entity]);
    names_[index] = hashes_of_names[entity];
  }
}

bool EntityManager::named(const char *name, Entity *entity) const {
  yeti_assert_debug(name != NULL);

  const u32 hash_of_name = core::murmur_hash_32(name);

  return this->named(hash_of_name, entity);
}

bool EntityManager::named(const u32 hash_of_name, Entity *entity) const {
  yeti_assert_debug(hash_of_name != 0);
  yeti_assert_debug(entity != NULL);

  if (const Entity *ptr_to_entity = name_to_entity_.find(hash_of_name)) {
    *entity = *ptr_to_entity;
    return true;
  }

  return false;
}

u32 EntityManager::register_lifecycle_callback(LifecycleCallback callback,
                                               void *context) {
  static u32 id = 0;

  callbacks_.push(RegisteredLifecycleCallback(++id, callback, context));

  return id;
}

void EntityManager::unregister_lifecycle_callback(u32 callback) {
  for (u32 index = 0; index < callbacks_.size(); ++index) {
    if (callbacks_[index].id != callback)
      continue;

    // Swap 'n' pop.
    callbacks_[index] = callbacks_[callbacks_.size() - 1];
    callbacks_.pop();

    break;
  }
}

void EntityManager::notify(Entity::LifecycleEvent event, Entity entity) {
  for (const RegisteredLifecycleCallback *I = callbacks_.begin(); I != callbacks_.end(); ++I)
    I->callback(event, entity, I->context);
}

void EntityManager::notify(Entity::LifecycleEvent event, const Entity *entities, size_t n) {
  for (const RegisteredLifecycleCallback *I = callbacks_.begin(); I != callbacks_.end(); ++I)
    for (unsigned entity = 0; entity < n; ++entity)
      I->callback(event, entities[entity], I->context);
}

EntityReflector::EntityReflector(EntityManager *manager)
  : manager_(manager)
  , callback_(manager_->register_lifecycle_callback(&shim, (void *)this))
  , created_(core::global_heap_allocator())
  , destroyed_(core::global_heap_allocator())
{
}

EntityReflector::~EntityReflector() {
  manager_->unregister_lifecycle_callback(callback_);
}

void EntityReflector::record(Entity::LifecycleEvent event, Entity entity) {
  switch (event) {
    case Entity::CREATED:
      created_.push(entity);
      break;

    case Entity::DESTROYED:
      destroyed_.push(entity);
      break;
  }
}

void EntityReflector::shim(Entity::LifecycleEvent event,
                           Entity entity,
                           void *entity_reflector_ptr) {
  ((EntityReflector *)entity_reflector_ptr)->record(event, entity);
}

} // yeti
