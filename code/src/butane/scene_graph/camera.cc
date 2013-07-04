// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  SceneGraph::Node::Camera::Camera(
    const Serialized& serialized )
  {
    _type = serialized.type;
    _near = serialized.near;
    _far = serialized.far;

    switch (serialized.type) {
      case Camera::ORTHOGRAPHIC: {
        _as.orthographic.min_x = serialized.orthographic.min_x;
        _as.orthographic.max_x = serialized.orthographic.max_x;
        _as.orthographic.min_y = serialized.orthographic.min_y;
        _as.orthographic.min_y = serialized.orthographic.max_y;
      } break;

      case Camera::PERSPECTIVE: {
        _as.perspective.field_of_view = serialized.perspective.field_of_view;
        _as.perspective.aspect_ratio = serialized.perspective.aspect_ratio;
      } break;
    }
  }

  SceneGraph::Node::Camera::~Camera()
  {
  }

  void SceneGraph::Node::Camera::update_visual_representation(
    VisualRepresentation& vr ) const
  {
    switch (_type) {
      case Camera::ORTHOGRAPHIC:
        vr.projection = Mat4::orthographic(
          _as.orthographic.min_x, _as.orthographic.max_x,
          _as.orthographic.min_x, _as.orthographic.max_y,
          _near, _far);
        break;

      case Camera::PERSPECTIVE:
        vr.projection = Mat4::perspective(
          _as.perspective.field_of_view,
          _as.perspective.aspect_ratio,
          _near, _far);
        break;

      default:
        __builtin_unreachable();
    }
  }

  void SceneGraph::Node::Camera::set_type(
    const Camera::Type type )
  {
    node().scene_graph().flags()[node().id()] &= Node::DIRTY;
    _type = type;
  }

  void SceneGraph::Node::Camera::set_near(
    const float near )
  {
    node().scene_graph().flags()[node().id()] &= Node::DIRTY;
    _near = near;
  }

  void SceneGraph::Node::Camera::set_far(
    const float far )
  {
    node().scene_graph().flags()[node().id()] &= Node::DIRTY;
    _far = far;
  }

  void SceneGraph::Node::Camera::set_view(
    const Vec2f& min,
    const Vec2f& max )
  {
    node().scene_graph().flags()[node().id()] &= Node::DIRTY;
    _as.orthographic.min_x = min.x;
    _as.orthographic.min_y = min.y;
    _as.orthographic.max_x = max.x;
    _as.orthographic.max_y = max.y;
  }

  void SceneGraph::Node::Camera::set_field_of_view(
    const float field_of_view )
  {
    node().scene_graph().flags()[node().id()] &= Node::DIRTY;
    _as.perspective.field_of_view = field_of_view;
  }

  void SceneGraph::Node::Camera::set_aspect_ratio(
    const float aspect_ratio )
  {
    node().scene_graph().flags()[node().id()] &= Node::DIRTY;
    _as.perspective.aspect_ratio = aspect_ratio;
  }

} // butane
