//===-- yeti/components/camera.h ------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_COMPONENTS_CAMERA_H_
#define _YETI_COMPONENTS_CAMERA_H_

#include "yeti/core.h"

#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

namespace yeti {

struct Camera {
  /// Opaque handle to a camera component.
  struct Handle {
    u32 instance;
  };

  /// Type of projection.
  enum Type {
    UNKNOWN      = 0,
    /// Perspective projection.
    PERSPECTIVE  = 1,
    /// Orthographic projection.
    ORTHOGRAPHIC = 2
  };

  /// \copydoc yeti::Camera::Type
  Type type;

  union {
    struct {
      /// Horizontal field of view.
      f32 fov;
    } perspective;

    struct {
      f32 top;
      f32 left;
      f32 bottom;
      f32 right;
    } orthographic;
  };

  f32 near;
  f32 far;
};

namespace camera {
  extern YETI_PUBLIC Camera::Type type_from_string(const char *type);
  extern YETI_PUBLIC const char *type_to_string(const Camera::Type type);
}

class YETI_PUBLIC CameraSystem : public System {
 YETI_DISALLOW_COPYING(CameraSystem)

 private:
  CameraSystem(EntityManager *entities);
  ~CameraSystem();

 public:
  /// \brief Creates a new camera associated with @entity.
  Camera::Handle create(Entity entity);

  /// \brief Destroys a particular camera.
  void destroy(Camera::Handle handle);

  /// \brief Destroys all cameras associated with @entity.
  void destroy(Entity entity);

  /// \brief Returns a handle to a camera component associated with @entity.
  Camera::Handle lookup(Entity entity, unsigned instance = 0);

  /// \brief Determines whether or not @entity has any associated cameras.
  bool has(Entity entity) const;

  /// \brief Returns the type of projection used by a camera.
  Camera::Type get_type(Camera::Handle handle) const;

  /// \brief Returns the horizontal field-of-view of a camera.
  f32 get_field_of_view(Camera::Handle handle) const;

  /// \brief Returns the viewing volume of a camera.
  void get_box(Camera::Handle handle, f32 *top, f32 *left, f32 *bottom, f32 *right) const;

  /// \brief Returns the near plane of a camera.
  f32 get_near_plane(Camera::Handle handle) const;

  /// \brief Returns the far plane of a camera.
  f32 get_far_plane(Camera::Handle handle) const;

  /// \brief Sets the type of projection used by a camera.
  void set_type(Camera::Handle handle, Camera::Type type);

  /// \brief Sets the horizontal field-of-view of a camera.
  void set_field_of_view(Camera::Handle handle, f32 field_of_view);

  /// \brief Sets the viewing volume of a camera.
  void set_box(Camera::Handle handle, f32 top, f32 left, f32 bottom, f32 right);

  /// \brief Sets the near plane of a camera.
  void set_near_plane(Camera::Handle handle, f32 near_plane);

  /// \brief Sets the far plane of a camera.
  void set_far_plane(Camera::Handle handle, f32 far_plane);

  /// \brief Returns a complete description of a camera.
  Camera describe(Camera::Handle handle) const;

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

  // As there are only a handful of cameras at any point in time, a map results
  // in drastically less overhead at the cost of marginally more expensive
  // lookups.
  core::Map<Entity, u32> entity_to_instance_;

  // Used for reverse lookups.
  core::Array<Entity> instance_to_entity_;

  core::Array<Camera> cameras_;

  core::Array<u32> siblings_;
};

} // yeti

#endif // _YETI_COMPONENTS_CAMERA_H_
