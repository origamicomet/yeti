//===-- yeti/resource_manager.cc --------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/resource_manager.h"

// TODO(mtwilliams): Dynamically associate file extensions to resource types.

namespace yeti {

namespace resource_manager {
  // foundation::Map<Resource::Id, Resource *> resources_(foundation::heap());
  foundation::Array<const Resource::Type *> types_(foundation::heap());
  // foundation::Queue<Resource *> to_be_loaded_(foundation::heap());
  // foundation::Queue<Resource *> to_be_unloaded_(foundation::heap());
  // foundation::Queue<Resource *> to_be_onlined_(foundation::heap());
  // foundation::Queue<Resource *> to_be_offlined_(foundation::heap());
}

Resource::Type::Id resource_manager::id_from_type(const Resource::Type *type) {
  yeti_assert_debug(type != NULL);
  return (Resource::Type::Id)foundation::murmur(type->name, 0);
}

const Resource::Type *resource_manager::type_from_id(Resource::Type::Id id) {
  for (const Resource::Type **type = types_.first(); type <= types_.last(); ++type)
    if (id == foundation::murmur((*type)->name, 0))
      return *type;

  return NULL;
}

const Resource::Type *resource_manager::type_from_path(const char *path) {
  yeti_assert_development(path != NULL);
  return type_from_ext(foundation::path::extension(path));
}

const Resource::Type *resource_manager::type_from_ext(const char *ext) {
  yeti_assert_development(ext != NULL);

  for (const Resource::Type **type = types_.first(); type <= types_.last(); ++type)
    for (const char **applicable_ext = &(*type)->extensions[0]; *applicable_ext; ++applicable_ext)
      if (strcmp(*applicable_ext, ext) == 0)
        return *type;

  return NULL;
}

void resource_manager::initialize() {
}

void resource_manager::shutdown() {
}

// Of course we can't call this 'register' because that's a reserved keyword in
// C and C++. Lovely language, right?
void resource_manager::track(const Resource::Type *type) {
  yeti_assert_debug(type != NULL);

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  // Make sure `type->name` is indeed unique.
  for (const Resource::Type **I = types_.first(); I <= types_.last(); ++I)
    if (strcmp((*I)->name, type->name) == 0)
      yeti_assertf(0, "A resource type with the name '%s' is already registered!", type->name);

  // Make sure `type->extensions` do not overlap.
  for (const Resource::Type **I = types_.first(); I <= types_.last(); ++I)
    for (const char **E = &type->extensions[0]; *E; ++E)
      for (const char **ext = &(*I)->extensions[0]; *ext; ++ext)
        if (strcmp(*E, *ext) == 0)
          yeti_assertf(0, "The resource type '%s' already registered the file extension '%s'!", (*I)->name, *E);
#endif

  types_.push(type);
}

Resource *resource_manager::load(Resource::Id id) {
  // if (Resource *resource = resources_.find(id)) {
  //   resource->ref();
  //   return resource;
  // }

  const Resource::Type *type = type_from_id(Resource::type_from_id(id));
  yeti_assert_development(type != NULL);

  yeti_assert_debug(type->prepare != NULL);
  Resource *resource = type->prepare(id);
  // resources_.insert(id, resource);
  // to_be_loaded_.push(resource);

  return resource;
}

void resource_manager::unload(Resource *resource) {
  yeti_assert_debug(resource != NULL);
  // to_be_unloaded_.push(resource);
}

} // yeti
