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

  static bool comparison_from_string(
    const char* str,
    DepthStencilState::Comparison::Value& func )
  {
    if (strcmp("never", str) == 0) {
      func = DepthStencilState::Comparison::NEVER;
      return true;
    } else if (strcmp("less", str) == 0) {
      func = DepthStencilState::Comparison::LESS;
      return true;
    } else if (strcmp("equal", str) == 0) {
      func = DepthStencilState::Comparison::EQUAL;
      return true;
    } else if (strcmp("less_equal", str) == 0) {
      func = DepthStencilState::Comparison::LESS_EQUAL;
      return true;
    } else if (strcmp("greater", str) == 0) {
      func = DepthStencilState::Comparison::GREATER;
      return true;
    } else if (strcmp("not_equal", str) == 0) {
      func = DepthStencilState::Comparison::NOT_EQUAL;
      return true;
    } else if (strcmp("greater_equal", str) == 0) {
      func = DepthStencilState::Comparison::GREATER_EQUAL;
      return true;
    } else if (strcmp("always", str) == 0) {
      func = DepthStencilState::Comparison::ALWAYS;
      return true;
    }

    return false;
  }

  static bool stencil_op_from_string(
    const char* str,
    DepthStencilState::StencilOp::Value& op )
  {
    if (strcmp("keep", str) == 0) {
      op = DepthStencilState::StencilOp::KEEP;
      return true;
    } else if (strcmp("zero", str) == 0) {
      op = DepthStencilState::StencilOp::ZERO;
      return true;
    } else if (strcmp("replace", str) == 0) {
      op = DepthStencilState::StencilOp::REPLACE;
      return true;
    } else if (strcmp("incr_sat", str) == 0) {
      op = DepthStencilState::StencilOp::INCR_SAT;
      return true;
    } else if (strcmp("decr_sat", str) == 0) {
      op = DepthStencilState::StencilOp::DECR_SAT;
      return true;
    } else if (strcmp("invert", str) == 0) {
      op = DepthStencilState::StencilOp::INVERT;
      return true;
    } else if (strcmp("incr", str) == 0) {
      op = DepthStencilState::StencilOp::INCR;
      return true;
    } else if (strcmp("decr", str) == 0) {
      op = DepthStencilState::StencilOp::DECR;
      return true;
    }

    return false;
  }

  static bool blend_from_string(
    const char* str,
    BlendState::Blend::Value& blend )
  {
    if (strcmp("zero", str) == 0) {
      blend = BlendState::Blend::ZERO;
      return true;
    } else if (strcmp("one", str) == 0) {
      blend = BlendState::Blend::ONE;
      return true;
    } else if (strcmp("src_color", str) == 0) {
      blend = BlendState::Blend::SRC_COLOR;
      return true;
    } else if (strcmp("inv_src_color", str) == 0) {
      blend = BlendState::Blend::INV_SRC_COLOR;
      return true;
    } else if (strcmp("src_alpha", str) == 0) {
      blend = BlendState::Blend::SRC_ALPHA;
      return true;
    } else if (strcmp("inv_src_alpha", str) == 0) {
      blend = BlendState::Blend::INV_SRC_ALPHA;
      return true;
    } else if (strcmp("dest_alpha", str) == 0) {
      blend = BlendState::Blend::DEST_ALPHA;
      return true;
    } else if (strcmp("inv_dest_alpha", str) == 0) {
      blend = BlendState::Blend::INV_DEST_ALPHA;
      return true;
    } else if (strcmp("dest_color", str) == 0) {
      blend = BlendState::Blend::DEST_COLOR;
      return true;
    } else if (strcmp("inv_dest_color", str) == 0) {
      blend = BlendState::Blend::INV_DEST_COLOR;
      return true;
    } else if (strcmp("src_alpha_sat", str) == 0) {
      blend = BlendState::Blend::SRC_ALPHA_SAT;
      return true;
    }

    return false;
  }

  static bool blend_op_from_string(
    const char* str,
    BlendState::BlendOp::Value& op )
  {
    if (strcmp("add", str) == 0) {
      op = BlendState::BlendOp::ADD;
      return true;
    } else if (strcmp("subtract", str) == 0) {
      op = BlendState::BlendOp::SUBTRACT;
      return true;
    } else if (strcmp("rev_subtract", str) == 0) {
      op = BlendState::BlendOp::REV_SUBTRACT;
      return true;
    } else if (strcmp("min", str) == 0) {
      op = BlendState::BlendOp::MIN;
      return true;
    } else if (strcmp("max", str) == 0) {
      op = BlendState::BlendOp::MAX;
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

    mrd.dssd.tests = true; {
      const sjson::Boolean* tests =
        (const sjson::Boolean*)root->find("depth.tests");
      if (tests && tests->is_boolean())
        mrd.dssd.tests = tests->value();
    }

    mrd.dssd.writes = true; {
      const sjson::Boolean* writes =
        (const sjson::Boolean*)root->find("depth.writes");
      if (writes && writes->is_boolean())
        mrd.dssd.writes = writes->value();
    }

    mrd.dssd.func = DepthStencilState::Comparison::LESS; {
      const sjson::String* func =
        (const sjson::String*)root->find("depth.func");
      if (func && func->is_string())
        comparison_from_string(func->raw(), mrd.dssd.func);
    }

    mrd.dssd.stencil = false; {
      const sjson::Boolean* tests =
        (const sjson::Boolean*)root->find("stencil.tests");
      if (tests && tests->is_boolean())
        mrd.dssd.stencil = tests->value();
    }

    mrd.dssd.front.fail = DepthStencilState::StencilOp::KEEP; {
      const sjson::String* op =
        (const sjson::String*)root->find("stencil.front.fail");
      if (op && op->is_string())
        stencil_op_from_string(op->raw(), mrd.dssd.front.fail);
    }

    mrd.dssd.front.depth_fail = DepthStencilState::StencilOp::KEEP; {
      const sjson::String* op =
        (const sjson::String*)root->find("stencil.front.depth");
      if (op && op->is_string())
        stencil_op_from_string(op->raw(), mrd.dssd.front.depth_fail);
    }

    mrd.dssd.front.pass = DepthStencilState::StencilOp::KEEP; {
      const sjson::String* op =
        (const sjson::String*)root->find("stencil.front.pass");
      if (op && op->is_string())
        stencil_op_from_string(op->raw(), mrd.dssd.front.pass);
    }

    mrd.dssd.front.func = DepthStencilState::Comparison::ALWAYS; {
      const sjson::String* func =
        (const sjson::String*)root->find("stencil.front.func");
      if (func && func->is_string())
        comparison_from_string(func->raw(), mrd.dssd.front.func);
    }

    mrd.dssd.back.fail = DepthStencilState::StencilOp::KEEP; {
      const sjson::String* op =
        (const sjson::String*)root->find("stencil.back.fail");
      if (op && op->is_string())
        stencil_op_from_string(op->raw(), mrd.dssd.back.fail);
    }

    mrd.dssd.back.depth_fail = DepthStencilState::StencilOp::KEEP; {
      const sjson::String* op =
        (const sjson::String*)root->find("stencil.back.depth");
      if (op && op->is_string())
        stencil_op_from_string(op->raw(), mrd.dssd.back.depth_fail);
    }

    mrd.dssd.back.pass = DepthStencilState::StencilOp::KEEP; {
      const sjson::String* op =
        (const sjson::String*)root->find("stencil.back.pass");
      if (op && op->is_string())
        stencil_op_from_string(op->raw(), mrd.dssd.back.pass);
    }

    mrd.dssd.back.func = DepthStencilState::Comparison::ALWAYS; {
      const sjson::String* func =
        (const sjson::String*)root->find("stencil.back.func");
      if (func && func->is_string())
        comparison_from_string(func->raw(), mrd.dssd.back.func);
    }

    mrd.bsd.enabled = false; {
      const sjson::Boolean* enabled =
        (const sjson::Boolean*)root->find("blending.enabled");
      if (enabled && enabled->is_boolean())
        mrd.bsd.enabled = enabled->value();
    }

    mrd.bsd.color.src = BlendState::Blend::ONE; {
      const sjson::String* blend =
        (const sjson::String*)root->find("blending.color.src");
      if (blend && blend->is_string())
        blend_from_string(blend->raw(), mrd.bsd.color.src);
    }

    mrd.bsd.color.dest = BlendState::Blend::ZERO; {
      const sjson::String* blend =
        (const sjson::String*)root->find("blending.color.dest");
      if (blend && blend->is_string())
        blend_from_string(blend->raw(), mrd.bsd.color.dest);
    }

    mrd.bsd.color.op = BlendState::BlendOp::ADD; {
      const sjson::String* op =
        (const sjson::String*)root->find("blending.color.op");
      if (op && op->is_string())
        blend_op_from_string(op->raw(), mrd.bsd.color.op);
    }

    mrd.bsd.alpha.src = BlendState::Blend::ONE; {
      const sjson::String* blend =
        (const sjson::String*)root->find("blending.alpha.src");
      if (blend && blend->is_string())
        blend_from_string(blend->raw(), mrd.bsd.alpha.src);
    }

    mrd.bsd.alpha.dest = BlendState::Blend::ZERO; {
      const sjson::String* blend =
        (const sjson::String*)root->find("blending.alpha.dest");
      if (blend && blend->is_string())
        blend_from_string(blend->raw(), mrd.bsd.alpha.dest);
    }

    mrd.bsd.alpha.op = BlendState::BlendOp::ADD; {
      const sjson::String* op =
        (const sjson::String*)root->find("blending.alpha.op");
      if (op && op->is_string())
        blend_op_from_string(op->raw(), mrd.bsd.alpha.op);
    }

    if (!File::write_out(output.memory_resident_data, (const void*)&mrd, sizeof(MemoryResidentData)))
      return false;

    return true;
  }
} // butane
