// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/shader.h>

#include <butane/graphics/vertex_shader.h>
#include <butane/graphics/pixel_shader.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("shader resources", Allocators::heap());
    return allocator;
  }

  const Resource::Type ShaderResource::type(
    "shader", "shader",
    (Resource::Type::Load)&ShaderResource::load,
    (Resource::Type::Unload)&ShaderResource::unload,
    (Resource::Type::Compile)&ShaderResource::compile);

  ShaderResource::ShaderResource(
    const Resource::Id id
  ) : butane::Resource(ShaderResource::type, id)
  {
  }

  ShaderResource::~ShaderResource()
  {
  }

  ShaderResource* ShaderResource::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    const LogScope _("ShaderResource::load");

    const MemoryResidentData& mrd =
      *((const MemoryResidentData*)stream.memory_resident_data());

    ShaderResource* shader =
      make_new(ShaderResource, allocator())(id);

    shader->_layer = mrd.layer;
    shader->_state = mrd.state;
    shader->_vertex_shader = mrd.vertex_shader;
    shader->_pixel_shader = mrd.pixel_shader;

    return shader;
  }

  void ShaderResource::unload(
    ShaderResource* shader )
  {
    const LogScope _("ShaderResource::unload");

    assert(shader != nullptr);
    make_delete(ShaderResource, allocator(), shader);
  }

  bool ShaderResource::compile(
    const Resource::Compiler::Input& input,
    const Resource::Compiler::Output& output )
  {
    const LogScope _("ShaderResource::compile");

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

    /* layer = */ {
      const sjson::String* layer =
        (const sjson::String*)root->find("layer");
      if (!layer || !layer->is_string()) {
        log("Layer not specified!");
        return false; }
      mrd.layer = Hash<uint32_t, murmur_hash>(layer->raw());
    }

    /* state = */ {
      const sjson::String* state =
        (const sjson::String*)root->find("state");
      if (!state || !state->is_string()) {
        log("State not specified!");
        return false; }
      mrd.state = Resource::Id(StateResource::type, state->raw());
    }

    /* vertex shader = */ {
      const sjson::String* vs =
        (const sjson::String*)root->find("vertex");
      if (!vs || !vs->is_string()) {
        log("Vertex shader not specified!");
        return false; }
      mrd.vertex_shader = Resource::Id(VertexShader::type, vs->raw());
    }

    /* pixel shader = */ {
      const sjson::String* ps =
        (const sjson::String*)root->find("pixel");
      if (!ps || !ps->is_string()) {
        log("Pixel shader not specified!");
        return false; }
      mrd.pixel_shader = Resource::Id(PixelShader::type, ps->raw());
    }

    if (!File::write_out(output.memory_resident_data, (const void*)&mrd, sizeof(MemoryResidentData)))
      return false;

    return true;
  }
} // butane
