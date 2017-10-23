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

// TODO(mtwilliams): Cookies.

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

/// \brief An entity managed by an [`EntityManager`](@ref yeti::EntityManager).
///
/// \note Entities don't actually "exist." Rather, they are weak references
/// used to refer to entities.
///
/// \see yeti::EntityManager
///
struct Entity {
  /// Unique identifier assigned to an entity.
  typedef u32 Id;

  /// Types of lifecycle events.
  enum LifecycleEvent {
    /// Entity was created.
    CREATED = 1,
    /// Entity was destroyed.
    DESTROYED = 2
  };

  /// \copydoc yeti::Entity::Id
  Id id;

  Entity()
    : id(0xFFFFFFFFul)
  {}

  explicit Entity(Id id)
    : id(id)
  {}

  explicit Entity(u32 index, u32 generation)
    : id((index << ENTITY_INDEX_OFFSET) | (generation << ENTITY_GENERATION_OFFSET))
  {}

  /// \internal Returns index of entity corresponding to this handle.
  u32 index() const {
    return (id >> ENTITY_INDEX_OFFSET) & ENTITY_INDEX_MASK;
  }

  /// \internal Returns generation corresponding to this handle.
  u32 generation() const {
    return (id >> ENTITY_GENERATION_OFFSET) & ENTITY_GENERATION_MASK;
  }
};

/// Manages entities.
///
/// # Overview
///
/// Entity managers create, destroy, and track entities. They also facilitate
/// high-level management of entities through a few simple but powerful
/// mechanisms.
///
/// \remark Since entities don't "exist" per se and rather are an abstraction
/// around weak references referring to entities, you can conceptualize this as
/// an index.
///
/// ## Naming
///
/// Entities can be _uniquely_ named and looked up by name in constant time.
/// However, you cannot look up the name of a particular entity. This is a
/// consequence of names being hashed to reduce memory footprint and improve
/// performance.
///
/// ## Lifetime
///
/// As entities are essentially weak references, validity can be quickly
/// checked through `alive` and `dead`.
///
/// ## Ownership
///
/// One or more entities may be associated with an arbitary non-zero 32-bit
/// integer referred to as a "cookie." At any point, various operations can be
/// performed on any entities associated with a particular cookie. This can be
/// used establish weak ownership semantics of entities.
///
/// \remark Cookies shouldn't be used to tag entities as a certain type to
/// drive game logic. Rather, the [`TagSystem`](@ref yeti::TagSystem) should
/// be used. For example, each spawn point could be tagged as such and iterated
/// on load to build internal data structures necessary for spawning or
/// respawning players or enemies.
///
/// ### Garbage Collection
///
/// Rather than relying on callbacks, you can periodically check if entities
/// are alive to perform "garbage collection" of data associated with a
/// particular entity.
///
/// \warning As liveliness is implemented by counting underlying reuse of slots
/// there is a very small possibility of erroneously considering an entity
/// alive when it is actually dead. Good practice is to check relatively
/// frequently to prevent that from happening.
///
/// ## Callbacks
///
/// You can dynamically register and unregister callbacks that are called
/// whenever an entity is created or destroyed.
///
class YETI_PUBLIC EntityManager {
 YETI_DISALLOW_COPYING(EntityManager)

 public:
  EntityManager(unsigned size = 1048576);
  ~EntityManager();

 public:
  /// Creates an entity.
  Entity create();

  /// Creates @n entities, storing the handles in @entities.
  void create(Entity *entities, unsigned n);

  /// Destroys @entity.
  void destroy(Entity entity);

  /// Determines if @entity is alive.
  bool alive(Entity entity) const;

  /// Determines if @entity is dead.
  bool dead(Entity entity) const;

  /// Names @entity.
  void name(Entity entity, const char *name);

  /// Names @entity.
  void name(Entity entity, const u32 hash_of_name);

  /// Names @n entities.
  void name(const Entity *entities, const u32 *hashes_of_names, unsigned n);

  /// Tries to find an entity named @name.
  ///
  /// \return If the entity with @name was found.
  ///
  bool named(const char *name, Entity *entity) const;

  /// Tries to find a named entity.
  ///
  /// \return If the entity was found.
  ///
  bool named(const u32 hash_of_name, Entity *entity) const;

 public:
  typedef void (*LifecycleCallback)(Entity::LifecycleEvent event,
                                    Entity entity,
                                    void *context);

  /// Registers @callback to be called whenever an entity is created or
  /// destroyed.
  ///
  /// \return Identifier that can be used to unregister the callback.
  ///
  u32 register_lifecycle_callback(LifecycleCallback callback,
                                  void *context = NULL);

  /// Unregisters a @calblack.
  void unregister_lifecycle_callback(u32 callback);

 private:
  /// \internal Calls all registered lifecycle callbacks.
  /// @{
  void notify(Entity::LifecycleEvent event, Entity entity);
  void notify(Entity::LifecycleEvent event, const Entity *entities, size_t n);
  /// @}

 public:
  /// Limit on number of entities that can be alive at any point in time.
  unsigned limit() const { return limit_; }

 private:
  /// Limit on number of entities that can be alive at any point in time.
  const unsigned limit_;

  /// Number of entities currently alive.
  unsigned n_;

  /// All entities.
  core::Array<Entity> entities_;

  /// Names associated with entities.
  core::Array<u32> names_;

  /// Map of names to entities used to accelerate lookups of entities by name.
  core::Map<u32, Entity, core::map::IdentityHashFunction<u32>::hash> name_to_entity_;

  /// Cookies associated with entities.
  core::Array<u32> cookies_;

  /// Queue of available indicies.
  core::Queue<u32> free_;

  struct RegisteredLifecycleCallback {
    u32 id;

    LifecycleCallback callback;
    void *context;

    explicit RegisteredLifecycleCallback(u32 id,
                                         LifecycleCallback callback,
                                         void *context)
      : id(id)
      , callback(callback)
      , context(context)
    {}
  };

  /// Lifecycle callbacks.
  core::Array<RegisteredLifecycleCallback> callbacks_;
};

/// Records entity lifecycle events.
class YETI_PUBLIC EntityReflector {
 YETI_DISALLOW_COPYING(EntityReflector)

 public:
  EntityReflector(EntityManager *manager);
  ~EntityReflector();

 private:
  /// \internal Records a lifecycle event to appropriate buffer.
  void record(Entity::LifecycleEvent event, Entity entity);

 private:
  /// \internal Wraps `record` to provide a `Entity::LifecycleCallback` function signature.
  static void shim(Entity::LifecycleEvent event, Entity entity, void *);

 private:
  EntityManager *manager_;

  u32 callback_;

  core::Array<Entity> created_;
  core::Array<Entity> destroyed_;
};

} // yeti

#endif // _YETI_ENTITY_H_
