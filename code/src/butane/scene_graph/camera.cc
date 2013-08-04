// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  Mat4 SceneGraph::Node::Camera::VisualRepresentation::projection(
    const uint32_t width,
    const uint32_t height ) const
  {
    switch (type) {
      case Camera::ORTHOGRAPHIC:
        return Mat4::orthographic(
          ((float)width) * ((as.orthographic.min_x + 1.0f) * 0.5f),
          ((float)width) * ((as.orthographic.max_x + 1.0f) * 0.5f),
          ((float)height) * ((as.orthographic.min_y + 1.0f) * 0.5f),
          ((float)height) * ((as.orthographic.max_y + 1.0f) * 0.5f),
          near, far);
      case Camera::PERSPECTIVE:
        return Mat4::perspective(
          as.perspective.field_of_view,
          ((float)width) / ((float)height),
          near, far);
      default:
        __builtin_unreachable();
    }
  }

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
        _as.orthographic.max_y = serialized.orthographic.max_y;
      } break;

      case Camera::PERSPECTIVE: {
        _as.perspective.field_of_view = serialized.perspective.field_of_view;
      } break;
    }
  }

  SceneGraph::Node::Camera::~Camera()
  {
  }

  void SceneGraph::Node::Camera::update_visual_representation(
    VisualRepresentation& vr ) const
  {
    vr.type = _type;
    vr.near = _near;
    vr.far  = _far;
    switch (_type) {
      case Camera::ORTHOGRAPHIC:
        vr.as.orthographic.min_x = _as.orthographic.min_x;
        vr.as.orthographic.max_x = _as.orthographic.max_x;
        vr.as.orthographic.min_y = _as.orthographic.min_y;
        vr.as.orthographic.max_y = _as.orthographic.max_y;
        break;
      case Camera::PERSPECTIVE:
        vr.as.perspective.field_of_view = _as.perspective.field_of_view;
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
} // butane
