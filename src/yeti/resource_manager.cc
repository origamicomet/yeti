//===-- yeti/resource_manager.cc ------------------------*- mode: C++11 -*-===//
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
    // Indicates if loading and unloading is available.
    static bool enabled_ = false;

    static const size_t queue_memory_ = 1 * 1024 * 1024;

    static const size_t load_queue_memory_    = (queue_memory_ * 1) / 4;
    static const size_t unload_queue_memory_  = (queue_memory_ * 1) / 4;
    static const size_t online_queue_memory_  = (queue_memory_ * 1) / 4;
    static const size_t offline_queue_memory_ = (queue_memory_ * 1) / 4;

    core::Array<const Resource::Type *> types_(core::global_heap_allocator());

    core::ReaderWriterLock lock_;

    // Signaled whenever some work is added to one of the four queues.
    core::Event work_to_be_done_;

    core::Map<Resource::Id, Resource *> resources_(core::global_heap_allocator(), 65535);

    core::Queue<Resource *> to_be_loaded_(core::global_heap_allocator(), load_queue_memory_);
    core::Queue<Resource *> to_be_unloaded_(core::global_heap_allocator(), unload_queue_memory_);

    core::Queue<Resource *> to_be_brought_online_(core::global_heap_allocator(), online_queue_memory_);
    core::Queue<Resource *> to_be_put_offline_(core::global_heap_allocator(), offline_queue_memory_);
  }

  static void management_thread(void *);
}

Resource::Type::Id resource_manager::id_from_type(const Resource::Type *type) {
  yeti_assert_debug(type != NULL);
  return (Resource::Type::Id)core::murmur_hash_32(type->name, 0);
}

const Resource::Type *resource_manager::type_from_id(Resource::Type::Id id) {
  for (const Resource::Type **type = types_.begin(); type != types_.end(); ++type)
    if (id == core::murmur_hash_32((*type)->name, 0))
      return *type;

  return NULL;
}

const Resource::Type *resource_manager::type_from_name(const char *name) {
  yeti_assert_debug(name != NULL);
  return type_from_id(core::murmur_hash_32(name, 0));
}

const Resource::Type *resource_manager::type_from_path(const char *path) {
  yeti_assert_development(path != NULL);
  return type_from_ext(core::path::extension(path));
}

const Resource::Type *resource_manager::type_from_ext(const char *ext) {
  yeti_assert_development(ext != NULL);

  for (const Resource::Type **type = types_.begin(); type != types_.end(); ++type)
    for (const char **applicable_ext = &(*type)->extensions[0]; *applicable_ext; ++applicable_ext)
      if (strcmp(*applicable_ext, ext) == 0)
        return *type;

  return NULL;
}

void resource_manager::initialize(const Config &config) {
  yeti_assert_debug(!enabled_);

  core::Thread::Options management_thread_opts;
  management_thread_opts.name = "Resource Management";
  management_thread_opts.affinity = ~0ull;
  management_thread_opts.stack = 0x100000 /* 1MiB */;

  core::Thread::spawn(&resource_manager::management_thread, 0, management_thread_opts)->detach();

  enabled_ = true;
}

void resource_manager::shutdown() {
}

// Of course we can't call this "register" because that's a reserved keyword.
void resource_manager::track(const Resource::Type *type) {
  yeti_assert_debug(type != NULL);

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  // Make sure `type->name` is indeed unique.
  for (const Resource::Type **I = types_.begin(); I != types_.end(); ++I)
    if (strcmp((*I)->name, type->name) == 0)
      yeti_assert_with_reason(0, "A resource type with the name '%s' is already registered!", type->name);

  // Make sure `type->extensions` do not overlap.
  for (const Resource::Type **I = types_.begin(); I != types_.end(); ++I)
    for (const char **E = &type->extensions[0]; *E; ++E)
      for (const char **ext = &(*I)->extensions[0]; *ext; ++ext)
        if (strcmp(*E, *ext) == 0)
          yeti_assert_with_reason(0, "The resource type '%s' already registered the file extension '%s'!", (*I)->name, *E);
#endif

  types_.push(type);
}

Resource *resource_manager::find(Resource::Id id) {
  yeti_assert_debug(enabled_);

  {
    YETI_SCOPED_LOCK_NON_EXCLUSIVE(lock_);

    if (Resource **resource = resources_.find(id)) {
      (*resource)->ref();
      return (*resource);
    }
  }

  return NULL;
}

Resource *resource_manager::load(Resource::Id id) {
  yeti_assert_debug(enabled_);

  {
    YETI_SCOPED_LOCK_NON_EXCLUSIVE(lock_);

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
    YETI_SCOPED_LOCK_EXCLUSIVE(lock_);

    resources_.insert(id, resource);
    to_be_loaded_.push(resource);
  }

  work_to_be_done_.signal();

  return resource;
}

void resource_manager::unload(Resource *resource) {
  yeti_assert_debug(enabled_);
  yeti_assert_debug(resource != NULL);
  yeti_assert_development(resource->refs() == 0);

  {
    YETI_SCOPED_LOCK_EXCLUSIVE(lock_);

    resources_.remove(resource->id());
    to_be_unloaded_.push(resource);
  }

  work_to_be_done_.signal();
}

void resource_manager::management_thread(void *) {
  for (;;) {
    work_to_be_done_.wait();

    {
      // PERF(mtwilliams): Collect work to be done to an internal "queue" then
      // release `lock_` so as not to block other threads when loading,
      // unloading, onlining, or offlining.

      YETI_SCOPED_LOCK_EXCLUSIVE(lock_);

      Resource *resource;

      while (to_be_loaded_.pop(&resource)) {
        const Resource::Type *type = type_from_id(Resource::type_from_id(resource->id()));
        yeti_assert_debug(type != NULL);

        Resource::Data data;

        char memory_resident_data_path[256] = { 0, };
        sprintf(&memory_resident_data_path[0], "data/%016llx", resource->id());
        data.memory_resident_data = core::fs::open(&memory_resident_data_path[0], core::File::READ);

        // HACK(mtwilliams): Assume every resource has memory resident data.
        yeti_assert_debug(data.memory_resident_data != NULL);

        char streaming_data_path[256] = { 0, };
        sprintf(&streaming_data_path[0], "data/%016llx.streaming", resource->id());
        data.streaming_data = core::fs::open(&streaming_data_path[0], core::File::READ);

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
