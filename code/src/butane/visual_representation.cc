// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/visual_representation.h>

#include <butane/scene_graph.h>

namespace butane {
  VisualRepresentation::VisualRepresentation()
    : id(VisualRepresentation::invalid)
  {
  }

  VisualRepresentation::~VisualRepresentation()
  {
  }

  VisualRepresentation::Type VisualRepresentation::type(
    const VisualRepresentation::Id id )
  {
    return (Type)(id >> 32u);
  }

  size_t VisualRepresentation::length(
    const VisualRepresentation::Type type )
  {
    switch (type) {
      case VisualRepresentation::CAMERA:
        return sizeof(SceneGraph::Node::Camera::VisualRepresentation);
      case VisualRepresentation::MESH:
        return sizeof(SceneGraph::Node::Mesh::VisualRepresentation);
    }

    return 0;
  }
} // butane
