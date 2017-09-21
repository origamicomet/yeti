//===-- yeti/resources/render_config_format.h -----------*- mode: C++11 -*-===//
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

#ifndef _YETI_RENDER_CONFIG_FORMAT_H_
#define _YETI_RENDER_CONFIG_FORMAT_H_

namespace yeti {

namespace render_config_format {
  struct Header {
    u32 num_variables;
    u32 num_global_resources;
    u32 num_local_resources;
    u32 num_generators;
    u32 num_modifiers;
    u32 num_layers;

    u32 variables_offset;
    u32 global_resources_offset;
    u32 local_resources_offset;
    u32 generators_offset;
    u32 modifiers_offset;
    u32 layers_offset;
  };

  typedef char Name[256];

  struct Variable {
    Name name;
    u32 hash_of_name;
  };

  struct Binding {
    enum Type {
      BOUND_TO_CONSTANT = 0,
      BOUND_TO_VARIABLE = 1
    };
  };

  struct Resource {
    Name name;
    u32 hash_of_name;
  };

  struct Generator {
    Name name;
    u32 hash_of_name;
  };

  struct Modifier {
    // What will this look like?
  };

  struct Layer {
    Name name;
    u32 hash_of_name;

    Name scope;
    u32 hash_of_scope;

    // TODO(mtwilliams): Layer specific samplers?
    // u32 samplers[8];

    u32 render_targets[8];
    u32 depth_stencil_target;

    u32 order;

    u32 generator;
  };

  // TODO(mtwilliams): Hints.
#if 0
  // How will these work with extensions?

  struct Hint {
    enum Type {
      LIFETIME = 1,
    };

    Type type;
  };

  // Indicates when resources are "alive."
  struct LifetimeHint : Hint {
    u32 resource;

    // First layer that references.
    u32 first_referenced_by;

    // Last layer that references.
    u32 last_referenced_by;
  };
#endif
}

#if 0

  struct Resource {
    enum Type {
      TEXTURE              = 1,
      RENDER_TARGET        = 2,
      DEPTH_STENCIL_TARGET = 3
    };

    struct Texture {
      u32 dimensions;

      Variable width;
      Variable height;
      Variable depth;

      u32 format;

      struct {
        u32 width_is_ratio     : 1;
        u32 height_is_ratio    : 1;
        u32 reserved           : 30;
      } flags;

      u32 resource;
    };

    struct RenderTarget {
      Variable x;
      Variable y;

      Variable width;
      Variable height;

      u32 format;

      struct {
        u32 x_is_ratio         : 1;
        u32 y_is_ratio         : 1;
        u32 width_is_ratio     : 1;
        u32 height_is_ratio    : 1;
        u32 reserved           : 28;
      } flags;

      u32 storage;
    };

    struct DepthStencilTarget {
      Variable x;
      Variable y;

      Variable width;
      Variable height;

      u32 format;

      struct {
        u32 x_is_ratio         : 1;
        u32 y_is_ratio         : 1;
        u32 width_is_ratio     : 1;
        u32 height_is_ratio    : 1;
        u32 reserved           : 28;
      } flags;

      u32 storage;
    };

    char name[256];
    u32 hash_of_name;

    u32 type;

    union {
      Texture texture;
      RenderTarget render_target;
      DepthStencilTarget depth_stencil_target;
    };
  };
}

#endif

} // yeti

#endif // _YETI_RENDER_CONFIG_FORMAT_H_
