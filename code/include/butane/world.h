// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_WORLD_H_
#define _BUTANE_WORLD_H_

#include <butane/butane.h>
#include <butane/unit.h>
#include <butane/visual_representation.h>
#include <butane/visual_representation_stream.h>
#include <butane/tasks/update_world.h>
#include <butane/tasks/update_units.h>
#include <butane/tasks/update_scene_graphs.h>
#include <butane/tasks/update_visual_representations.h>
#include <butane/tasks/graduate_units.h>
#include <butane/tasks/render_world.h>
#include <butane/tasks/apply_visual_representation_stream.h>

namespace butane {
  class BUTANE_EXPORT World final {
    __foundation_trait(World, non_copyable);

    private:
      friend void Tasks::update_world( Task*, uintptr_t );
      friend void Tasks::update_units( Task*, uintptr_t );
      friend void Tasks::update_scene_graphs( Task*, uintptr_t );
      friend void Tasks::update_visual_representations( Task*, uintptr_t );
      friend void Tasks::graduate_units( Task*, uintptr_t );
      friend void Tasks::render_world( Task*, uintptr_t );
      friend void Tasks::apply_visual_representation_stream( Task*, uintptr_t );

    public:
      class VisualRepresentation final
      {
        __foundation_trait(VisualRepresentation, non_copyable);

        private:
          friend class World;

        private:
          friend void Tasks::render_world( Task*, uintptr_t );
          friend void Tasks::apply_visual_representation_stream( Task*, uintptr_t );

        private:
          VisualRepresentation(
            const World& world );

          ~VisualRepresentation();

        private:
          void apply(
            const VisualRepresentationStream& vrs );

        private:
          butane::VisualRepresentation::Id create(
            const VisualRepresentationStream::Requests::Create* request );

          void update(
            const VisualRepresentationStream::Requests::Update* request,
            const butane::VisualRepresentation* visual_representation );

          void destroy(
            const VisualRepresentationStream::Requests::Destroy* request );

        private:
          static void __requests_for_each(
            void* closure,
            const VisualRepresentationStream::Request* request );

        private:
          Mutex _mutex;
          const World& _world;
          butane::VisualRepresentation::Id _next_avail_camera_id;
          Array<size_t> _camera_ids;
          Array<SceneGraph::Node::Camera::VisualRepresentation> _cameras;
          butane::VisualRepresentation::Id _next_avail_mesh_id;
          Array<size_t> _mesh_ids;
          Array<SceneGraph::Node::Mesh::VisualRepresentation> _meshes;
      };

    private:
      World();

      ~World();

    public:
      /*! */
      static World* create();

      /*! */
      Unit::Id spawn_unit(
        const Resource::Handle<UnitResource>& type,
        const Vec3f& position = Vec3f(0.0f, 0.0f, 0.0f),
        const Quatf& rotation = Quatf(1.0f, 0.0f, 0.0f, 0.0f),
        const Vec3f& scale = Vec3f(1.0f, 1.0f, 1.0f) );

      /*! */
      void destroy_unit(
        const Unit::Id id );

      /*! */
      Unit& unit(
        const Unit::Id id );

      /*! */
      const Unit& unit(
        const Unit::Id id ) const;

      /*! */
      void update(
        const float dt );

      /*! */
      void render(
        const Unit::Reference& camera ) const;

    public:
      void destroy();

    public:
      FOUNDATION_INLINE const VisualRepresentation& visual_representation() const
      { return _visual_representation; }

      FOUNDATION_INLINE size_t num_of_units() const
      { return _units.size(); }

      FOUNDATION_INLINE Unit* const * units() const
      { return _units.raw(); }

    private:
      mutable VisualRepresentation _visual_representation;
      mutable VisualRepresentationStream* _visual_representation_stream;
      Array<Unit::Id> _spawning;
      Array<Unit::Id> _despawning;
      Unit::Id _next_avail_unit_id;
      Array<size_t> _unit_ids;
      Array<Unit*> _units;
  };
} // butane

#endif // _BUTANE_WORLD_H_
