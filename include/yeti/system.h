//===-- yeti/system.h -----------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_SYSTEM_H_
#define _YETI_SYSTEM_H_

#include "yeti/core.h"

// Inescapably coupled to entities and components.
#include "yeti/entity.h"
#include "yeti/component.h"

namespace yeti {

/// Common interface for systems that manage instances of a component.
class YETI_PUBLIC System {
 YETI_DISALLOW_COPYING(System)

 // Hide internals from users.
 friend class SystemManager;

 protected:
  System();
  ~System();

 public:
  /// Gets the value of a property.
  template <typename T>
  T get(const char *property);

  /// Sets the value of a property.
  template <typename T>
  void set(const char *property, const T &value);

 protected:
  virtual void created(Entity entity);
  virtual void destroyed(Entity entity);
};

/// Manages systems.
///
/// # Overview
///
/// System managers automatically create, destroy, and track systems for every
/// registered component. They also glue sub-systems together.
///
/// ## Registry
///
/// Systems managing instances of particular components can be looked up by
/// the component's uneique identifier or name. Systems can also be iterated,
/// to allow call
///
class YETI_PUBLIC SystemManager {
 YETI_DISALLOW_COPYING(SystemManager)

 public:
  SystemManager(EntityManager *entities);
  ~SystemManager();

 public:
  /// Returns the system managing instances of component named @name.
  System *lookup(const char *name);

  /// Returns the system managing instances of @component.
  System *lookup(Component::Id component);

 private:
  /// \internal Forwards entity lifecycle events to a system.
  static void entity_lifecycle_callback_shim(Entity::LifecycleEvent event,
                                             Entity entity,
                                             void *system);

 private:
  EntityManager *entities_;

  // Map of components to systems used to accelerate lookups.
  core::Map<Component::Id, u32, core::map::IdentityHashFunction<u32>::hash> component_to_index_;

  // Components managed by corresponding systems.
  core::Array<const Component *> components_;

  // Systems managing instances of components.
  core::Array<System *> systems_;

  // Callbacks registered (and unregistered) on behalf of systems.
  core::Array<u32> callbacks_;

  // Number of systems.
  unsigned count_;
};

} // yeti

#endif // _YETI_SYSTEM_H_
