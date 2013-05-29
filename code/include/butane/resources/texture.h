// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TEXTURE_RESOURCE_H_
#define _BUTANE_TEXTURE_RESOURCE_H_

#include <butane/resource.h>

#include <butane/graphics/dds.h>
#include <butane/graphics/texture.h>

namespace butane {
  class BUTANE_EXPORT TextureResource final
    : public butane::Resource
  {
    __foundation_trait(TextureResource, non_copyable);

    private:
      // The texture has precomputed mipmaps.
      static const uint32_t HasMipmaps = (1u << 0u);

      struct MemoryResidentData {
        Texture::Type type;
        PixelFormat pixel_format;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        uint32_t flags;
      };

    public:
      static const Resource::Type type;

    protected:
      TextureResource(
        const Resource::Id id );

      ~TextureResource();

    private:
      static TextureResource* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        TextureResource* texture );

      static bool compile(
        const Resource::Compiler::Source& src,
        const Resource::Compiler::Stream& cs );

    public: /* private */
      // void stream();
      // void mark_for_streaming();

    public:
      Texture* texture() const
      { return _texture; }

    private:
      Texture* _texture;
  };
} // butane

#endif // _BUTANE_TEXTURE_RESOURCE_H_
