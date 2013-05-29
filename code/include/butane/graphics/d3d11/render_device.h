// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_RENDER_DEVICE_H_
#define _BUTANE_D3D11_RENDER_DEVICE_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/render_device.h>

namespace butane {
  class BUTANE_EXPORT D3D11RenderDevice final
    : public RenderDevice
  {
    __foundation_trait(D3D11RenderDevice, non_copyable);

    private:
      friend class RenderDevice;

    protected:
      D3D11RenderDevice();
      ~D3D11RenderDevice();

    public:
      void destroy() override;

    public:
      FOUNDATION_INLINE IDXGIFactory* factory() const
      { return _factory; }

      FOUNDATION_INLINE IDXGIAdapter* adapter() const
      { return _adapter; }

      FOUNDATION_INLINE ID3D11Device* device() const
      { return _device; }

      FOUNDATION_INLINE ID3D11DeviceContext* context() const
      { return _context; }

    private:
      IDXGIFactory* _factory;
      IDXGIAdapter* _adapter;
      ID3D11Device* _device;
      ID3D11DeviceContext* _context;
  };
} // butane

#endif // _BUTANE_D3D11_RENDER_DEVICE_H_
