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
  namespace {
    static const size_t queue_mem_sz_ = 131072;

    static const size_t load_queue_mem_sz_    = queue_mem_sz_ * 1/3;
    static const size_t unload_queue_mem_sz_  = queue_mem_sz_ * 1/3;
    static const size_t online_queue_mem_sz_  = queue_mem_sz_ * 1/6;
    static const size_t offline_queue_mem_sz_ = queue_mem_sz_ * 1/6;

    static u8 queue_mem_[queue_mem_sz_] = { 0, };

    static u8 *load_queue_mem_ = &queue_mem_[0];
    static u8 *unload_queue_mem_ = &load_queue_mem_[load_queue_mem_sz_];
    static u8 *online_queue_mem_ = &unload_queue_mem_[unload_queue_mem_sz_];
    static u8 *offline_queue_mem_ = &online_queue_mem_[online_queue_mem_sz_];

    foundation::Array<const Resource::Type *> types_(foundation::heap());

    foundation::ReadersWriterLock *resources_lock_ = foundation::ReadersWriterLock::create();

    // TODO(mtwilliams): Replace with condition variable.
    // Signaled whenever some work is added to one of the four queues.
    foundation::Event *work_to_be_done_ = foundation::Event::create();

    foundation::HashMap<Resource::Id, Resource *> resources_(foundation::heap(), 65535);

    foundation::Queue<Resource *> to_be_loaded_((uintptr_t)load_queue_mem_, load_queue_mem_sz_);
    foundation::Queue<Resource *> to_be_unloaded_((uintptr_t)unload_queue_mem_, unload_queue_mem_sz_);

    foundation::Queue<Resource *> to_be_brought_online_((uintptr_t)online_queue_mem_, online_queue_mem_sz_);
    foundation::Queue<Resource *> to_be_put_offline_((uintptr_t)offline_queue_mem_, offline_queue_mem_sz_);
  }

  static void management_thread(uintptr_t /* unused */);
}

Resource::Type::Id resource_manager::id_from_type(const Resource::Type *type) {
  yeti_assert_debug(type != NULL);
  return (Resource::Type::Id)foundation::murmur_hash_32(type->name, 0);
}

const Resource::Type *resource_manager::type_from_id(Resource::Type::Id id) {
  for (const Resource::Type **type = types_.first(); type <= types_.last(); ++type)
    if (id == foundation::murmur_hash_32((*type)->name, 0))
      return *type;

  return NULL;
}

const Resource::Type *resource_manager::type_from_name(const char *name) {
  yeti_assert_debug(name != NULL);
  return type_from_id(foundation::murmur_hash_32(name, 0));
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
  foundation::Thread::Options management_thread_opts;
  sprintf(&management_thread_opts.name[0], "Resource Management");
  management_thread_opts.affinity = ~0ull;
  management_thread_opts.stack_size = 0x100000 /* 1MiB */;

  foundation::Thread::spawn(&resource_manager::management_thread, 0, &management_thread_opts)->detach();
}

void resource_manager::shutdown() {
}

// Of course we can't call this "register" because that's a reserved keyword.
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
  {
    YETI_SCOPED_LOCK_NON_EXCLUSIVE(resources_lock_);

    if (Resource **resource = resources_.find(id)) {
      (*resource)->ref();
      return (*resource);
    }
  }

  const Resource::Type *type = type_from_id(Resource::type_from_id(id));
  yeti_assert_development(type != NULL);

  Resource *resource = type->prepare(id);
  yeti_assert_debug(resource != NULL);

  {
    YETI_SCOPED_LOCK_EXCLUSIVE(resources_lock_);

    resources_.insert(id, resource);
    to_be_loaded_.push(resource);
  }

  work_to_be_done_->signal();

  return resource;
}

void resource_manager::unload(Resource *resource) {
  yeti_assert_debug(resource != NULL);
  yeti_assert_development(resource->refs() == 0);

  {
    YETI_SCOPED_LOCK_EXCLUSIVE(resources_lock_);

    resources_.remove(resource->id());
    to_be_unloaded_.push(resource);
  }

  work_to_be_done_->signal();
}

void resource_manager::management_thread(uintptr_t) {
  for (;;) {
    work_to_be_done_->wait();

    {
      // TODO(mtwilliams): Collect work to be done to an internal "queue" then
      // release |resources_lock_| so as not to block other threads when loading,
      // unloading, onlining, or offlining.

      YETI_SCOPED_LOCK_EXCLUSIVE(resources_lock_);

      Resource *resource;

      while (to_be_loaded_.pop(&resource)) {
        const Resource::Type *type = type_from_id(Resource::type_from_id(resource->id()));
        yeti_assert_debug(type != NULL);

        Resource::Data data;

        char memory_resident_data_path[256] = { 0, };
        sprintf(&memory_resident_data_path[0], "data/%016llx", resource->id());
        data.memory_resident_data = foundation::fs::open(&memory_resident_data_path[0], foundation::fs::READ);

        // HACK(mtwilliams): Assume every resource has memory resident data.
        yeti_assert_debug(data.memory_resident_data != NULL);

        char streaming_data_path[256] = { 0, };
        sprintf(&streaming_data_path[0], "data/%016llx.streaming", resource->id());
        data.streaming_data = foundation::fs::open(&streaming_data_path[0], foundation::fs::READ);

        type->load(resource, data);

        resource->set_state(Resource::LOADED);
      }

      while (to_be_unloaded_.pop(&resource)) {
        const Resource::Type *type = type_from_id(Resource::type_from_id(resource->id()));
        yeti_assert_debug(type != NULL);

        type->unload(resource);
      }
    }
  }
}

} // yeti
