//===-- yeti/component.h --------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_COMPONENT_H_
#define _YETI_COMPONENT_H_

#include "yeti/core.h"

namespace yeti {

// Forward declared because callback signatures reference `Entity` and `EntityManager`.
struct Entity;
class EntityManager;

namespace component {
  /// An opaque identifier assigned to a component based on its name.
  typedef u32 Id;

  /// Invalid identifier.
  static const Id INVALID = 0ul;

  /// Derives a component's identifier from its name.
  extern YETI_PUBLIC Id id_from_name(const char *name);
}

namespace component_compiler {
  struct Environment {
    /// Logs an informational message.
    void (*info)(const Environment *env, const char *format, ...);

    /// Logs a warning.
    void (*warning)(const Environment *env, const char *format, ...);

    /// Logs an error message.
    void (*error)(const Environment *env, const char *format, ...);
  };

  struct Input {
    // const json &data;
  };

  struct Output {
    /// Version of data.
    u32 version;

    /// Handle to write compiled data.
    core::File *data;
  };
}

struct Component {
  /// \copydoc yeti::component::Id
  typedef component::Id Id;

  /// \copydoc yeti::component::INVALID
  static const Id INVALID = component::INVALID;

  /// Name of the component. Used as a unique identifier.
  ///
  /// \note Convention is to use a lowercase sigular nouns joined by
  /// underscores. For example, `multi_mesh` fits convention and is easily
  /// understood while in the contexts it is used.
  ///
  const char *name;

  /// Arbitary number that determines the version of this component. Each
  /// instance of this component is tagged with this during compilation,
  /// allowing the engine and tools to quickly determine compatibility.
  ///
  /// \note If versions match, compatibility is assumed. When changing the
  /// format of compiled data, make sure to bump the version number and
  /// adjust the `compatible` callback accordingly.
  ///
  /// \note If version is `0`, incompatibility is always assumed. However,
  /// compatibility is ignored at runtime. Set version to `0` until you've
  /// stabilized your compiled data format.
  ///
  u32 version;

  // TODO(mtwilliams): Determine order automatically by letting components
  // specify dependencies.

  /// Compilation and spawn order relevant to other components.
  ///
  /// \note If order is `0`, then any order is considered valid. Otherwise,
  /// components are compiled and spawned from lowest order to highest.
  ///
  u32 order;

  /// Creates a system that manages instances of this component.
  ///
  /// \param @entities Manager of entities that will be associated with
  /// instances from the system.
  ///
  /// \return Opaque pointer to system.
  ///
  void *(*create_a_system)(EntityManager *entities);

  /// Destroys a system created by `create_a_system`.
  void (*destroy_a_system)(void *system);

  /// Creates components from compiled data.
  ///
  /// \param @system System managing instances of this component.
  /// \param @entities Entities being spawned.
  /// \param @owners Indicies into @entities that identity the owners of instances.
  /// \param @instances Number of instances to spawn.
  /// \param @data Compiled data.
  ///
  void (*spawn)(void *system,
                const Entity *entities,
                const u32 *owners,
                size_t instances,
                const void *data);

  /// Tries to compile a component from source data.
  bool (*compile)(const component_compiler::Environment *env,
                  const component_compiler::Input *input,
                  const component_compiler::Output *output);

  /// Determines if a component compiled with @version is compatible with this
  /// version of code.
  bool (*compatible)(u32 version);
};

/// Registers a component.
///
/// \warning Registering components is *not* thread-safe!
///
/// \param @component Pointer to description of the component.
///
/// \return Identifier assigned to the component.
///
extern YETI_PUBLIC Component::Id register_a_component(const Component *component);

/// Looks up a component by identifier.
///
/// \param @id Id of the component to look up.
///
/// \return Component or `NULL` if no component with @id exists.
///
extern YETI_PUBLIC const Component *component_from_id(Component::Id id);

/// Finds a component by name.
///
/// \param @name The name of the component to find.
///
/// \return Named component or `NULL` if no component named @name exists.
///
extern YETI_PUBLIC const Component *component_from_name(const char *name);

/// Fills arrays with identifiers and descriptions of every registered component.
///
/// \param @ids Identifiers assigned to components.
/// \params @components Every registered component.
///
extern YETI_PUBLIC void components(core::Array<Component::Id> *ids,
                                   core::Array<const Component *> *components);

} // yeti

#endif // _YETI_COMPONENT_H_
