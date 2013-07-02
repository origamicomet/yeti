// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_WORLD_H_
#define _BUTANE_WORLD_H_

#include <butane/butane.h>
#include <butane/unit.h>

namespace butane {
  class BUTANE_EXPORT World final {
    __foundation_trait(World, non_copyable);

    private:
      World();

      ~World();

    public:
      static World* create();

      Unit& spawn_unit(
        const Resource::Handle<UnitResource>& type,
        const Vec3f& position = Vec3f(0.0f, 0.0f, 0.0f),
        const Quatf& rotation = Quatf(1.0f, 0.0f, 0.0f, 0.0f),
        const Vec3f& scale = Vec3f(1.0f, 1.0f, 1.0f) );

      void destroy_unit(
        const Unit::Id id );

      void update(
        const float dt );

      void destory();

    public:
      FOUNDATION_INLINE size_t num_of_units() const
      { return _units.size(); }

      FOUNDATION_INLINE const Unit* units() const
      { return _units.raw(); }

    private:
      Unit::Id _next_available_unit_id;
      Array<Unit::Id> _unit_ids;
      Array<Unit> _units;
  };
} // butane

#endif // _BUTANE_WORLD_H_
