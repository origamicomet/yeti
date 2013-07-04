// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  const SceneGraph::Node::Id SceneGraph::Node::invalid = (Id)0xFFFFFFFFFFFFFFFFull;
  const SceneGraph::Node::Id SceneGraph::Node::unlinked = invalid;

  SceneGraph::Node::Node()
    : _scene_graph(nullptr)
    , _id(Node::invalid)
    , _type(Node::EMPTY)
  {
  }

  SceneGraph::Node::Node(
    SceneGraph& sg,
    const Serialized& serialized
  ) : _scene_graph(&sg)
    , _id((Id)serialized.id)
    , _type(serialized.type)
  {
    switch (serialized.type) {
      case Node::EMPTY:
        break;
      case Node::CAMERA:
        new ((void*)&_as) Node::Camera(*((Node::Camera::Serialized*)&serialized.as));
        break;
      case Node::MESH:
        new ((void*)&_as) Node::Mesh(*((Node::Mesh::Serialized*)&serialized.as));
        break;
      default:
        __builtin_unreachable();
    }
  }

  SceneGraph::Node::~Node()
  {
    switch (_type) {
      case Node::EMPTY:
        empty()->~Empty(); break;
      case Node::CAMERA:
        camera()->~Camera(); break;
      case Node::MESH:
        mesh()->~Mesh(); break;
      default:
        __builtin_unreachable();
    }
  }

  void SceneGraph::Node::update_visual_representation(
    VisualRepresentation& vr ) const
  {
    switch (_type) {
      case Node::EMPTY:
        break;
      case Node::CAMERA:
        camera()->update_visual_representation(
          *((Camera::VisualRepresentation*)&vr));
        break;
      case Node::MESH:
        mesh()->update_visual_representation(
          *((Mesh::VisualRepresentation*)&vr));
        break;
      default:
        __builtin_unreachable();
    }
  }
} // butane
