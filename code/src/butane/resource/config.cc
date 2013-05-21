// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource/config.h>

#include <butane/math.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("resources (config)", Allocators::heap());
    return allocator;
  }

  const Resource::Type ConfigResource::type(
    "config", "config",
    (Resource::Type::Load)&ConfigResource::load,
    (Resource::Type::Unload)&ConfigResource::unload,
    (Resource::Type::Compile)&ConfigResource::compile);

  ConfigResource::ConfigResource(
    const Resource::Id id
  ) : butane::Resource(ConfigResource::type, id)
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
    ConfigResource* cfg = make_new(ConfigResource, allocator())(id);
    const sjson::Object* root =
      (const sjson::Object*)stream.memory_resident_data();
    cfg->_root =
      (const sjson::Object*)allocator().alloc(stream.memory_resident_data_len());
    copy((void*)cfg->_root, (const void*)root, stream.memory_resident_data_len());
    return cfg;
  }

  void ConfigResource::unload(
    ConfigResource* config )
  {
    assert(config != nullptr);
    make_delete(ConfigResource, allocator(), config);
  }

  bool ConfigResource::compile(
    const Resource::Compiler::Stream& cs )
  {
    const char* sjson =
      (const char*)File::read_in(cs.source_data(), allocator());

    if (!sjson)
      return false;

    Array<uint8_t> blob(Allocators::scratch(), 1 << 13 /* 8kb */);
    blob.resize(blob.reserved());
    sjson::Parser parser(allocator(), (void*)&blob[0], blob.size());

    if (!parser.parse(sjson)) {
      allocator().free((void*)sjson);
      return false;
    }

    if (!File::write_out(cs.memory_resident_data(), (void*)&blob[0], blob.size())) {
      allocator().free((void*)sjson);
      return false;
    }
    
    allocator().free((void*)sjson);
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
  bool ConfigResource::find<Quat>(
    const char* name,
    Quat& value )
  {
    assert(name != nullptr);
    const sjson::Array* obj = (const sjson::Array*)_root->find(name);
    const bool valid = (obj && (obj->size() == 4) &&
      obj->at(0)->is_number() && obj->at(1)->is_number() &&
      obj->at(2)->is_number() && obj->at(3)->is_number());
    if (valid) {
      value = Quat(
        ((const sjson::Number*)obj->at(0))->value(),
        ((const sjson::Number*)obj->at(1))->value(),
        ((const sjson::Number*)obj->at(2))->value(),
        ((const sjson::Number*)obj->at(3))->value()); }
    return valid;
  }
} // butane
