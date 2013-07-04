// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/unit.h>

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
