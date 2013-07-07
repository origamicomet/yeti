// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_RENDER_DEVICE_H_
#define _BUTANE_D3D11_RENDER_DEVICE_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/render_device.h>
#include <butane/graphics/vertex_declaration.h>

namespace butane {
  class VertexShader;
  class BUTANE_EXPORT D3D11RenderDevice final
    : public RenderDevice
  {
    __foundation_trait(D3D11RenderDevice, non_copyable);

    private:
      friend class RenderDevice;

    public:
      struct Command {
        public:
          enum Type {
            BIND_RENDER_AND_DEPTH_STENCIL_VIEWS = 1,
            SET_VIEWPORTS                       = 2,
            CLEAR_RENDER_TARGET_VIEW            = 3,
            CLEAR_DEPTH_STENCIL_VIEW            = 4,
            DRAW                                = 5,
            PRESENT                             = 6
          };

        protected:
          Command( const Type type )
            : type(type)
          {}

        public:
          Type type;
      };

      class Commands final {
        __foundation_trait(Commands, non_constructable);
        __foundation_trait(Commands, non_copyable);

        public:
          struct BindRenderAndDepthStencilViews final
            : public Command
          {
            public:
              BindRenderAndDepthStencilViews()
                : Command(Command::BIND_RENDER_AND_DEPTH_STENCIL_VIEWS)
              {}

            public:
              UINT num_of_render_target_views;
              ID3D11RenderTargetView* render_target_views[8];
              ID3D11DepthStencilView* depth_stencil_view;
          };

          struct SetViewports final
            : public Command
          {
            public:
              SetViewports()
                : Command(Command::SET_VIEWPORTS)
              {}

            public:
              UINT num_of_viewports;
              D3D11_VIEWPORT viewports[8];
          };

          struct ClearRenderTargetView final
            : public Command
          {
            public:
              ClearRenderTargetView()
                : Command(Command::CLEAR_RENDER_TARGET_VIEW)
              {}

            public:
              ID3D11RenderTargetView* view;
              float rgba[4];
          };

          struct ClearDepthStencilView final
            : public Command
          {
            public:
              ClearDepthStencilView()
                : Command(Command::CLEAR_DEPTH_STENCIL_VIEW)
              {}

            public:
              ID3D11DepthStencilView* view;
              float depth;
              uint32_t stencil;
          };

          struct Draw final
            : public Command
          {
            public:
              Draw()
                : Command(Command::DRAW)
              {}

            public:
              ID3D11RasterizerState* rasterizer_state;
              ID3D11DepthStencilState* depth_stencil_state;
              ID3D11BlendState* blend_state;
              UINT num_of_samplers_and_textures;
              ID3D11SamplerState* samplers[8];
              ID3D11ShaderResourceView* textures[8];
              ID3D11VertexShader* vertex_shader;
              ID3D11PixelShader* pixel_shader;
              UINT stride;
              ID3D11InputLayout* input_layout;
              ID3D11Buffer* index_buffer;
              ID3D11Buffer* vertex_buffer;
              UINT num_of_constant_buffers;
              ID3D11Buffer* constant_buffers[8];
              D3D11_PRIMITIVE_TOPOLOGY topology;
              UINT num_of_indicies;
          };

          struct Present final
            : public Command
          {
            public:
              Present()
                : Command(Command::PRESENT)
              {}

            public:
              IDXGISwapChain* swap_chain;
              uint32_t interval;
          };
      };

    protected:
      D3D11RenderDevice();
      ~D3D11RenderDevice();

    public:
      void dispatch(
        size_t num_render_contexts,
        const RenderContext** render_contexts ) override;

    private:
      void dispatch(
        const Command& command );

    public:
      void destroy() override;

    public: /* private */
      ID3D11InputLayout* find_or_create_input_layout(
        const VertexDeclaration input,
        const VertexShader* shader );

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
      HashTable<uint64_t, ID3D11InputLayout*> _input_layout_cache;
  };
} // butane

#endif // _BUTANE_D3D11_RENDER_DEVICE_H_
