// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  SceneGraph::Node::Empty::Empty()
  {
  }

  SceneGraph::Node::Empty::Empty(
    const Empty& empty )
  {
  }

  SceneGraph::Node::Empty& SceneGraph::Node::Empty::operator= (
    const Empty& empty )
  {
    return *this;
  }

  SceneGraph::Node::Empty::~Empty()
  {
  }
} // butane
