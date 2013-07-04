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
} // butane

namespace butane {
  World::VisualRepresentation::VisualRepresentation(
    const World& world
  ) : _world(world)
    , _camera_ids(allocator())
    , _cameras(allocator())
    , _mesh_ids(allocator())
    , _meshes(allocator())
  {
  }

  World::VisualRepresentation::~VisualRepresentation()
  {
  }

  void World::VisualRepresentation::update(
    const VisualRepresentationStream& vrs )
  {
    const Mutex::ScopedLock lock(_mutex);
    vrs.for_each_request(
      &World::VisualRepresentation::__requests_for_each,
      (void*)this);
  }

  butane::VisualRepresentation::Id World::VisualRepresentation::create(
    const VisualRepresentationStream::Requests::Create* request )
  {
    return butane::VisualRepresentation::invalid;
  }

  void World::VisualRepresentation::update(
    const VisualRepresentationStream::Requests::Update* request )
  {
  }

  void World::VisualRepresentation::destroy(
    const VisualRepresentationStream::Requests::Destroy* request )
  {
  }

  void World::VisualRepresentation::__requests_for_each(
    void* closure,
    const VisualRepresentationStream::Request* request )
  {
    World::VisualRepresentation* vr =
      (World::VisualRepresentation*)closure;

    switch (request->type) {
      case VisualRepresentationStream::Request::CREATE: {
        VisualRepresentationStream::Requests::Create* request_ =
          (VisualRepresentationStream::Requests::Create*)request;
        *(request_->vrid) = vr->create(request_);
      } break;

      case VisualRepresentationStream::Request::UPDATE: {
        VisualRepresentationStream::Requests::Update* request_ =
          (VisualRepresentationStream::Requests::Update*)request;
        vr->update(request_);
      } break;

      case VisualRepresentationStream::Request::DESTROY: {
        VisualRepresentationStream::Requests::Destroy* request_ =
          (VisualRepresentationStream::Requests::Destroy*)request;
        vr->destroy(request_);
      } break;

      default:
        __builtin_unreachable();
    }
  }
} // butane

namespace butane {
  World::World()
    : _visual_representation(*this)
    , _spawning(allocator())
    , _despawning(allocator())
    , _next_avail_unit_id(Unit::invalid)
    , _unit_ids(allocator())
    , _units(allocator())
  {
  }

  World::~World()
  {
  }

  World* World::create()
  {
    return make_new(World, allocator())();
  }

  Unit::Id World::spawn_unit(
    const Resource::Handle<UnitResource>& type,
    const Vec3f& position,
    const Quatf& rotation,
    const Vec3f& scale )
  {
    Unit::Id id; {
      if (_next_avail_unit_id == Unit::invalid) {
        id = _unit_ids.size();
        _unit_ids.reserve(1);
      } else {
        id = _next_avail_unit_id;
        _next_avail_unit_id = _unit_ids[_next_avail_unit_id];
      }
    }

    _unit_ids[id] = _units.size();
    _units.resize(_units.size() + 1);
    new ((void*)&_units[_unit_ids[id]]) Unit(*this, id, type, position, rotation, scale);
    _spawning += id;
    return id;
  }

  void World::destroy_unit(
    const Unit::Id id )
  {
    _despawning += id;
  }

  void World::update(
    const float dt )
  {
    Task* update_world_task; {
      Tasks::UpdateWorldData* uwd =
        (Tasks::UpdateWorldData*)alloca(sizeof(Tasks::UpdateWorldData));
      uwd->world = this;
      uwd->dt = dt;
      update_world_task = Task::prepare(
        Thread::default_affinity,
        &Tasks::update_world,
        (uintptr_t)uwd);
    }

    Task* update_units_task; {
      Tasks::UpdateUnitsData* uud =
        (Tasks::UpdateUnitsData*)alloca(sizeof(Tasks::UpdateUnitsData));
      uud->world = this;
      uud->dt = dt;
      update_units_task = update_world_task->child(
        Thread::default_affinity,
        &Tasks::update_units,
        (uintptr_t)uud);
    }

    Task* update_scene_graphs_task; {
      Tasks::UpdateSceneGraphsData* usgd =
        (Tasks::UpdateSceneGraphsData*)alloca(sizeof(Tasks::UpdateSceneGraphsData));
      usgd->scene_graphs = (num_of_units() > 0) ? &_units[0].scene_graph() : nullptr;
      usgd->num_of_scene_graphs = num_of_units();
      usgd->stride = sizeof(Unit);
      update_scene_graphs_task = update_world_task->child(
        Thread::default_affinity,
        &Tasks::update_scene_graphs,
        (uintptr_t)usgd,
        update_units_task);
    }

    if (!_visual_representation_stream) {
      _visual_representation_stream =
        make_new(VisualRepresentationStream, Allocators::scratch())();
    }

    // TODO: Go wide. Use n VisualRepresentationStreams and then apply all with
    //       one task.
    Task* update_visual_representations_task; {
      Tasks::UpdateVisualRepresentationsData* uvrd =
        (Tasks::UpdateVisualRepresentationsData*)alloca(sizeof(Tasks::UpdateVisualRepresentationsData));
      uvrd->world = this;
      update_visual_representations_task = update_world_task->child(
        Thread::default_affinity,
        &Tasks::update_visual_representations,
        (uintptr_t)uvrd,
        update_scene_graphs_task);
    }

    Task* graduate_units_task; {
      Tasks::GraduateUnitsData* gud =
        (Tasks::GraduateUnitsData*)alloca(sizeof(Tasks::GraduateUnitsData));
      gud->world = this;
      graduate_units_task = update_world_task->child(
        Thread::default_affinity,
        &Tasks::graduate_units,
        (uintptr_t)gud,
        update_visual_representations_task);
    }

    graduate_units_task->kick();
    update_visual_representations_task->kick();
    update_scene_graphs_task->kick();
    update_units_task->kick();
    update_world_task->kick_and_wait();
  }

  void World::destroy()
  {
    make_delete(World, allocator(), this);
  }
} // butane
