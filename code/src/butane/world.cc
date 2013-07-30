// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/world.h>

#include <butane/application.h>
#include <butane/graphics/render_device.h>
#include <butane/graphics/render_context.h>

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

  const butane::VisualRepresentation* World::VisualRepresentation::visual_representation_from_id(
    const butane::VisualRepresentation::Id id )
  {
    typedef butane::VisualRepresentation::Id Id;

    const size_t id_ = (size_t)(id & 0xFFFFFFFFull);
    switch (butane::VisualRepresentation::type(id)) {
      case butane::VisualRepresentation::CAMERA: {
        if (_camera_ids[id] == (size_t)0xFFFFFFFFFFFFFFFFull)
          return nullptr;
        return &_cameras[_camera_ids[id]];
      } break;

      case butane::VisualRepresentation::MESH: {
        if (_mesh_ids[id] == (size_t)0xFFFFFFFFFFFFFFFFull)
          return nullptr;
        return &_meshes[_mesh_ids[id]];
      } break;

      default:
        return nullptr;
    }
  }

  void World::VisualRepresentation::apply(
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
        if (_camera_ids[id] == (size_t)0xFFFFFFFFFFFFFFFFull) {
          _camera_ids[id] = _cameras.size();
          _cameras.resize(_cameras.size() + 1); }
        _cameras[_camera_ids[id]] =
          *((const SceneGraph::Node::Camera::VisualRepresentation*)visual_representation);
      } break;

      case butane::VisualRepresentation::MESH: {
        if (_mesh_ids[id] == (size_t)0xFFFFFFFFFFFFFFFFull) {
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
        if (_camera_ids[id] != (size_t)0xFFFFFFFFFFFFFFFFull) {
          _camera_ids[_cameras[_cameras.size() - 1].id] = _camera_ids[id];
          _cameras.swap(_camera_ids[id], _cameras.size() - 1);
          _cameras.resize(_cameras.size() - 1); }
        _camera_ids[id] = _next_avail_camera_id;
        _next_avail_camera_id = id;
      } break;

      case butane::VisualRepresentation::MESH: {
        if (_mesh_ids[id] != (size_t)0xFFFFFFFFFFFFFFFFull) {
          _mesh_ids[_meshes[_meshes.size() - 1].id] = _mesh_ids[id];
          _meshes.swap(_mesh_ids[id], _meshes.size() - 1);
          _meshes.resize(_meshes.size() - 1); }
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
          (butane::VisualRepresentation*)(((uintptr_t)request) + sizeof(VisualRepresentationStream::Requests::Update));
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
    _units[_unit_ids[id]] =
      make_new(Unit, Unit::allocator())(*this, id, type, position, rotation, scale);
    _spawning += id;
    return id;
  }

  void World::destroy_unit(
    const Unit::Id id )
  {
    _despawning += id;
  }

  Unit& World::unit(
    const Unit::Id id )
  {
    assert(id < _unit_ids.size());
    return *_units[_unit_ids[id]];
  }

  const Unit& World::unit(
    const Unit::Id id ) const
  {
    assert(id < _unit_ids.size());
    return *_units[_unit_ids[id]];
  }

  void World::update(
    const float dt )
  {
    const LogScope _("World::update");

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

    // TODO: Go wide. Use n update_scene_graph tasks. Introducing unit linking
    //       will introduce ordering problems, so linked units will have to be
    //       handled seperately.
    Task* update_scene_graphs_task; {
      Tasks::UpdateSceneGraphsData* usgd =
        (Tasks::UpdateSceneGraphsData*)alloca(sizeof(Tasks::UpdateSceneGraphsData));
      usgd->world = this;
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
    // TODO: Move this into World::render()?
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

  void World::render(
    const Unit::Reference& camera,
    const Viewport& viewport,
    const TiedResources* swap_chain_and_resources ) const
  {
    const LogScope _("World::render");

    Task* render_world_task; {
      Tasks::RenderWorldData* rwd =
        (Tasks::RenderWorldData*)Allocators::scratch().alloc(sizeof(Tasks::RenderWorldData));
      rwd->world = this;
      render_world_task = Task::prepare(
        Thread::default_affinity,
        &Tasks::render_world,
        (uintptr_t)rwd);
    }

    Task* apply_visual_representation_stream_task; {
      Tasks::ApplyVisualRepresentationStreamData* avrsd =
        (Tasks::ApplyVisualRepresentationStreamData*)Allocators::scratch().alloc(sizeof(Tasks::ApplyVisualRepresentationStreamData));
      avrsd->world = this;
      avrsd->vrs = _visual_representation_stream;
      apply_visual_representation_stream_task = render_world_task->child(
        Thread::default_affinity,
        &Tasks::apply_visual_representation_stream,
        (uintptr_t)avrsd);
    }

    typedef Array<butane::VisualRepresentation::Culled> Culled;
    Culled* culled = make_new(Culled, Allocators::scratch())(Allocators::heap());

    Task* frustum_cull_task; {
      Tasks::FrustumCullData* fcd =
        (Tasks::FrustumCullData*)Allocators::scratch().alloc(sizeof(Tasks::FrustumCullData));
      fcd->objects = _visual_representation._meshes.raw();
      fcd->num_of_objects = _visual_representation._meshes.size();
      zero((void*)&fcd->frustums[0], 1 /* 32 */ * sizeof(Mat4));
      fcd->culled = culled;
      frustum_cull_task = render_world_task->child(
        Thread::default_affinity,
        &Tasks::frustum_cull,
        (uintptr_t)fcd,
        apply_visual_representation_stream_task);
    }

    RenderContext* render_context = make_new(RenderContext, Allocators::scratch())();

    Task* generate_render_commands_task; {
      Tasks::GenerateRenderCommandsData* grcd =
        (Tasks::GenerateRenderCommandsData*)Allocators::scratch().alloc(sizeof(Tasks::GenerateRenderCommandsData));
      grcd->world = this;
      grcd->swap_chain_and_resources = swap_chain_and_resources;
      grcd->camera = _visual_representation.visual_representation_from_id(camera.to_node().visual_representation());
      grcd->viewport = viewport;
      grcd->render_context = render_context;
      grcd->culled = culled;
      generate_render_commands_task = render_world_task->child(
        Thread::default_affinity,
        &Tasks::generate_render_commands,
        (uintptr_t)grcd,
        frustum_cull_task);
    }

    Task* dispatch_task; {
      Tasks::DispatchData* dd =
        (Tasks::DispatchData*)Allocators::scratch().alloc(sizeof(Tasks::DispatchData));
      dd->num_of_render_contexts = 1;
      dd->render_contexts[0] = render_context;
      dispatch_task = render_world_task->child(
        RenderDevice::affinity(),
        &Tasks::dispatch,
        (uintptr_t)dd,
        generate_render_commands_task);
    }

    _visual_representation_stream = nullptr;

    dispatch_task->kick();
    generate_render_commands_task->kick();
    frustum_cull_task->kick();
    apply_visual_representation_stream_task->kick();
    render_world_task->kick();
  }

  void World::destroy()
  {
    make_delete(World, allocator(), this);
  }
} // butane
