// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph.h>

namespace butane {
  static Allocator* __allocator_initializer() {
    return new ProxyAllocator("scene graphs", Allocators::heap());
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
    , _visual_representations(allocator())
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
    , _visual_representations(allocator(), serialized.num_of_nodes)
  {
    _types.resize(serialized.num_of_nodes);
    _links.resize(serialized.num_of_nodes);
    _names.resize(serialized.num_of_nodes);
    _flags.resize(serialized.num_of_nodes);
    _local_poses.resize(serialized.num_of_nodes);
    _world_transforms.resize(serialized.num_of_nodes);
    _nodes.resize(serialized.num_of_nodes);
    _visual_representations.resize(serialized.num_of_nodes);

    for (uint node = 0; node < serialized.num_of_nodes; ++node) {
      _types[node] = (Node::Type)serialized.nodes[node].type;
      _links[node] = (Node::Link)serialized.nodes[node].link;
      _names[node] = (Node::Name)serialized.nodes[node].name;
      _flags[node] = Node::MOVED;
      _local_poses[node] = serialized.nodes[node].pose;
      _world_transforms[node] = Mat4::identity();
      new ((void*)&_nodes[node]) Node(*this, serialized.nodes[node]);
      _visual_representations[node] = VisualRepresentation::invalid;
    }
  }

  SceneGraph::~SceneGraph()
  {
  }

  void SceneGraph::update()
  {
    for (uint node = 0; node < num_of_nodes(); ++node) {
      if (!(_flags[node] & Node::MOVED))
        continue;
      const Mat4 parent = (_links[node] == Node::unlinked) ?
        Mat4::identity() : _world_transforms[_links[node]];
      const Mat4 transform = Mat4::translate_rotate_scale(
        _local_poses[node].position,
        _local_poses[node].rotation,
        _local_poses[node].scale);
      _world_transforms[node] = parent * transform;
      _flags[node] &= ~Node::MOVED;
      _flags[node] |= (_flags[node] & Node::HAS_VISUAL_REPRESENTATION) ? Node::DIRTY : 0;
    }
  }

  void SceneGraph::create_visual_representations(
    VisualRepresentationStream& vrs ) const
  {
    for (size_t node = 0; node < num_of_nodes(); ++node) {
      switch (_types[node]) {
        case Node::EMPTY:
          break;
        case Node::CAMERA:
          vrs.create(
            VisualRepresentation::CAMERA,
            &_visual_representations[node]);
          _flags[node] |= Node::DIRTY;
          _flags[node] |= Node::HAS_VISUAL_REPRESENTATION; break;
        case Node::MESH:
          vrs.create(
            VisualRepresentation::MESH,
            &_visual_representations[node]);
          _flags[node] |= Node::DIRTY;
          _flags[node] |= Node::HAS_VISUAL_REPRESENTATION; break;
        default:
          __builtin_unreachable();
      }
    }
  }

  void SceneGraph::update_visual_representations(
    VisualRepresentationStream& vrs ) const
  {
    for (size_t node = 0; node < num_of_nodes(); ++node) {
      if (!(_flags[node] & Node::DIRTY))
        continue;
      Node::VisualRepresentation* vr =
        (Node::VisualRepresentation*)vrs.update(_visual_representations[node]);
      if (vr) {
        vr->id = _visual_representations[node];
        vr->transform = _world_transforms[node];
        // vr->bounding_box = _bounding_boxs[node];
        _nodes[node].update_visual_representation(*vr);
        _flags[node] &= ~Node::DIRTY;
      }
    }
  }

  void SceneGraph::destroy_visual_representations(
    VisualRepresentationStream& vrs ) const
  {
    for (size_t node = 0; node < num_of_nodes(); ++node)
      vrs.destroy(_visual_representations[node]);
  }
} // butane
