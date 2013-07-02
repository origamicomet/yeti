// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCENE_GRAPH_H_
#define _BUTANE_SCENE_GRAPH_H_

#include <butane/butane.h>
#include <butane/math.h>
#include <butane/resources/mesh.h>

namespace butane {
  class BUTANE_EXPORT SceneGraph final {
    public:
      #include <butane/scene_graph/node.h>

    public:
      /*! */
      static const uint maximum_num_of_nodes = 254;

    public:
      struct Serialized final {
        uint8_t num_of_nodes;
        relative_ptr<Node::Serialized*> nodes;
      };

    public:
      SceneGraph();

      SceneGraph(
        const Serialized& serialized );

      SceneGraph(
        const SceneGraph& sg );

      SceneGraph& operator= (
        const SceneGraph& sg );

      ~SceneGraph();

    public:
      void update();

    public:
      FOUNDATION_INLINE size_t num_of_nodes() const
      { return _nodes.size(); }

      FOUNDATION_INLINE const Node::Type* types() const
      { return _types.raw(); }

      FOUNDATION_INLINE const Node::Link* links() const
      { return _links.raw(); }

      FOUNDATION_INLINE const Node::Name* names() const
      { return _names.raw(); }

      FOUNDATION_INLINE uint32_t* flags()
      { return _flags.raw(); }

      FOUNDATION_INLINE const uint32_t* flags() const
      { return _flags.raw(); }

      FOUNDATION_INLINE Node::Pose* local_poses()
      { return _local_poses.raw(); }

      FOUNDATION_INLINE const Node::Pose* local_poses() const
      { return _local_poses.raw(); }

      FOUNDATION_INLINE Mat4* world_transforms()
      { return _world_transforms.raw(); }

      FOUNDATION_INLINE const Mat4* world_transforms() const
      { return _world_transforms.raw(); }

      FOUNDATION_INLINE const Node* nodes() const
      { return _nodes.raw(); }

    private:
      Array<Node::Type> _types;
      Array<Node::Link> _links;
      Array<Node::Name> _names;
      Array<uint32_t> _flags;
      Array<Node::Pose> _local_poses;
      Array<Mat4> _world_transforms;
      Array<Node> _nodes;
  };
} // butane

#endif // _BUTANE_SCENE_GRAPH_H_
