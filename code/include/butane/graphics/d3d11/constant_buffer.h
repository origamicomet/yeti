// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_CONSTANT_BUFFER_H_
#define _BUTANE_D3D11_CONSTANT_BUFFER_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/constant_buffer.h>

namespace butane {
  class BUTANE_EXPORT D3D11ConstantBuffer final
    : public ConstantBuffer
  {
    __foundation_trait(D3D11ConstantBuffer, non_copyable);

    private:
      friend class ConstantBuffer;

    private:
      D3D11ConstantBuffer();
      ~D3D11ConstantBuffer();

    public:
      void destroy() override;

    public:
      void update(
        const void* data,
        const size_t data_len ) override;

    public:
      FOUNDATION_INLINE ID3D11Buffer* resource() const
      { return _resource; }

    private:
      ID3D11Buffer* _resource;
  };
} // butane

#endif // _BUTANE_D3D11_CONSTANT_BUFFER_H_
