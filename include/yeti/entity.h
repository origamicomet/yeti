//===-- yeti/entity.h -----------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_ENTITY_H_
#define _YETI_ENTITY_H_

#include "yeti/core.h"

namespace yeti {

/// Bits reserved for index portion of `yeti::Entity::Id`.
static const u32 ENTITY_INDEX_BITS = 20;

/// Mask for index portion of `yeti::Entity::Id`.
static const u32 ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

/// Offset, in bits, for index portion of `yeti::Entity::Id`.
static const u32 ENTITY_INDEX_OFFSET = 4;

/// Bits reserved for generation portion of `yeti::Entity::Id`.
static const u32 ENTITY_GENERATION_BITS = 8;

/// Mask for generation portion of `yeti::Entity::Id`.
static const u32 ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;

/// Offset, in bits, for generation portion of `yeti::Entity::Id`.
static const u32 ENTITY_GENERATION_OFFSET = 24;

struct Entity {
  /// Unique identifier assigned to an entity.
  typedef u32 Id;

  /// \copydoc yeti::Entity::Id
  Id id;

  explicit Entity(Id id)
    : id(id)
  {}

  explicit Entity(u32 index, u32 generation)
    : id((index << ENTITY_INDEX_OFFSET) | (generation << ENTITY_GENERATION_OFFSET))
  {}

  /// Returns index of entity corresponding to this handle.
  u32 index() const {
    return (id >> ENTITY_INDEX_OFFSET) & ENTITY_INDEX_MASK;
  }

  /// Returns generation corresponding to this handle.
  u32 generation() const {
    return (id >> ENTITY_GENERATION_OFFSET) & ENTITY_GENERATION_MASK;
  }
};

class YETI_PUBLIC EntityManager {
 YETI_DISALLOW_COPYING(EntityManager)

 public:
  EntityManager(unsigned size = 1048576);
  ~EntityManager();

 public:
  /// Creates an entity, returning it.
  Entity create();

  /// Creates @n entities, storing the handles in @entities.
  void create(Entity *entities, unsigned n);

  /// Determines if @entity is alive.
  bool alive(Entity entity) const;

  /// Determines if @entity is dead.
  bool dead(Entity entity) const;

  /// Destroys @entity.
  void destroy(Entity entity);

 private:
  /// Upper limit on number of entities that can be alive.
  const unsigned limit_;

  /// Number of alive entities.
  unsigned n_;

  /// Generations of all entities.
  core::Array<u8> generations_;

  /// Queue of available indicies.
  core::Queue<u32> free_;
};

} // yeti

#endif // _YETI_ENTITY_H_
