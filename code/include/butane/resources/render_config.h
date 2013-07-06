// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RENDER_CONFIG_RESOURCE_H_
#define _BUTANE_RENDER_CONFIG_RESOURCE_H_

#include <butane/resource.h>
#include <butane/math.h>
#include <butane/graphics/pixel_format.h>

namespace butane {
  class BUTANE_EXPORT RenderConfigResource final
    : public butane::Resource
  {
    __foundation_trait(RenderConfigResource, non_copyable);

    public:
      /*! */
      class Resource {
        public:
          typedef uint32_t Id;

          typedef Hash<uint32_t, murmur_hash> Name;

          enum Type {
            /*! */
            RENDER_TARGET = 1u,
            /*! */
            DEPTH_STENCIL_TARGET = 2u,
          };

        public:
          /*! */
          static const Id invalid = (Id)0xFFFFFFFFFFFFFFFFull;

        public:
          /*! */
          Resource::Id id;

          /*! */
          Resource::Name name;

          /*! */
          Resource::Type type;

          /*! */
          union {
            struct {
              struct {
                float x, y;
              } scale;
              PixelFormat::Value format;
              uint8_t clear;
            } render_or_depth_stencil_target;
          };
      };

      /*! */
      class Layer {
        public:
          typedef uint8_t Id;

          typedef Hash<uint32_t, murmur_hash> Name;

        public:
          /*! */
          static const size_t maximum_num_of_layers = 255;

        public:
          /*! */
          Layer::Id id;

          /*! */
          Layer::Name name;

          /*! */
          size_t num_of_samplers;

          /*! */
          Resource::Id samplers[8];

          /*! */
          size_t num_of_render_targets;

          /*! */
          Resource::Id render_targets;

          /*! */
          Resource::Id depth_stencil_target;

          // /*! */
          // Generator::Id generator;
      };

    private:
      struct MemoryResidentData {
        size_t num_of_globals;
        relative_ptr<Resource*> globals;
        size_t num_of_layers;
        relative_ptr<Layer*> layers;
      };

    public:
      static const butane::Resource::Type& type();

    private:
      RenderConfigResource(
        const butane::Resource::Id id );

      ~RenderConfigResource();

    public: /* private */
      static RenderConfigResource* load(
        const butane::Resource::Id id,
        const butane::Resource::Stream& stream );

      static void unload(
        RenderConfigResource* render_config );

      static bool compile(
        const butane::Resource::Compiler::Input& input,
        const butane::Resource::Compiler::Output& output );

    public:
      FOUNDATION_INLINE const Array<Resource>& globals() const
      { return _globals; }

      FOUNDATION_INLINE const Array<Layer>& layers() const
      { return _layers; }

      const Layer* find_layer_by_name( const Layer::Name name ) const {
        Layer* layer;
        return _name_to_layer.find(name, layer) ? layer : nullptr;
      }

    private:
      Array<Resource> _globals;
      Array<Layer> _layers;
      HashTable<Layer::Name, Layer*> _name_to_layer;
  };
} // butane

#endif // _BUTANE_RENDER_CONFIG_RESOURCE_H_
