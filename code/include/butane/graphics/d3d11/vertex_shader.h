// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_VERTEX_SHADER_H_
#define _BUTANE_D3D11_VERTEX_SHADER_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/vertex_shader.h>

namespace butane {
  class BUTANE_EXPORT D3D11VertexShader final
    : public VertexShader
  {
    __foundation_trait(D3D11VertexShader, non_copyable);

    private:
      friend class VertexShader;

    private:
      D3D11VertexShader(
        const Resource::Id id );

      ~D3D11VertexShader();

    public:
      FOUNDATION_INLINE ID3D11VertexShader* resource() const
      { return _resource; }

    private:
      ID3D11VertexShader* _resource;
      Array<uint8_t> _byte_code;
  };
} // butane

#endif // _BUTANE_D3D11_VERTEX_SHADER_H_
