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

  static const Resource::Type& __type_initializer() {
    static const Resource::Type type(
      "shader", "shader",
      (Resource::Type::Load)&ShaderResource::load,
      (Resource::Type::Unload)&ShaderResource::unload,
      (Resource::Type::Compile)&ShaderResource::compile);
    return type;
  }

  static const thread_safe::Static< const Resource::Type >
    __ts_type(&__type_initializer);

  const Resource::Type& ShaderResource::type() {
    return __ts_type();
  }

  ShaderResource::ShaderResource(
    const Resource::Id id
  ) : butane::Resource(ShaderResource::type(), id)
    , _layers(allocator())
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

    const MemoryResidentData* mrd =
      ((const MemoryResidentData*)stream.memory_resident_data());

    ShaderResource* shader =
      make_new(ShaderResource, allocator())(id);

    shader->_layers.resize(mrd->num_of_layers);

    copy(
      (void*)shader->_layers.raw(),
      (const void*)&mrd->layers[0],
      mrd->num_of_layers * sizeof(RenderConfig::Layer::Name));

    shader->_state = mrd->state;
    shader->_vertex_shader = mrd->vertex_shader;
    shader->_pixel_shader = mrd->pixel_shader;

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
      (const char*)File::read(input.data, Allocators::heap(), &sjson_len);

    Array<uint8_t> blob(Allocators::scratch(), 1 << 12 /* 4kb */);
    blob.resize(blob.reserved());
    zero((void*)blob.raw(), blob.size());
    sjson::Parser parser(Allocators::heap(), (void*)&blob[0], blob.size());

    if (!parser.parse(sjson, sjson_len)) {
      Allocators::heap().free((void*)sjson);
      output.log("Malformed sjson; parsing failed.");
      return false; }

    Allocators::heap().free((void*)sjson);

    const sjson::Object* root =
      (const sjson::Object*)&blob[0];

    const sjson::String* layer =
      (const sjson::String*)root->find("layer");
    const sjson::Array* layers =
      (const sjson::Array*)root->find("layers");
    if (!layer && !layers) {
      output.log("Malformed input: 'layer' or 'layers' not specified!");
      return false; }
    if (layer && layers) {
      output.log("Malformed input: expected 'layer' or 'layers', not both!");
      return false; }
    if (layer && !layer->is_string()) {
      output.log("Malformed input: 'layer' is not a string!");
      return false; }
    if (layers && !layers->is_array()) {
      output.log("Malformed input 'layers' is not an array!");
      return false; }

    const size_t mrd_len =
      sizeof(MemoryResidentData) +
      (layers ? layers->size() : 1) * sizeof(RenderConfig::Layer::Name);

    MemoryResidentData* mrd =
      (MemoryResidentData*)Allocators::heap().alloc(mrd_len);

    {
      int64_t offset = sizeof(MemoryResidentData);
      mrd->layers = relative_ptr<RenderConfig::Layer::Name*>(offset - offsetof(MemoryResidentData, layers));
    }

    /* layers = */ {
      mrd->num_of_layers = (layers ? layers->size() : 1);
      if (layer) {
        mrd->layers[0] = RenderConfig::Layer::Name(layer->raw());
      } else {
        for (size_t i = 0; i < layers->size(); ++i) {
          const sjson::String* layer = (const sjson::String*)layers->at(i);
          if (!layer->is_string()) {
            output.log("Malformed input: the %u layer in 'layers' is not a string!", i);
            goto failure; }
          mrd->layers[i] = RenderConfig::Layer::Name(layer->raw());
        }
      }
    }

    /* state = */ {
      const sjson::String* state =
        (const sjson::String*)root->find("state");
      if (!state || !state->is_string()) {
        output.log("Malformed input: 'state' not specified!");
        goto failure; }
      mrd->state = Resource::Id(StateResource::type(), state->raw());
    }

    /* vertex shader = */ {
      const sjson::String* vs =
        (const sjson::String*)root->find("vertex");
      if (!vs || !vs->is_string()) {
        output.log("Malformed input: 'vertex' not specified!");
        goto failure; }
      mrd->vertex_shader = Resource::Id(VertexShader::type(), vs->raw());
    }

    /* pixel shader = */ {
      const sjson::String* ps =
        (const sjson::String*)root->find("pixel");
      if (!ps || !ps->is_string()) {
        output.log("Malformed input: 'pixel' not specified!");
        goto failure; }
      mrd->pixel_shader = Resource::Id(PixelShader::type(), ps->raw());
    }

    if (!File::write(output.memory_resident_data, (const void*)mrd, mrd_len)) {
      output.log("Unable to write memory-resident data!");
      goto failure; }

    Allocators::heap().free((void*)mrd);
    return true;

  failure:
    Allocators::heap().free((void*)mrd);
    return false;
  }
} // butane
