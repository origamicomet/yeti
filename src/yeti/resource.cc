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

namespace resource {
  namespace {
    static core::Array<Resource::Type::Id> id_for_types_(core::global_heap_allocator());
    static core::Array<const Resource::Type *> types_(core::global_heap_allocator());
    static core::Map<const char *, const Resource::Type *> extension_to_type_(core::global_heap_allocator(), 256);
  }

  namespace identifier {
    union Componentized {
      struct {
        u32 type;
        u32 name;
      };

      Resource::Id id;
    };

    static YETI_INLINE Resource::Id compose(u32 type, u32 name) {
      Componentized componentized;
      componentized.type = type;
      componentized.name = name;
      return componentized.id;
    }

    static YETI_INLINE Resource::Id decompose(Resource::Id id, u32 *type, u32 *name) {
      Componentized componentized;

      componentized.id = id;

      *type = componentized.type;
      *name = componentized.name;
    }

    static YETI_INLINE u32 type(Resource::Id id) {
      Componentized componentized;
      componentized.id = id;
      return componentized.type;
    }

    static YETI_INLINE u32 name(Resource::Id id) {
      Componentized componentized;
      componentized.id = id;
      return componentized.name;
    }
  }
}

Resource::Type::Id resource::register_a_type(const Resource::Type *type) {
  yeti_assert_debug(type != NULL);

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  // Make sure `type->name` is indeed unique.
  for (const Resource::Type **I = types_.begin(); I != types_.end(); ++I)
    if (strcmp((*I)->name, type->name) == 0)
      yeti_assert_with_reason(0, "Already registered a type with the name '%s'!", type->name);

  // Make sure `type->extensions` do not overlap.
  for (const Resource::Type **I = types_.begin(); I != types_.end(); ++I)
    for (const char **E = &type->extensions[0]; *E; ++E)
      for (const char **ext = &(*I)->extensions[0]; *ext; ++ext)
        if (strcmp(*E, *ext) == 0)
          yeti_assert_with_reason(0, "The the file extension '%s' is already associated with '%s'!", *E, (*I)->name);
#endif

  const Resource::Type::Id id =
    (Resource::Type::Id)core::murmur_hash_32(type->name);

  id_for_types_.push(id);
  types_.push(type);

  for (const char **ext = &type->extensions[0]; *ext; ++ext)
    extension_to_type_.insert(*ext, type);

  return id;
}

const Resource::Type *resource::type_from_name(const char *name) {
  yeti_assert_debug(name != NULL);

  const Resource::Type::Id id = (Resource::Type::Id)core::murmur_hash_32(name);

  return type_from_id(id);
}

const Resource::Type *resource::type_from_path(const char *path) {
  yeti_assert_debug(path != NULL);

  if (const char *extension = core::path::extension(path))
    return type_from_extension(extension);

  // No file extension.
  return NULL;
}

const Resource::Type *resource::type_from_extension(const char *extension) {
  yeti_assert_debug(extension != NULL);

  if (const Resource::Type **type = extension_to_type_.find(extension))
    return *type;

  // Not associated with any type.
  return NULL;
}

Resource::Type::Id resource::id_from_type(const Resource::Type *type) {
  yeti_assert_debug(type != NULL);

  unsigned index = 0;

  for (const Resource::Type **I = types_.begin(); I != types_.end(); ++I, ++index)
    if (*I == type)
      return id_for_types_[index];

  // Not registered.
  return NULL;
}

const Resource::Type *resource::type_from_id(Resource::Type::Id id) {
  yeti_assert_debug(id != Resource::Type::INVALID);

  unsigned index = 0;

  for (const Resource::Type::Id *I = id_for_types_.begin(); I != id_for_types_.end(); ++I, ++index)
    if (*I == id)
      return types_[index];

  // Not registered.
  return NULL;
}

Resource::Id resource::id_from_path(const char *path) {
  yeti_assert_debug(path != NULL);

  const char *extension = core::path::extension(path);

  if (extension == NULL)
    return INVALID;

  const Resource::Type *type = type_from_extension(extension);

  if (type == NULL)
    return INVALID;

  const u32 name = core::murmur_hash_32((const void *)path, extension - path);

  return identifier::compose(core::murmur_hash_32(type->name), name);
}

Resource::Id resource::id_from_name(Resource::Type::Id type, const char *name) {
  yeti_assert_debug(type != Resource::Type::INVALID);
  yeti_assert_debug(name != NULL);

  return identifier::compose(type, core::murmur_hash_32(name));
}

void resource::type_and_name_from_id(Resource::Id id, const Resource::Type **type, u32 *name) {
  yeti_assert_debug(id != INVALID);

  *type = type_from_id(identifier::type(id));
  *name = identifier::name(id);
}

void resource::for_each_type(void (*callback)(Resource::Type::Id id, const Resource::Type *type, void *context),
                             void *context) {
  for (unsigned index = 0; index < types_.size(); ++index)
    callback(id_for_types_[index], types_[index], context);
}

Resource::Resource(Resource::Id id)
  : id_(id)
{
  atomic::store(&references_, 0);
}

Resource::~Resource() {
}

void Resource::ref() const {
  atomic::increment(&references_);
}

void Resource::deref() const {
  const u32 references = atomic::decrement(&references_);

  if (YETI_LIKELY(references > 0))
    // Still in use somewhere.
    return;

  // Queue for unload.
  resource_manager::unload((Resource *)this);
}

Resource::Handle::Handle(Resource *resource) {
  yeti_assert_debug(resource != NULL);

  resource_ = resource;
  resource->ref();
}

Resource::Handle::Handle(const Resource::Handle &hndl) {
  yeti_assert_debug(hndl.resource_ != NULL);

  resource_ = hndl.resource_;
  hndl.resource_->ref();
}

Resource::Handle &Resource::Handle::operator=(const Resource::Handle &hndl) {
  yeti_assert_debug(resource_ != NULL);
  yeti_assert_debug(hndl.resource_ != NULL);

  hndl.resource_->ref();

  // Dereference our exisiting resource after referencing the new resource,
  // just in case both handles refer to the same resource.
  resource_->deref();

  // Then replace.
  resource_ = hndl.resource_;

  return *this;
}

Resource::Handle::~Handle() {
  resource_->deref();
}

} // yeti
