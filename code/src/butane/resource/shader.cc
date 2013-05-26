// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource/shader.h>

#include <butane/resource/vertex_shader.h>
#include <butane/resource/pixel_shader.h>

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
    ShaderResource* shader = make_new(ShaderResource, allocator())(id);

    uintptr_t mrd = (uintptr_t)stream.memory_resident_data();

    copy((void*)&shader->_layer, (const void*)mrd, sizeof(shader->_layer));
    mrd += sizeof(shader->_layer);

    copy((void*)&shader->_vertex_shader, (const void*)mrd, sizeof(shader->_vertex_shader));
    mrd += sizeof(shader->_vertex_shader);

    copy((void*)&shader->_pixel_shader, (const void*)mrd, sizeof(shader->_pixel_shader));
    mrd += sizeof(shader->_pixel_shader);

    return shader;
  }

  void ShaderResource::unload(
    ShaderResource* shader )
  {
    assert(shader != nullptr);
    make_delete(ShaderResource, allocator(), shader);
  }

  bool ShaderResource::compile(
    const Resource::Compiler::Source& src,
    const Resource::Compiler::Stream& cs )
  {
    size_t sjson_len = 0;
    const char* sjson =
      (const char*)File::read_in(cs.source_data(), allocator(), &sjson_len);

    Array<uint8_t> blob(Allocators::scratch(), 1 << 12 /* 4kb */);
    blob.resize(blob.reserved());
    zero((void*)blob.raw(), blob.size());
    sjson::Parser parser(allocator(), (void*)&blob[0], blob.size());

    if (!parser.parse(sjson, sjson_len)) {
      allocator().free((void*)sjson);
      return false; }

    allocator().free((void*)sjson);

    const sjson::Object* root =
      (const sjson::Object*)&blob[0];

    /* layer = */ {
      const sjson::String* layer =
        (const sjson::String*)root->find("layer");
      if (!layer || !layer->is_string()) {
        log("Layer not specified!");
        return false; }
      const ImmutableHash<uint32_t, murmur_hash> name(layer->raw());
      if (!File::write_out(cs.memory_resident_data(), (void*)&name, sizeof(name)))
        return false;
    }

    /* vertex shader = */ {
      const sjson::String* vs =
        (const sjson::String*)root->find("vertex");
      if (!vs || !vs->is_string()) {
        log("Vertex shader not specified!");
        return false; }
      const Resource::Id id(VertexShader::type, vs->raw());
      if (!File::write_out(cs.memory_resident_data(), (void*)&id, sizeof(id)))
        return false;
    }

    /* pixel shader = */ {
      const sjson::String* ps =
        (const sjson::String*)root->find("pixel");
      if (!ps || !ps->is_string()) {
        log("Pixel shader not specified!");
        return false; }
      const Resource::Id id(VertexShader::type, ps->raw());
      if (!File::write_out(cs.memory_resident_data(), (void*)&id, sizeof(id)))
        return false;
    }

    return true;
  }
} // butane
