//===-- yeti/resource.cc ----------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/resource.h"

#include "yeti/resource_database.h"
#include "yeti/resource_manager.h"

namespace yeti {

Resource::Type::Id Resource::type_from_id(Resource::Id id) {
  return (Type::Id)(id >> 32ull);
}

u32 Resource::hash_from_id(Resource::Id id) {
  return (u32)(id & 0xFFFFFFFFull);
}

Resource::Id Resource::id_from_path(const char *path) {
  yeti_assert_debug(path != NULL);

  const char *ext = foundation::path::extension(path);
  yeti_assert_debug(ext != NULL);

  const Resource::Type *type = resource_manager::type_from_ext(ext);
  yeti_assert_debug(type != NULL);

  char name[256] = { 0, };
  const size_t name_len = ext - path - 1;
  yeti_assert_debug(name_len <= 255);
  strncpy(&name[0], path, name_len);

  return Resource::id_from_type_and_name(resource_manager::id_from_type(type), &name[0]);
}

Resource::Id Resource::id_from_type_and_name(Resource::Type::Id type,
                                             const char *name) {
  yeti_assert_debug(name != NULL);
  return Resource::id_from_type_and_hash(type, foundation::murmur_hash_32(name, 0));
}

Resource::Id Resource::id_from_type_and_hash(Resource::Type::Id type,
                                             u32 hash) {
  return (Id)(((u64)type << 32ull) | (u64)hash);
}

Resource::Resource(Resource::Id id)
  : id_(id)
{
  foundation::atomic::store(&refs_, 0);
  foundation::atomic::store(&state_, UNLOADED);
}

Resource::~Resource() {
}

void Resource::ref() {
  foundation::atomic::add(&refs_, 1);
}

void Resource::deref() {
  const u32 refs = foundation::atomic::sub(&refs_, 1) - 1;

  // TODO(mtwilliams): Measure if |refs > 0| is most likely.
  if (YETI_LIKELY(refs > 0))
    // Still in use somewhere.
    return;

  // Queue for unload.
  resource_manager::unload(this);
}

Resource::State Resource::state() const {
  return (Resource::State)foundation::atomic::load(&state_);
}

void Resource::set_state(Resource::State state) {
  foundation::atomic::store(&state_, (u32)state);
}

Resource::Handle::Handle(Resource *resource) {
  yeti_assert_debug(resource != NULL);
  resource_ = resource;
  resource->ref();
}

Resource::Handle::Handle(const Resource::Handle &hndl) {
  yeti_assert_debug(hndl.resource_ != NULL);
  hndl.resource_->ref();
  resource_ = hndl.resource_;
}

Resource::Handle &Resource::Handle::operator=(const Resource::Handle &hndl) {
  yeti_assert_debug(resource_ != NULL);
  resource_->deref();
  yeti_assert_debug(hndl.resource_ != NULL);
  hndl.resource_->ref();
  resource_ = hndl.resource_;
  return *this;
}

Resource::Handle::~Handle() {
  resource_->deref();
}

} // yeti
