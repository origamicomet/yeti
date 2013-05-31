// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/render_device.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("render devices", Allocators::heap());
    return allocator;
  }

  D3D11RenderDevice::D3D11RenderDevice()
    : _factory(nullptr)
    , _adapter(nullptr)
    , _device(nullptr)
    , _context(nullptr)
  {
  }

  D3D11RenderDevice::~D3D11RenderDevice()
  {
    if (_factory)
      _factory->Release();
    if (_adapter)
      _adapter->Release();
    if (_device)
      _device->Release();
    if (_context)
      _context->Release();
  }

  RenderDevice* RenderDevice::create(
    const uint32_t adapter )
  {
    const LogScope _("D3D11RenderDevice::create");

    D3D11RenderDevice* render_device =
      make_new(D3D11RenderDevice, allocator())();

    /* render_device->_factory = */ {
      const HRESULT hr = CreateDXGIFactory(
        __uuidof(IDXGIFactory), (void**)&render_device->_factory);
      if (FAILED(hr))
        fail("CreateDXGIFactory failed, hr=%#08x", hr);
    }

    /* render_device->_adapter = */ {
      const HRESULT hr = render_device->_factory->EnumAdapters(
        adapter, &render_device->_adapter);
      if (FAILED(hr))
        fail("IDXGIFactory::EnumAdapters failed, hr=%#08x", hr);
    }

  #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_DEVELOPMENT_BUILD)
    static const UINT flags = 0; // D3D11_CREATE_DEVICE_DEBUG;
  #else
    static const UINT flags = 0;
  #endif

    /* render_device->_device, render_device->_context = */ {
      static const D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
      D3D_FEATURE_LEVEL feature_level_;
      const HRESULT hr = D3D11CreateDevice(
        render_device->_adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, flags, &feature_level, 1, /* D3D11 only */
        D3D11_SDK_VERSION, &render_device->_device, &feature_level_, &render_device->_context);
      if (FAILED(hr))
        fail("D3D11CreateDevice failed, h=%#08x", hr);
    }

    return render_device;
  }

  void D3D11RenderDevice::destroy()
  {
    make_delete(D3D11RenderDevice, allocator(), this);
  }

  void D3D11RenderDevice::dispatch(
    size_t num_render_contexts,
    const RenderContext** render_contexts )
  {
    assert(num_render_contexts > 0);
    assert(render_contexts != nullptr);

    // 1: Gather and Merge
    Array<RenderContext::Command> commands(allocator());
    for (size_t context = 0; context < num_render_contexts; ++context) {
      const size_t offset = commands.size();
      const size_t num_of_cmds = render_contexts[context]->num_of_commands();
      commands.resize(commands.size() + num_of_cmds);
      for (size_t c = 0; c < num_of_cmds; ++c) {
        RenderContext::Command& cmd = commands[offset + c];
        cmd = render_contexts[context]->commands()[c];
        cmd.offset += (uintptr_t)render_contexts[context]->stream(); }
    }

    // 2: Sort
    sort(commands.raw(), commands.size());

    // 3: Build
    for (auto iter = commands.begin(); iter != commands.end(); ++iter)
      dispatch(*((const Command*)(*iter).offset));
  }

  void D3D11RenderDevice::dispatch(
    const Command& command )
  {
    switch (command.type) {
      case Command::CLEAR_RENDER_TARGET_VIEW: {
        const Commands::ClearRenderTargetView& cmd =
          *((const Commands::ClearRenderTargetView*)&command);
        _context->ClearRenderTargetView(cmd.view, &cmd.rgba[0]);
      } return;

      case Command::CLEAR_DEPTH_STENCIL_VIEW: {
        static const UINT clear_flags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
        const Commands::ClearDepthStencilView& cmd =
          *((const Commands::ClearDepthStencilView*)&command);
        _context->ClearDepthStencilView(cmd.view, clear_flags, cmd.depth, cmd.stencil);
      } return;

      case Command::PRESENT: {
        const Commands::Present& cmd =
          *((const Commands::Present*)&command);
        cmd.swap_chain->Present(cmd.interval, 0);
      } return;
    }

    __builtin_unreachable();
  }
} // butane
