// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/unit.h>

#include <butane/world.h>

namespace butane {
  Unit::Reference::Reference()
    : _world(nullptr)
    , _unit(Unit::invalid)
    , _node(SceneGraph::Node::invalid)
  {}

  Unit::Reference::Reference(
    World& world,
    const Unit::Id id,
    const SceneGraph::Node::Id node
  ) : _world(&world)
    , _unit(id)
    , _node(node)
  {}

  Unit::Reference::Reference(
    const Unit& unit,
    const SceneGraph::Node::Id node
  ) : _world(&unit._world)
    , _unit(unit._id)
    , _node(node)
  {}

  Unit::Reference::Reference(
    const Reference& ref
  ) : _world(ref._world)
    , _unit(ref._unit)
    , _node(ref._node)
  {}

  Unit::Reference& Unit::Reference::operator= (
    const Reference& ref )
  {
    if (&ref == this)
      return *this;
    _world = ref._world;
    _unit = ref._unit;
    _node = ref._node;
    return *this;
  }

  Unit::Reference::~Reference()
  {}

  Unit& Unit::Reference::to_unit() const
  { return _world->unit(_unit); }

  SceneGraph::Node& Unit::Reference::to_node() const
  {
    Unit& unit = _world->unit(_unit);
    assert(_node < unit._scene_graph.num_of_nodes());
    return unit._scene_graph.nodes()[_node];
  }
} // butane

namespace butane {
  const Unit::Id Unit::invalid = (Id)0xFFFFFFFFFFFFFFFF;

  Unit::Unit()
    : _id(Unit::invalid)
    , _world(*((World*)nullptr))
  {
  }

  Unit::Unit(
    World& world,
    const Unit::Id id,
    const Resource::Handle<UnitResource> resource,
    const Vec3f& position,
    const Quatf& rotation,
    const Vec3f& scale
  ) : _world(world)
    , _id(id)
    , _type(resource)
    , _scene_graph(*(resource->_scene_graph))
  {
    _scene_graph.local_poses()[0].position = position;
    _scene_graph.local_poses()[0].rotation = rotation;
    _scene_graph.local_poses()[0].scale = scale;
  }

  Unit::~Unit()
  {
  }

  void Unit::update(
    const float dt )
  {
  }

  void Unit::create_visual_representation(
    VisualRepresentationStream& vrs ) const
  {
    _scene_graph.create_visual_representations(vrs);
  }

  void Unit::update_visual_representation(
    VisualRepresentationStream& vrs ) const
  {
    _scene_graph.update_visual_representations(vrs);
  }

  void Unit::destroy_visual_representation(
    VisualRepresentationStream& vrs ) const
  {
    _scene_graph.destroy_visual_representations(vrs);
  }
} // butane
