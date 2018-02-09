//===-- yeti/resources/entity_resource_format.h ---------*- mode: C++11 -*-===//
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

#ifndef _YETI_ENTITY_RESOURCE_FORMAT_H_
#define _YETI_ENTITY_RESOURCE_FORMAT_H_

namespace yeti {

namespace entity_resource_format {
  // We pack our structures and manually align and pad since we're using these
  // structures with memory-mapped files across architectures and compilers.
  #pragma pack(push, 1)

  struct Header {
    // Number of entities in total.
    u32 num_of_entities;
    // Number of components in total.
    u32 num_of_components;
    // Offset to relevant data.
    u32 offset_to_entities;
    u32 offset_to_components;
    u32 offset_to_hierarchy;
    // Pad to 16-byte alignment.
    u8 padding[12];
  };

  struct Entity {
    // Universally unique identifier assigned to instance.
    u8 identifier[16];
  };

  struct Component {
    // Type of component, i.e. `Component::Id`.
    u32 type;
    // Version of component, i.e. `Component::version`.
    u32 version;
    // Number of instances.
    u32 count;
    // Offsets to relevant data.
    u32 offset_to_mapping;
    u32 offset_to_instances;
    u32 offset_to_data;
  };

  // Maps from component to entity.
  struct Mappings {
    u32 indef_of_entity[0];
  };

  struct Instance {
    // Universally unique identifier assigned to instance.
    u8 identifier[16];
  };

  struct Hierarchy {
    // Index of parent for each entity. Must be mapped to runtime handles.
    u32 index_of_parent[0];
  };

  #pragma pack(pop)
}

} // yeti

#endif // _YETI_ENTITY_RESOURCE_FORMAT_H_
