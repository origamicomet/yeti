// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  const SceneGraph::Node::Id SceneGraph::Node::invalid = (Id)0xFFFFFFFFFFFFFFFFull;
  const SceneGraph::Node::Id SceneGraph::Node::unlinked = invalid;

  SceneGraph::Node::Node()
    : _id(Node::invalid)
    , _type(Node::EMPTY)
  {
  }

  SceneGraph::Node::Node(
    const Serialized& serialized
  ) : _id((Id)serialized.id)
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

  SceneGraph::Node::Node(
    const Node& node
  ) : _id(node._id)
    , _type(node._type)
  {
    switch (node._type) {
      case Node::EMPTY:
      case Node::CAMERA:
        copy((void*)&_as, (const void*)&node._as, sizeof(Node::Data));
        break;
      case Node::MESH:
        new ((void*)&_as) Node::Mesh(*node.mesh());
        break;
      default:
        __builtin_unreachable();
    }
  }

  SceneGraph::Node& SceneGraph::Node::operator= (
    const Node& node )
  {
    if (&node == this)
      return *this;

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

    _id = node._id;
    _type = node._type;

    switch (node._type) {
      case Node::EMPTY:
      case Node::CAMERA:
        copy((void*)&_as, (const void*)&node._as, sizeof(Node::Data));
        break;
      case Node::MESH:
        new ((void*)&_as) Node::Mesh(*node.mesh());
        break;
      default:
        __builtin_unreachable();
    }

    return *this;
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
} // butane
