// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("scene graphs", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  SceneGraph::SceneGraph()
    : _types(allocator())
    , _links(allocator())
    , _names(allocator())
    , _flags(allocator())
    , _local_poses(allocator())
    , _world_transforms(allocator())
    , _nodes(allocator())
  {
  }

  SceneGraph::SceneGraph(
    const Serialized& serialized
  ) : _types(allocator(), serialized.num_of_nodes)
    , _links(allocator(), serialized.num_of_nodes)
    , _names(allocator(), serialized.num_of_nodes)
    , _flags(allocator(), serialized.num_of_nodes)
    , _local_poses(allocator(), serialized.num_of_nodes)
    , _world_transforms(allocator(), serialized.num_of_nodes)
    , _nodes(allocator(), serialized.num_of_nodes)
  {
    _types.resize(serialized.num_of_nodes);
    _links.resize(serialized.num_of_nodes);
    _names.resize(serialized.num_of_nodes);
    _flags.resize(serialized.num_of_nodes);
    _local_poses.resize(serialized.num_of_nodes);
    _world_transforms.resize(serialized.num_of_nodes);
    _nodes.resize(serialized.num_of_nodes);

    for (uint node = 0; node < serialized.num_of_nodes; ++node) {
      _types[node] = (Node::Type)serialized.nodes[node].type;
      _links[node] = (Node::Link)serialized.nodes[node].link;
      _names[node] = (Node::Name)serialized.nodes[node].name;
      _flags[node] = Node::DIRTY;
      _local_poses[node] = serialized.nodes[node].pose;
      _world_transforms[node] = Mat4::identity();
      new ((void*)&_nodes[node]) Node(serialized.nodes[node]);
    }
  }

  SceneGraph::SceneGraph(
    const SceneGraph& sg
  ) : _types(sg._types)
    , _links(sg._links)
    , _names(sg._names)
    , _flags(sg._flags)
    , _local_poses(sg._local_poses)
    , _world_transforms(sg._world_transforms)
    , _nodes(sg._nodes)
  {
  }

  SceneGraph& SceneGraph::operator= (
    const SceneGraph& sg )
  {
    if (&sg == this)
      return *this;

    _types = sg._types;
    _links = sg._links;
    _names = sg._names;
    _flags = sg._flags;
    _local_poses = sg._local_poses;
    _world_transforms = sg._world_transforms;
    _nodes = sg._nodes;

    return *this;
  }

  SceneGraph::~SceneGraph()
  {
  }

  void SceneGraph::update()
  {
    for (uint node = 0; node < num_of_nodes(); ++node) {
      if (!(_flags[node] & Node::DIRTY))
        continue;
      const Mat4 parent = (_links[node] == Node::unlinked) ?
        Mat4::identity() : _world_transforms[_links[node]];
      const Mat4 transform = Mat4::translate_rotate_scale(
        _local_poses[node].position,
        _local_poses[node].rotation,
        _local_poses[node].scale);
      _world_transforms[node] = parent * transform;
      _flags[node] &= ~Node::DIRTY;
    }
  }
} // butane
