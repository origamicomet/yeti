//===-- yeti/resources/entity_resource.h ----------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_ENTITY_RESOURCE_H_
#define _YETI_ENTITY_RESOURCE_H_

#include "yeti/core.h"

#include "yeti/resource.h"

namespace yeti {

#pragma pack(push, 1)

namespace entity_resource {
  struct Header {
    // Number of entities in total.
    u32 num_of_entities;
    // Number of components in total.
    u32 num_of_components;
    // Offset to relevant data.
    u32 offset_to_entities;
    u32 offset_to_components;
    u32 offset_to_hierarchy;
  };

  struct Entity {
    // Universally unique identifier assigned to instance.
    u8 identifier[128 / 8];
    // Name of entity. Used for debugging. *Not* mapped to runtime naming system!
    char name[31+1];
  };

  struct Component {
    // Type of component, i.e. `Component::Id`.
    u32 type;
    // Name of component. Used for debugging.
    char name[31+1];
    // Size of each instance in bytes.
    u32 size;
    // Number of instances.
    u32 count;
    // Offsets to relevant data.
    u32 offset_to_instances;
    u32 offset_to_mapping;
  };

  // Maps from component to entity.
  typedef u32 Mapping;

  struct Instance {
    // Universally unique identifier assigned to instance.
    u8 identifier[128 / 8];
    // Name of instance. Used for debugging.
    char name[31+1];
    // Component data.
    u8 data[0];
  };

  struct Hierarchy {
    // Index of parent for each entity. Must be mapped to runtime handles.
    u32 index_of_parent[0];
  };
}

#pragma pack(pop)

class YETI_PUBLIC EntityResource : public Resource {
 YETI_DISALLOW_COPYING(EntityResource)

 private:
  EntityResource(Resource::Id id);
  ~EntityResource();

 public:
  /// \internal
  static const Resource::Type *type();

 private:
  static Resource *prepare(Resource::Id id);

  static void load(Resource *resource, const Resource::Data &data);
  static void unload(Resource *resource);

  static bool compile(const resource_compiler::Environment *env,
                      const resource_compiler::Input *input,
                      const resource_compiler::Output *output);

  static bool compatible(u32 version);

 private:
  // TODO(mtwilliams): Components.
};

} // yeti

#endif // _YETI_ENTITY_RESOURCE_H_
