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

  SceneGraph::Node::Mesh::~Mesh()
  {
  }

  void SceneGraph::Node::Mesh::update_visual_representation(
    VisualRepresentation& vr ) const
  {
    vr.resource = _resource;
    vr.material = _material;
  }

  bool SceneGraph::Node::Mesh::set_material(
    const MeshResource::Material::Name& name )
  {
    for (size_t material = 0; material < _resource->materials().size(); ++material) {
      if (name != _resource->materials()[material].name)
        continue;
      _material = material;
      node().scene_graph().flags()[node().id()] &= Node::DIRTY;
      return true; }
    return false;
  }
} // butane
