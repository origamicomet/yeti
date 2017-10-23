//===-- yeti/components/light.h -------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_COMPONENTS_LIGHT_H_
#define _YETI_COMPONENTS_LIGHT_H_

#include "yeti/core.h"

#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

#include "yeti/math.h"
#include "yeti/color.h"

// TODO(mtwilliams): Temperature based parameterization.
// TODO(mtwilliams): Wave length based parameterization.
// TODO(mtwilliams): Area lights.
// TODO(mtwilliams): IES.
// TODO(mtwilliams): Gobos.

namespace yeti {

/// A directional light source.
struct DirectionalLight {
  /// Color of light emitted.
  Color color;

  /// Energy emitted in lux.
  f32 intensity;
};

/// A point light source.
struct PointLight {
  /// Distance up to which light affects.
  f32 radius;

  /// Color of light emitted.
  /// \note Alpha is ingored.
  Color color;

  /// Energy emitted in lumens.
  f32 intensity;
};

/// A spot light source.
struct SpotLight {
  /// Distance up to which light affects.
  f32 radius;

  /// Angle up to which the light affects, expressed in radians.
  f32 angle;

  /// Color of light emitted.
  /// \note Alpha is ingored.
  Color color;

  /// Energy emitted in lumens.
  f32 intensity;
};

/// A light source.
struct Light {
  /// Opaque handle to a light component.
  struct Handle {
    u32 instance;
  };

  enum Type {
    UNKNOWN     = 0,
    DIRECTIONAL = 1,
    POINT       = 2,
    SPOT        = 3
  };

  enum Flag {
    /// Contributes light.
    ENABLED = 1 << 0,

    /// Contribution is computed offline.
    BAKED = 1 << 1,

    /// Does not contribute to indirect lighting.
    VIRTUAL = 1 << 2,

    /// Shadow casting.
    SHADOWS = 1 << 3,

    /// Volumetric.
    VOLUMETRIC = 1 << 4
  };

  Light::Type type;

  union {
    DirectionalLight directional;
    PointLight point;
    SpotLight spot;
  };

  u32 flags;

  // Define explicit constructor, copy constructor, and assignment operator
  // because members of anonymous union have non-trivial constructors.

  Light() {
    core::memory::zero((void *)this, sizeof(Light));
  }

  Light(const Light &light) {
    core::memory::copy((const void *)&light, (void *)this, sizeof(Light));
  }

  Light operator=(const Light &light) {
    core::memory::copy((const void *)&light, (void *)this, sizeof(Light));
    return *this;
  }
};

namespace light {
  extern YETI_PUBLIC Light::Type type_from_string(const char *type);
  extern YETI_PUBLIC const char *type_to_string(const Light::Type type);
}

class YETI_PUBLIC LightSystem : public System {
 YETI_DISALLOW_COPYING(LightSystem)

 private:
  LightSystem(EntityManager *entities);
  ~LightSystem();

 public:
  /// \brief Creates a new light associated with @entity.
  Light::Handle create(Entity entity);

  /// \brief Destroys a particular light.
  void destroy(Light::Handle handle);

  /// \brief Destroys all lights associated with @entity.
  void destroy(Entity entity);

  /// \brief Returns a handle to a light component associated with @entity.
  Light::Handle lookup(Entity entity, unsigned instance = 0);

  /// \brief Determines whether or not @entity has any associated lights.
  bool has(Entity entity) const;

  /// \brief Returns the type of a light.
  Light::Type get_type(Light::Handle handle) const;

  /// \brief Returns the distance where intensity reaches zero.
  ///
  /// \note This does not apply to directional lights!
  ///
  f32 get_radius(Light::Handle handle) const;

  /// \brief Returns the angle where intensity reaches zero in radians.
  ///
  /// \note This only applies to spot lights!
  ///
  f32 get_angle(Light::Handle handle) const;

  /// \brief Returns the color of a light.
  Color get_color(Light::Handle handle) const;

  /// \brief Returns the intensity of a light.
  ///
  /// \return Intensity in lumens for point lights and spot lights, or lux for
  /// directional lights.
  ///
  f32 get_intensity(Light::Handle handle) const;

  /// \brief Returns flags set on a light.
  u32 flags(Light::Handle handle) const;

  /// \brief Sets the type of a light.
  void set_type(Light::Handle handle, Light::Type type);

  /// \brief Sets the distance where intensity reaches zero.
  ///
  /// \param @distance Distance.
  ///
  /// \note This does not apply to directional lights!
  ///
  void set_radius(Light::Handle handle, f32 radius);

  /// \brief Sets the angle where intensity reaches zero.
  ///
  /// \param @angle Angle, in radians.
  ///
  /// \note This only applies to spot lights!
  ///
  void set_angle(Light::Handle handle, f32 angle);

  /// \brief Sets the color of a light.
  void set_color(Light::Handle handle, const Color &color);

  /// \brief Sets the intensity of a light.
  ///
  /// \note Intensity is given in lumens for point lights and spot lights, or
  /// lux for directional lights.
  ///
  void set_intensity(Light::Handle handle, f32 intensity);

  /// \brief Sets flags on a light.
  ///
  /// \note Only modifies flags in @mask.
  ///
  void set_flags(Light::Handle handle, u32 flags, u32 mask = 0xFFFFFFFFul);

  /// \brief Returns a complete description of a light.
  Light describe(Light::Handle handle) const;

 public:
  /// \internal Description of this component.
  static const Component *component();

 private:
  /// \internal Callbacks and shims exposed through `Component`.
  /// @{
  static void *create(EntityManager *entities);
  static void destroy(void *system);
  /// @}

 private:
  EntityManager *entities_;

  // As there are magnitudes less light than entities at any point in time, a
  // map results in drastically less overhead at the cost of marginally more
  // expensive lookups.
  core::Map<Entity, u32> entity_to_instance_;

  // Used for reverse lookups.
  core::Array<Entity> instance_to_entity_;

  core::Array<Light> lights_;

  core::Array<u32> siblings_;
};

} // yeti

#endif // _YETI_COMPONENTS_LIGHT_H_
