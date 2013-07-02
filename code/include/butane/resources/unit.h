// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_UNIT_RESOURCE_H_
#define _BUTANE_UNIT_RESOURCE_H_

#include <butane/resource.h>
#include <butane/scene_graph.h>

namespace butane {
  class Unit;

  class BUTANE_EXPORT UnitResource final
    : public butane::Resource
  {
    __foundation_trait(UnitResource, non_copyable);

    private:
      friend class Unit;

    private:
      struct MemoryResidentData {
        relative_ptr<SceneGraph::Serialized*> scene_graph;
      };

    public:
      static const Resource::Type& type();

    protected:
      UnitResource(
      const Resource::Id id );

      ~UnitResource();

    public: /* private */
      static UnitResource* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        UnitResource* unit );

      static bool compile(
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );

    private:
      SceneGraph::Serialized* _scene_graph;
  };
} // butane

#endif // _BUTANE_UNIT_RESOURCE_H_
