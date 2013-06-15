// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_PIXEL_SHADER_H_
#define _BUTANE_PIXEL_SHADER_H_

#include <butane/resource.h>

namespace butane {
  class BUTANE_EXPORT PixelShader
    : public butane::Resource
  {
    __foundation_trait(PixelShader, non_copyable);

    private:
      struct MemoryResidentData {
        uint32_t byte_code_len;
        relative_ptr<void*> byte_code;
      };

    public:
      static const Resource::Type type;

    protected:
      PixelShader(
        const Resource::Id id );

      virtual ~PixelShader();

    private:
      static PixelShader* load(
        const Resource::Id id,
        const Resource::Stream& stream );

      static void unload(
        PixelShader* pixel_shader );

      static bool compile(
        const Resource::Compiler::Input& input,
        const Resource::Compiler::Output& output );
  };
} // butane

#endif // _BUTANE_PIXEL_SHADER_H_
