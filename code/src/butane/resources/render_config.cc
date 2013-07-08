// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/render_config.h>

#include <butane/math/vec2.h>
#include <butane/math/vec3.h>
#include <butane/math/vec4.h>
#include <butane/math/quat.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("render config resources", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  static const Resource::Type& __type_initializer() {
    static const Resource::Type type(
      "render_config", "render_config",
      (Resource::Type::Load)&RenderConfigResource::load,
      (Resource::Type::Unload)&RenderConfigResource::unload,
      (Resource::Type::Compile)&RenderConfigResource::compile);
    return type;
  }

  static const thread_safe::Static< const Resource::Type >
    __ts_type(&__type_initializer);

  const Resource::Type& RenderConfigResource::type() {
    return __ts_type();
  }

  RenderConfigResource::RenderConfigResource(
    const butane::Resource::Id id
  ) : butane::Resource(RenderConfigResource::type(), id)
    , _globals(allocator())
    , _layers(allocator())
    , _name_to_layer(allocator(), 256)
  {
  }

  RenderConfigResource::~RenderConfigResource()
  {
  }

  RenderConfigResource* RenderConfigResource::load(
    const butane::Resource::Id id,
    const butane::Resource::Stream& stream )
  {
    const LogScope _("RenderConfigResource::load");

    const MemoryResidentData* mrd =
      ((const MemoryResidentData*)stream.memory_resident_data());

    RenderConfigResource* cfg =
      make_new(RenderConfigResource, allocator())(id);

    cfg->_globals.resize(mrd->num_of_globals);
    cfg->_layers.resize(mrd->num_of_layers);

    copy(
      (void*)cfg->_globals.raw(),
      (const void*)mrd->globals,
      mrd->num_of_globals * sizeof(Resource));

    copy(
      (void*)cfg->_layers.raw(),
      (const void*)mrd->layers,
      mrd->num_of_layers * sizeof(Layer));

    for (size_t i = 0; i < mrd->num_of_layers; ++i)
      cfg->_name_to_layer.insert(cfg->_layers[i].name, &cfg->_layers[i]);

    return cfg;
  }

  void RenderConfigResource::unload(
    RenderConfigResource* render_config )
  {
    const LogScope _("RenderConfigResource::unload");

    assert(render_config != nullptr);
    make_delete(RenderConfigResource, allocator(), render_config);
  }

  static bool resource_type_from_string(
    const char* str,
    RenderConfigResource::Resource::Type& type )
  {
    if (strcmp("render_target", str) == 0) {
      type = RenderConfigResource::Resource::RENDER_TARGET;
      return true;
    } else if (strcmp("depth_stencil_target", str) == 0) {
      type = RenderConfigResource::Resource::DEPTH_STENCIL_TARGET;
      return true; }
    return false;
  }

  bool RenderConfigResource::compile(
    const butane::Resource::Compiler::Input& input,
    const butane::Resource::Compiler::Output& output )
  {
    const LogScope _("RenderConfigResource::compile");

    size_t sjson_len = 0;
    const char* sjson =
      (const char*)File::read(input.data, Allocators::heap(), &sjson_len);

    Array<uint8_t> blob(Allocators::scratch(), 1 << 13 /* 8kb */);
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

    const sjson::Array* globals =
      (const sjson::Array*)root->find("resources.globals");
    if (!globals) {
      output.log("Malformed input: 'resources.globals' does not exist!");
      return false; }
    if (!globals->is_array()) {
      output.log("Malformed input: expected 'resources.globals' to be an array!");
      return false; }

    const sjson::Array* layers =
      (const sjson::Array*)root->find("layers");
    if (!layers) {
      output.log("Malformed input: 'layers' does not exist!");
      return false; }
    if (!layers->is_array()) {
      output.log("Malformed input: expected 'layers' to be an array!");
      return false; }

    const size_t mrd_len =
      sizeof(MemoryResidentData) +
      globals->size() * sizeof(Resource) +
      layers->size() * sizeof(Layer);

    MemoryResidentData* mrd =
      (MemoryResidentData*)Allocators::scratch().alloc(mrd_len);

    {
      int64_t offset = sizeof(MemoryResidentData);

      if (globals->size() > 0)
        mrd->globals = relative_ptr<Resource*>(offset - offsetof(MemoryResidentData, globals));
      else
        mrd->globals = relative_ptr<Resource*>();

      offset += globals->size() * sizeof(Resource);

      if (layers->size() > 0)
        mrd->layers = relative_ptr<Layer*>(offset - offsetof(MemoryResidentData, layers));
      else
        mrd->layers = relative_ptr<Layer*>();

      // offset += layers->size() * sizeof(Layer);
    }

    mrd->num_of_globals = globals->size();
    for (size_t id = 0; id < globals->size(); ++id) {
      Resource& sr = mrd->globals[id];
      const sjson::Object* resource = (const sjson::Object*)globals->at(id);
      if (!resource->is_object()) {
        output.log("Malformed input: the global resource %u not an object!", id);
        goto failure; }

      sr.id = (Resource::Id)id;

      /* sr.name = */ {
        const sjson::String* value = (const sjson::String*)resource->find("name");
        if (!value) {
          output.log("Malformed input: the global resource %u is missing 'name'!", id);
          goto failure; }
        if (!value->is_string()) {
          output.log("Malformed input: 'name' in the global resource %u is not a string!", id);
          goto failure; }
        sr.name = value->raw();
      }

      /* sr.type = */ {
        const sjson::String* value = (const sjson::String*)resource->find("type");
        if (!value) {
          output.log("Malformed input: the global resource %u is missing 'type'!", id);
          goto failure; }
        if (!value->is_string()) {
          output.log("Malformed input: 'type' in the global resource %u is not a string!", id);
          goto failure; }
        if (!resource_type_from_string(value->raw(), sr.type)) {
          output.log("Malformed input: the type '%s' in the global resource %u is not recognized!", value->raw(), id);
          goto failure; }
      }

      switch (sr.type) {
        case Resource::RENDER_TARGET:
        case Resource::DEPTH_STENCIL_TARGET: {
          /* sr.render_or_depth_stencil_target.scale = */ {
            const sjson::Array* value = (const sjson::Array*)resource->find("scale");
            if (!value) {
              output.log("Malformed input: the global resource %u is missing 'scale'!", id);
              goto failure; }
            if (!value->is_array() || (value->size() != 2) || (!value->at(0)->is_number()) || (!value->at(1)->is_number())) {
              output.log("Malformed input: 'scale' in the global resource %u is not a vector!", id);
              goto failure; }
            sr.render_or_depth_stencil_target.scale.x =
              ((const sjson::Number*)value->at(0))->value();
            sr.render_or_depth_stencil_target.scale.y =
              ((const sjson::Number*)value->at(1))->value();
          }

          /* sr.render_or_depth_stencil_target.format = */ {
            const sjson::String* value = (const sjson::String*)resource->find("format");
            if (!value) {
              output.log("Malformed input: the global resource %u is missing 'format'!", id);
              goto failure; }
            if (!value->is_string()) {
              output.log("Malformed input: 'format' in the global resource %u is not a string!", id);
              goto failure; }
            sr.render_or_depth_stencil_target.format = PixelFormat::from_str(value->raw());
            if (sr.render_or_depth_stencil_target.format == PixelFormat::INVALID) {
              output.log("Malformed input: the format '%s' in the global resource %u is not recognized!", value->raw(), id);
              goto failure; }
          }

          /* sr.render_or_depth_stencil_target.clear = */ {
            const sjson::Boolean* value = (const sjson::Boolean*)resource->find("clear");
            if (value) {
              if (!value->is_boolean()) {
                output.log("Malformed input: 'clear' in the global resource %u is not a boolean!", id);
                goto failure; }
              sr.render_or_depth_stencil_target.clear = value->value();
            } else {
              sr.render_or_depth_stencil_target.clear = false;
            }
          }
        } break;
      }
    }

    mrd->num_of_layers = layers->size();
    for (size_t id = 0; id < layers->size(); ++id) {
      Layer& sl = mrd->layers[id];
      const sjson::Object* layer = (const sjson::Object*)layers->at(id);
      if (!layer->is_object()) {
        output.log("Malformed input: the layer %u not an object!", id);
        goto failure; }

      sl.id = (Layer::Id)id;

      /* sl.name = */ {
        const sjson::String* value = (const sjson::String*)layer->find("name");
        if (!value) {
          output.log("Malformed input: the layer %u is missing 'name'!", id);
          goto failure; }
        if (!value->is_string()) {
          output.log("Malformed input: 'name' in the layer %u is not a string!", id);
          goto failure; }
        sl.name = value->raw();
      }

      /* sl.samplers = */ {
        const sjson::Array* samplers =
          (const sjson::Array*)layer->find("samplers");
        if (samplers) {
          if (!samplers->is_array()) {
            output.log("Malformed input: 'samplers' in the layer %u is not an array!", id);
            goto failure; }
          if (samplers->size() > Layer::maximum_num_of_samplers) {
            output.log("Malformed input: expected less than %u samplers in the layer %u!", Layer::maximum_num_of_samplers, id);
            goto failure; }
          sl.num_of_samplers = samplers->size();
          for (size_t sampler = 0; sampler < samplers->size(); ++sampler) {
            const sjson::String* value = (const sjson::String*)samplers->at(sampler);
            if (!value->is_string()) {
              output.log("Malformed input: the sampler %u in the layer %u is not a string!", sampler, id);
              goto failure; }
            const Resource::Name name = value->raw();
            for (size_t i = 0; i < mrd->num_of_globals; ++i) {
              if (name != mrd->globals[i].name)
                continue;
              switch (mrd->globals[i].type) {
                case Resource::RENDER_TARGET:
                case Resource::DEPTH_STENCIL_TARGET:
                  break;
                default:
                  output.log("Malformed input: the sampler %u in the layer %u references a non-samplable resource!", sampler, id);
                  goto failure; }
              sl.samplers[sampler] = mrd->globals[i].id;
              goto found_sampler; }
            output.log("Malformed input: the sampler %u in the layer %u references an undefined global resource `%s`!", sampler, id, value->raw());
            goto failure;
          found_sampler:
            continue;
          }
        } else {
          sl.num_of_samplers = 0;
        }
      }

      /* sl.render_targets = */ {
        const sjson::Array* render_targets =
          (const sjson::Array*)layer->find("render_targets");
        if (render_targets) {
          if (!render_targets->is_array()) {
            output.log("Malformed input: 'render_targets' in the layer %u is not an array!", id);
            goto failure; }
          if (render_targets->size() > Layer::maximum_num_of_render_targets) {
            output.log("Malformed input: expected less than %u render targets in the layer %u!", Layer::maximum_num_of_render_targets, id);
            goto failure; }
          sl.num_of_render_targets = render_targets->size();
          for (size_t render_target = 0; render_target < render_targets->size(); ++render_target) {
            const sjson::String* value = (const sjson::String*)render_targets->at(render_target);
            if (!value->is_string()) {
              output.log("Malformed input: the render target %u in the layer %u is not a string!", render_target, id);
              goto failure; }
            const Resource::Name name = value->raw();
            static const Resource::Name back_buffer = "back_buffer";
            if (name == back_buffer) {
              sl.render_targets[render_target] = Resource::back_buffer;
              goto found_render_target; }
            for (size_t i = 0; i < mrd->num_of_globals; ++i) {
              if (name != mrd->globals[i].name)
                continue;
              if (mrd->globals[i].type != Resource::RENDER_TARGET) {
                output.log("Malformed input: the render target %u in the layer %u does not reference a render target!", render_target, id);
                goto failure; }
              sl.render_targets[render_target] = mrd->globals[i].id;
              goto found_render_target; }
            output.log("Malformed input: the render target %u in the layer %u references an undefined global resource `%s`!", render_target, id, value->raw());
            goto failure;
          found_render_target:
            continue;
          }
        } else {
          sl.num_of_render_targets = 0;
        }
      }

      /* sl.depth_stencil_target = */ {
        const sjson::String* depth_stencil_target =
          (const sjson::String*)layer->find("depth_stencil_target");
        if (depth_stencil_target) {
          if (!depth_stencil_target->is_string()) {
            output.log("Malformed input: the depth-stencil target in the layer %u is not a string!", id);
            goto failure; }
          const Resource::Name name = depth_stencil_target->raw();
          for (size_t i = 0; i < mrd->num_of_globals; ++i) {
            if (name != mrd->globals[i].name)
              continue;
            if (mrd->globals[i].type != Resource::DEPTH_STENCIL_TARGET) {
              output.log("Malformed input: the depth-stencil target in the layer %u does not reference a depth-stencil target!", layer);
              goto failure; }
            sl.depth_stencil_target = mrd->globals[i].id;
            goto found_depth_stencil_target; }
          output.log("Malformed input: the depth-stencil target in the layer %u references an undefined global resource `%s`!", id, depth_stencil_target->raw());
          goto failure;
        found_depth_stencil_target:
          __builtin_nop();
        } else {
          sl.depth_stencil_target = Resource::invalid;
        }
      }

      /* sl.generator = */ {
      }
    }

    if (!File::write(output.memory_resident_data, (void*)mrd, mrd_len)) {
      output.log("Unable to write to memory-resident data!");
      goto failure; }

    Allocators::scratch().free((void*)mrd);
    return true;

  failure:
    Allocators::scratch().free((void*)mrd);
    return false;
  }
} // butane
