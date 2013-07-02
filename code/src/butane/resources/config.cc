// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/config.h>

#include <butane/math/vec2.h>
#include <butane/math/vec3.h>
#include <butane/math/vec4.h>
#include <butane/math/quat.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("config resources", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  static const Resource::Type& __type_initializer() {
    static const Resource::Type type(
      "config", "config",
      (Resource::Type::Load)&ConfigResource::load,
      (Resource::Type::Unload)&ConfigResource::unload,
      (Resource::Type::Compile)&ConfigResource::compile);
    return type;
  }

  static const thread_safe::Static< const Resource::Type >
    __ts_type(&__type_initializer);

  const Resource::Type& ConfigResource::type() {
    return __ts_type();
  }

  ConfigResource::ConfigResource(
    const Resource::Id id
  ) : butane::Resource(ConfigResource::type(), id)
    , _root(nullptr)
  {
  }

  ConfigResource::~ConfigResource()
  {
    allocator().free((void*)_root);
  }

  ConfigResource* ConfigResource::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    const LogScope _("ConfigResource::load");

    ConfigResource* cfg =
      make_new(ConfigResource, allocator())(id);

    cfg->_root =
      (const sjson::Object*)allocator().alloc(stream.memory_resident_data_len());
    copy((void*)cfg->_root, stream.memory_resident_data(), stream.memory_resident_data_len());

    return cfg;
  }

  void ConfigResource::unload(
    ConfigResource* config )
  {
    const LogScope _("ConfigResource::unload");

    assert(config != nullptr);
    make_delete(ConfigResource, allocator(), config);
  }

  bool ConfigResource::compile(
    const Resource::Compiler::Input& input,
    const Resource::Compiler::Output& output )
  {
    const LogScope _("ConfigResource::compile");

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

    if (!File::write(output.memory_resident_data, (void*)&blob[0], blob.size())) {
      output.log("Unable to write to memory-resident data!");
      return false; }

    return true;
  }

  template <>
  bool ConfigResource::find<bool>(
    const char* name,
    bool& value )
  {
    assert(name != nullptr);
    const sjson::Boolean* obj = (const sjson::Boolean*)_root->find(name);
    if (obj && obj->is_boolean())
      value = obj->value();
    return (obj != nullptr);
  }

  template <>
  bool ConfigResource::find<double>(
    const char* name,
    double& value )
  {
    assert(name != nullptr);
    const sjson::Number* obj = (const sjson::Number*)_root->find(name);
    if (obj && obj->is_number())
      value = obj->value();
    return (obj != nullptr);
  }

  template <>
  bool ConfigResource::find<const char*>(
    const char* name,
    const char*& value )
  {
    assert(name != nullptr);
    const sjson::String* obj = (const sjson::String*)_root->find(name);
    if (obj && obj->is_string())
      value = obj->raw();
    return (obj != nullptr);
  }

  template <>
  bool ConfigResource::find<String>(
    const char* name,
    String& value )
  {
    assert(name != nullptr);
    const sjson::String* obj = (const sjson::String*)_root->find(name);
    if (obj && obj->is_string())
      value = obj->raw();
    return (obj != nullptr);
  }

  template <>
  bool ConfigResource::find<Vec2f>(
    const char* name,
    Vec2f& value )
  {
    assert(name != nullptr);
    const sjson::Array* obj = (const sjson::Array*)_root->find(name);
    const bool valid = (obj && (obj->size() == 2) &&
      obj->at(0)->is_number() && obj->at(1)->is_number());
    if (valid) {
      value = Vec2f(
        ((const sjson::Number*)obj->at(0))->value(),
        ((const sjson::Number*)obj->at(1))->value()); }
    return valid;
  }

  template <>
  bool ConfigResource::find<Vec3f>(
    const char* name,
    Vec3f& value )
  {
    assert(name != nullptr);
    const sjson::Array* obj = (const sjson::Array*)_root->find(name);
    const bool valid = (obj && (obj->size() == 3) &&
      obj->at(0)->is_number() && obj->at(1)->is_number() &&
      obj->at(2)->is_number());
    if (valid) {
      value = Vec3f(
        ((const sjson::Number*)obj->at(0))->value(),
        ((const sjson::Number*)obj->at(1))->value(),
        ((const sjson::Number*)obj->at(2))->value()); }
    return valid;
  }

  template <>
  bool ConfigResource::find<Vec4f>(
    const char* name,
    Vec4f& value )
  {
    assert(name != nullptr);
    const sjson::Array* obj = (const sjson::Array*)_root->find(name);
    const bool valid = (obj && (obj->size() == 4) &&
      obj->at(0)->is_number() && obj->at(1)->is_number() &&
      obj->at(2)->is_number() && obj->at(3)->is_number());
    if (valid) {
      value = Vec4f(
        ((const sjson::Number*)obj->at(0))->value(),
        ((const sjson::Number*)obj->at(1))->value(),
        ((const sjson::Number*)obj->at(2))->value(),
        ((const sjson::Number*)obj->at(3))->value()); }
    return valid;
  }

  template <>
  bool ConfigResource::find<Quatf>(
    const char* name,
    Quatf& value )
  {
    assert(name != nullptr);
    const sjson::Array* obj = (const sjson::Array*)_root->find(name);
    const bool valid = (obj && (obj->size() == 4) &&
      obj->at(0)->is_number() && obj->at(1)->is_number() &&
      obj->at(2)->is_number() && obj->at(3)->is_number());
    if (valid) {
      value = Quatf(
        ((const sjson::Number*)obj->at(0))->value(),
        ((const sjson::Number*)obj->at(1))->value(),
        ((const sjson::Number*)obj->at(2))->value(),
        ((const sjson::Number*)obj->at(3))->value()); }
    return valid;
  }
} // butane
