// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  SceneGraph::Node::Camera::Camera(
    const Serialized& serialized )
  {
    _type = serialized.type;

    switch (serialized.type) {
      case Camera::ORTHOGRAPHIC:
        _projection = Mat4::orthographic(
          serialized.orthographic.min_x, serialized.orthographic.max_x,
          serialized.orthographic.min_x, serialized.orthographic.max_y,
          serialized.near, serialized.far);
        break;

      case Camera::PERSPECTIVE:
        _projection = Mat4::perspective(
          serialized.perspective.field_of_view,
          serialized.perspective.aspect_ratio,
          serialized.near, serialized.far);
        break;

      default:
        __builtin_unreachable();
    }
  }

  SceneGraph::Node::Camera::Camera(
    const Camera& camera
  ) : _type(camera._type)
    , _projection(camera._projection)
  {
  }

  SceneGraph::Node::Camera& SceneGraph::Node::Camera::operator= (
    const Camera& camera )
  {
    if (&camera == this)
      return *this;

    _type = camera._type;
    _projection = camera._projection;

    return *this;
  }

  SceneGraph::Node::Camera::~Camera()
  {
  }
} // butane
