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
  , generations_(core::global_page_allocator(), size)
  , free_(core::global_page_allocator(), size)
{
  // PERF(mtwilliams): Better bulk insertions.
  for (unsigned index = 0; index < size; ++index) {
    generations_[index] = 0;
    free_.push(index);
  }
}

EntityManager::~EntityManager() {
}

Entity EntityManager::create() {
  yeti_assert_debug(n_ <= limit_);

  const u32 index = free_.pop();
  const u32 generation = generations_[index];

  const Entity entity = Entity(index, generation);

  n_ += 1;

  return entity;
}

bool EntityManager::alive(Entity entity) const {
  return (generations_[entity.index()] == entity.generation());
}

bool EntityManager::dead(Entity entity) const {
  return (generations_[entity.index()] != entity.generation());
}

void EntityManager::destroy(Entity entity) {
  // Prevent users from destroying entities
  yeti_assert_debug(alive(entity));

  // Generation is bumped when destroyed, rather than when allocated, to catch
  // dead entities right away.
  generations_[entity.index()] += 1;

  free_.push(entity.index());

  n_ -= 1;
}

} // yeti
