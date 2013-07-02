// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  SceneGraph::Node::Mesh::Mesh(
    const Serialized& serialized
  ) : _resource(serialized.resource)
    , _material(serialized.material)
  {
  }

  SceneGraph::Node::Mesh::Mesh(
    const Mesh& mesh
  ) : _resource(mesh._resource)
    , _material(mesh._material)
  {
  }

  SceneGraph::Node::Mesh& SceneGraph::Node::Mesh::operator= (
    const Mesh& mesh )
  {
    if (&mesh == this)
      return *this;

    _resource = mesh._resource;
    _material = mesh._material;

    return *this;
  }

  SceneGraph::Node::Mesh::~Mesh()
  {
  }
} // butane
