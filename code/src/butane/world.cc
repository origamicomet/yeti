// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/world.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("worlds", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  World::World()
    : _unit_ids(allocator())
    , _units(allocator())
    , _next_available_unit_id(Unit::invalid)
  {
  }

  World::~World()
  {
  }

  World* World::create()
  {
    return make_new(World, allocator())();
  }

  Unit& World::spawn_unit(
    const Resource::Handle<UnitResource>& type,
    const Vec3f& position,
    const Quatf& rotation,
    const Vec3f& scale )
  {
    Unit::Id id; {
      if (_next_available_unit_id == Unit::invalid) {
        id = _unit_ids.size();
        _unit_ids.reserve(max((size_t)1, _unit_ids.size()));
      } else {
        id = _next_available_unit_id;
        _next_available_unit_id = _unit_ids[_next_available_unit_id];
      }
    }

    _unit_ids[id] = _units.size();

    if (_units.size() == _units.reserved())
      _units.reserve(max((size_t)1, _units.reserved()));

    new ((void*)&_units[_unit_ids[id]]) Unit(*this, type, position, rotation, scale);

    return _units[_unit_ids[id]];
  }

  void World::destroy_unit(
    const Unit::Id id )
  {
    if (id >= _unit_ids.size())
      fail("Invalid unit id %u specified.", id);

    _units[_unit_ids[id]].~Unit();

    if (_unit_ids[id] < (_units.size() - 1)) {
      copy(
        (void*)&_units[_unit_ids[id]],
        (const void*)&_units[_units.size() - 1],
        sizeof(Unit));
      _unit_ids[_units[_units.size() - 1].id()] = _unit_ids[id]; }

    _units.resize(_units.size() - 1);

    _unit_ids[id] = _next_available_unit_id;
    _next_available_unit_id = id;
  }

  void World::update(
    const float dt )
  {
  }

  void World::destory()
  {
    make_delete(World, allocator(), this);
  }
} // butane
