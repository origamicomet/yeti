// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_UNIT_H_
#define _BUTANE_UNIT_H_

#include <butane/butane.h>
#include <butane/math.h>
#include <butane/scene_graph.h>
#include <butane/resources/unit.h>

namespace butane {
  class World;
  class VisualRepresentationStream;
  class BUTANE_EXPORT Unit final {
    __foundation_trait(Unit, non_copyable);

    public:
      /*! */
      typedef uint32_t Id;

      /*! */
      static const Id invalid;

    public:
      Unit();

      Unit(
        World& world,
        const Unit::Id id,
        const Resource::Handle<UnitResource> resource,
        const Vec3f& position = Vec3f(0.0f, 0.0f, 0.0f),
        const Quatf& rotation = Quatf(1.0f, 0.0f, 0.0f, 0.0f),
        const Vec3f& scale = Vec3f(1.0f, 1.0f, 1.0f) );

      ~Unit();

    public:
      /*! */
      void update(
        const float dt );

      /*! */
      void create_visual_representation(
        VisualRepresentationStream& vrs ) const;

      /*! */
      void update_visual_representation(
        VisualRepresentationStream& vrs ) const;

      /*! */
      void destroy_visual_representation(
        VisualRepresentationStream& vrs ) const;

    public:
      FOUNDATION_INLINE Unit::Id id() const
      { return _id; }

      FOUNDATION_INLINE World& world() const {
        assert(_id != Unit::invalid);
        return _world;
      }

      FOUNDATION_INLINE const Resource::Handle<UnitResource>& type() const {
        assert(_id != Unit::invalid);
        return _type;
      }

      FOUNDATION_INLINE SceneGraph& scene_graph()
      { return _scene_graph; }

      FOUNDATION_INLINE const SceneGraph& scene_graph() const
      { return _scene_graph; }

    private:
      World& _world;
      Unit::Id _id;
      Resource::Handle<UnitResource> _type;
      SceneGraph _scene_graph;
  };
} // butane

#endif // _BUTANE_UNIT_H_
