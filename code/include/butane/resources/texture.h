// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TEXTURE_RESOURCE_H_
#define _BUTANE_TEXTURE_RESOURCE_H_

#include <butane/resource.h>

#include <butane/graphics/dds.h>
#include <butane/graphics/sampler.h>
#include <butane/graphics/texture.h>

namespace butane {
  class BUTANE_EXPORT TextureResource final
    : public butane::Resource
  {
    __foundation_trait(TextureResource, non_copyable);

    private:
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
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );

    public:
      Texture* texture() const
      { return _texture; }

      Sampler* sampler() const
      { return _sampler; }

    private:
      Texture* _texture;
      Sampler* _sampler;
  };
} // butane

#endif // _BUTANE_TEXTURE_RESOURCE_H_
