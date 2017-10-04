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

// Forward declared because callback signatures reference `component_compiler`.
namespace component_compiler {
  struct Environment;
  struct Input;
  struct Output;
}

/// Description of a component.
///
/// \see yeti::component_registry
///
struct Component {
  /// An opaque identifier assigned to a component dervied from its name.
  typedef u32 Id;

  /// Invalid identifier.
  static const Id INVALID = 0ul;

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

  /// Description of a property of a particular component.
  struct Property {
    enum Type {
      /// True or false.
      BOOLEAN  = 1,
      /// Signed integer.
      SIGNED   = 2,
      /// Unsigned integer.
      UNSIGNED = 3,
      /// Single-precision floating-point number.
      FLOAT    = 4,
      /// Null-terminated string.
      STRING   = 5,
      /// A reference to an entity.
      ENTITY   = 6,
      /// A reference to an instance.
      INSTANCE = 7
    };

    /// Name of the property.
    ///
    /// \note Convention is to use a lowercase sigular nouns joined by
    /// underscores. For example, `point_of_origin` fits convention and is
    /// easily understood in the contexts it is used.
    ///
    const char *name;

    /// Type of value.
    Type type;

    /// Number of values.
    u32 count;
  };

  /// Description of properties of the component.
  /// @{
  const Property *properties;
  u32 num_of_properties;
  /// @}

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

  /// Compiles a component from source data.
  bool (*compile)(const component_compiler::Environment *env,
                  const component_compiler::Input *input,
                  const component_compiler::Output *output);

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

  /// Determines if a component compiled with @version is compatible with this
  /// version of code.
  bool (*compatible)(u32 version);
};

/// Tracks components.
///
/// # Overview
///
/// The component registry tracks components and provides centralized
/// mechanisims for identifying and using components.
///
/// # Registration
///
/// Components _must_ be registered to be used. To simplify registration, you
/// can use [`YETI_AUTO_REGISTER_COMPONENT`](@ref YETI_AUTO_REGISTER_COMPONENT)
/// to automatically register a component.
///
namespace component_registry {
  /// Registers a component.
  ///
  /// \warning Registering components is *not* thread-safe!
  ///
  /// \return Identifier assigned to the component.
  ///
  extern YETI_PUBLIC Component::Id register_a_component(const Component *component);

  /// Derives a component's identifier from its name.
  extern YETI_PUBLIC Component::Id id_from_name(const char *name);

  /// Looks up a component by identifier.
  ///
  /// \return Component or `NULL` if no component with @id exists.
  ///
  extern YETI_PUBLIC const Component *component_by_id(Component::Id id);

  /// Looks up a component by name.
  ///
  /// \return Named component or `NULL` if no component named @name exists.
  ///
  extern YETI_PUBLIC const Component *component_by_name(const char *name);

  /// Calls @callback for every registered component.
  extern YETI_PUBLIC void for_each_registered(void (*callback)(Component::Id id, const Component *component, void *context),
                                              void *context = NULL);

  /// Fills arrays with identifiers and descriptions of every registered
  /// component.
  ///
  /// \param @ids Identifiers assigned to registered components.
  /// \params @components Descriptions of registered component.
  ///
  extern YETI_PUBLIC void registered(core::Array<Component::Id> *ids,
                                     core::Array<const Component *> *components);
}

/// \def YETI_AUTO_REGISTER_COMPONENT
/// \brief Automatically registers [`Component`](@ref yeti::Component) with
/// the component registry.
#define YETI_AUTO_REGISTER_COMPONENT(Component) \
  static const ::yeti::AutoRegisterComponent YETI_PASTE(__automatic_component_registration__, __LINE__)(Component);

/// \internal Automatically registers a [`Component`](@ref yeti::Component)
/// with the component registry upon construction.
class YETI_PUBLIC AutoRegisterComponent {
 YETI_DISALLOW_COPYING(AutoRegisterComponent)

 public:
  AutoRegisterComponent(const Component *component)
    : id_(component_registry::register_a_component(component))
    , component_(component)
  {}

  ~AutoRegisterComponent() {}

 private:
  const Component::Id id_;
  const Component *const component_;
};

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

} // yeti

#endif // _YETI_COMPONENT_H_
