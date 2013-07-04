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
    , _next_avail_camera_id(butane::VisualRepresentation::invalid)
    , _cameras(allocator())
    , _next_avail_mesh_id(butane::VisualRepresentation::invalid)
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
    typedef butane::VisualRepresentation::Id Id;

    switch (request->vrt) {
      case butane::VisualRepresentation::CAMERA: {
        Id id; {
          if (_next_avail_camera_id == butane::VisualRepresentation::invalid) {
            id = _camera_ids.size();
            _camera_ids.reserve(1);
          } else {
            id = _next_avail_camera_id;
            _next_avail_camera_id = _camera_ids[_next_avail_camera_id];
          }
        }

        // The first update request effectivly "creates" the visual representation.
        _camera_ids[id] = (size_t)0xFFFFFFFFFFFFFFFFull;
        return (((Id)id) | ((Id)butane::VisualRepresentation::CAMERA << 32ull));
      } break;

      case butane::VisualRepresentation::MESH: {
        Id id; {
          if (_next_avail_mesh_id == butane::VisualRepresentation::invalid) {
            id = _mesh_ids.size();
            _mesh_ids.reserve(1);
          } else {
            id = _next_avail_mesh_id;
            _next_avail_mesh_id = _mesh_ids[_next_avail_mesh_id];
          }
        }

        // The first update request effectivly "creates" the visual representation.
        _mesh_ids[id] = (size_t)0xFFFFFFFFFFFFFFFFull;
        return (((Id)id) | ((Id)butane::VisualRepresentation::MESH << 32ull));
      } break;
    }

    __builtin_unreachable();
  }

  void World::VisualRepresentation::update(
    const VisualRepresentationStream::Requests::Update* request,
    const butane::VisualRepresentation* visual_representation )
  {
    typedef butane::VisualRepresentation::Id Id;

    const size_t id = (size_t)(request->vrid & 0xFFFFFFFFull);
    switch (butane::VisualRepresentation::type(request->vrid)) {
      case butane::VisualRepresentation::CAMERA: {
        if (_camera_ids[id] == (Id)0xFFFFFFFFFFFFFFFFull) {
          _camera_ids[id] = _cameras.size();
          _cameras.resize(_cameras.size() + 1); }
        _cameras[_camera_ids[id]] =
          *((const SceneGraph::Node::Camera::VisualRepresentation*)visual_representation);
      } break;

      case butane::VisualRepresentation::MESH: {
        if (_mesh_ids[id] == (Id)0xFFFFFFFFFFFFFFFFull) {
          _mesh_ids[id] = _meshes.size();
          _meshes.resize(_meshes.size() + 1); }
        _meshes[_mesh_ids[id]] =
          *((const SceneGraph::Node::Mesh::VisualRepresentation*)visual_representation);
      } break;

      default:
        __builtin_unreachable();
    }
  }

  void World::VisualRepresentation::destroy(
    const VisualRepresentationStream::Requests::Destroy* request )
  {
    typedef butane::VisualRepresentation::Id Id;

    const size_t id = (size_t)(request->vrid & 0xFFFFFFFFull);
    switch (butane::VisualRepresentation::type(request->vrid)) {
      case butane::VisualRepresentation::CAMERA: {
        if (_camera_ids[id] != (Id)0xFFFFFFFFFFFFFFFFull) {
          _camera_ids[_cameras[_cameras.size() - 1].id] = _camera_ids[id];
          _cameras.swap(_camera_ids[id], _cameras.size() - 1); }
        _camera_ids[id] = _next_avail_camera_id;
        _next_avail_camera_id = id;
      } break;

      case butane::VisualRepresentation::MESH: {
        if (_mesh_ids[id] != (Id)0xFFFFFFFFFFFFFFFFull) {
          _mesh_ids[_meshes[_meshes.size() - 1].id] = _mesh_ids[id];
          _meshes.swap(_mesh_ids[id], _meshes.size() - 1); }
        _mesh_ids[id] = _next_avail_mesh_id;
        _next_avail_mesh_id = id;
      } break;

      default:
        __builtin_unreachable();
    }
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
        butane::VisualRepresentation* visual_representation =
          (butane::VisualRepresentation*)(request_ + sizeof(VisualRepresentationStream::Requests::Update));
        vr->update(request_, visual_representation);
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
    , _visual_representation_stream(nullptr)
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
