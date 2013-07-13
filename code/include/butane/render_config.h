// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RENDER_CONFIG_H_
#define _BUTANE_RENDER_CONFIG_H_

#include <butane/butane.h>
#include <butane/resource.h>
#include <butane/math.h>
#include <butane/graphics/pixel_format.h>

namespace butane {
  class BUTANE_EXPORT RenderConfig final
    : public butane::Resource
  {
    __foundation_trait(RenderConfig, non_constructable);
    __foundation_trait(RenderConfig, non_copyable);

    public:
      class Resource;
      class Generator;
      class Layer;

      #include <butane/render_config/resource.h>
      #include <butane/render_config/generator.h>
      #include <butane/render_config/layer.h>

    public:
      struct MemoryResidentData {
        size_t num_of_globals;
        relative_ptr<Resource*> globals;
        size_t num_of_layers;
        relative_ptr<Layer*> layers;
      };

    public:
      /*! */
      static const butane::Resource::Type& type();

    private:
      RenderConfig(
        const butane::Resource::Id id );

      ~RenderConfig();

    public: /* private */
      static RenderConfig* load(
        const butane::Resource::Id id,
        const butane::Resource::Stream& stream );

      static void unload(
        RenderConfig* render_config );

      static bool compile(
        const butane::Resource::Compiler::Input& input,
        const butane::Resource::Compiler::Output& output );

    public:
      /*! */
      FOUNDATION_INLINE const Array<Resource>& globals() const
      { return _globals; }

      /*! */
      FOUNDATION_INLINE const Array<Layer>& layers() const
      { return _layers; }

      /*! */
      const Resource* find_global_by_name( const Resource::Name name ) const {
        Resource* resource;
        return _name_to_global.find(name, resource) ? resource : nullptr;
      }

      /*! */
      const Layer* find_layer_by_name( const Layer::Name name ) const {
        Layer* layer;
        return _name_to_layer.find(name, layer) ? layer : nullptr;
      }

    private:
      Array<Resource> _globals;
      HashTable<Resource::Name, Resource*> _name_to_global;
      Array<Layer> _layers;
      HashTable<Layer::Name, Layer*> _name_to_layer;
  };
} // butane

#endif // _BUTANE_RENDER_CONFIG_H_
