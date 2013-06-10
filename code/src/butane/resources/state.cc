// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/state.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("state resources", Allocators::heap());
    return allocator;
  }

  const Resource::Type StateResource::type(
    "state", "state",
    (Resource::Type::Load)&StateResource::load,
    (Resource::Type::Unload)&StateResource::unload,
    (Resource::Type::Compile)&StateResource::compile);

  StateResource::StateResource(
    const Resource::Id id
  ) : butane::Resource(StateResource::type, id)
    , _rasterizer_state(nullptr)
    , _depth_stencil_state(nullptr)
    , _blend_state(nullptr)
  {
  }

  StateResource::~StateResource()
  {
  }

  StateResource* StateResource::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    const LogScope _("StateResource::load");

    const MemoryResidentData& mrd =
      *((const MemoryResidentData*)stream.memory_resident_data());

    StateResource* state =
      make_new(StateResource, allocator())(id);

    state->_rasterizer_state = RasterizerState::create(mrd.rsd);
    state->_depth_stencil_state = DepthStencilState::create(mrd.dssd);
    state->_blend_state = BlendState::create(mrd.bsd);

    return state;
  }

  void StateResource::unload(
    StateResource* state )
  {
    const LogScope _("StateResource::unload");

    assert(state != nullptr);
    make_delete(StateResource, allocator(), state);
  }

  static bool fill_from_string(
    const char* str,
    RasterizerState::Fill::Value& fill )
  {
    if (strcmp("solid", str) == 0) {
      fill = RasterizerState::Fill::SOLID;
      return true;
    } else if ((strcmp("wire", str) == 0) || (strcmp("wireframe", str) == 0)) {
      fill = RasterizerState::Fill::WIREFRAME;
      return true;
    }

    return false;
  }

  static bool cull_from_string(
    const char* str,
    RasterizerState::Cull::Value& fill )
  {
    if (strcmp("none", str) == 0) {
      fill = RasterizerState::Cull::NONE;
      return true;
    } else if ((strcmp("front", str) == 0)) {
      fill = RasterizerState::Cull::FRONT;
      return true;
    } else if ((strcmp("back", str) == 0)) {
      fill = RasterizerState::Cull::BACK;
      return true;
    }

    return false;
  }

  bool StateResource::compile(
    const Resource::Compiler::Input& input,
    const Resource::Compiler::Output& output )
  {
    const LogScope _("StateResource::compile");

    size_t sjson_len = 0;
    const char* sjson =
      (const char*)File::read_in(input.data, Allocators::heap(), &sjson_len);

    Array<uint8_t> blob(Allocators::scratch(), 1 << 12 /* 4kb */);
    blob.resize(blob.reserved());
    zero((void*)blob.raw(), blob.size());
    sjson::Parser parser(Allocators::heap(), (void*)&blob[0], blob.size());

    if (!parser.parse(sjson, sjson_len)) {
      Allocators::heap().free((void*)sjson);
      return false; }

    Allocators::heap().free((void*)sjson);

    const sjson::Object* root =
      (const sjson::Object*)&blob[0];

    MemoryResidentData mrd;

    mrd.rsd.fill = RasterizerState::Fill::SOLID; {
      const sjson::String* fill =
        (const sjson::String*)root->find("rasterizer.fill");
      if (fill && fill->is_string())
        fill_from_string(fill->raw(), mrd.rsd.fill);
    }

    mrd.rsd.cull = RasterizerState::Cull::BACK; {
      const sjson::String* cull =
        (const sjson::String*)root->find("rasterizer.cull");
      if (cull && cull->is_string())
        cull_from_string(cull->raw(), mrd.rsd.cull);
    }

    mrd.rsd.scissor = false; {
      const sjson::Boolean* scissor =
        (const sjson::Boolean*)root->find("rasterizer.scissor");
      if (scissor && scissor->is_boolean())
        mrd.rsd.scissor = scissor->value();
    }

    mrd.dssd.tests            = false;
    mrd.dssd.write            = true;
    mrd.dssd.func             = DepthStencilState::Comparison::LESS;
    mrd.dssd.stencil          = false;
    mrd.dssd.front.fail       = DepthStencilState::StencilOp::KEEP;
    mrd.dssd.front.depth_fail = DepthStencilState::StencilOp::KEEP;
    mrd.dssd.front.pass       = DepthStencilState::StencilOp::KEEP;
    mrd.dssd.front.func       = DepthStencilState::Comparison::ALWAYS;
    mrd.dssd.back.fail        = DepthStencilState::StencilOp::KEEP;
    mrd.dssd.back.depth_fail  = DepthStencilState::StencilOp::KEEP;
    mrd.dssd.back.pass        = DepthStencilState::StencilOp::KEEP;
    mrd.dssd.back.func        = DepthStencilState::Comparison::ALWAYS;

    mrd.bsd.enabled = false;
    mrd.bsd.color.src = BlendState::Blend::ONE;
    mrd.bsd.color.dest = BlendState::Blend::ZERO;
    mrd.bsd.color.op = BlendState::BlendOp::ADD;
    mrd.bsd.alpha.src = BlendState::Blend::ONE;
    mrd.bsd.alpha.dest = BlendState::Blend::ZERO;
    mrd.bsd.alpha.op = BlendState::BlendOp::ADD;

    if (!File::write_out(output.memory_resident_data, (const void*)&mrd, sizeof(MemoryResidentData)))
      return false;

    return true;
  }
} // butane
