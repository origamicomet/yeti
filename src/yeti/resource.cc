//===-- yeti/resource.cc --------------------------------*- mode: C++11 -*-===//
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
#include "yeti/resource_manager.h"
#include "yeti/resource_database.h"

namespace yeti {

Resource::Id Resource::id_from_path(const char *path) {
  ResourceDatabase *db = resource_manager::database();
  return db->id_from_path(path);
}

Resource::Id Resource::id_from_type_and_name(Resource::Type::Id type,
                                             const char *name) {
  ResourceDatabase *db = resource_manager::database();
  return db->id_from_name(type, name);
}

Resource::Type::Id Resource::type_from_id(Resource::Id id) {
  ResourceDatabase *db = resource_manager::database();
  return db->type_from_id(id);
}

Resource::Resource(Resource::Id id)
  : id_(id)
{
  atomic::store(&refs_, 0);
  atomic::store(&state_, UNLOADED);
}

Resource::~Resource() {
}

void Resource::ref() {
  atomic::increment(&refs_);
}

void Resource::deref() {
  const u32 refs = atomic::decrement(&refs_);

  if (YETI_LIKELY(refs > 0))
    // Still in use somewhere.
    return;

  // Queue for unload.
  resource_manager::unload(this);
}

Resource::State Resource::state() const {
  return (Resource::State)atomic::load(&state_);
}

void Resource::set_state(Resource::State state) {
  atomic::store(&state_, (u32)state);
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
