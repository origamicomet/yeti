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

#include "yeti/resource.h"
#include "yeti/resource_manager.h"
#include "yeti/resource_database.h"

namespace yeti {

namespace core {
namespace log {

static const core::log::Category::Id RESOURCE_MANAGER =
  Category::add("resource_manager", GENERAL);

} // log
} // core

namespace resource_manager {
  namespace {
    core::ReaderWriterLock lock_;

    ResourceDatabase *database_ = NULL;

    //
    bool autoload_ = false;

    // Signaled whenever some work is added to one of the four queues.
    core::Event work_to_be_done_;

    core::Map<Resource::Id, Resource *> resources_(core::global_heap_allocator(), 131072);
    core::Map<Resource::Id, Resource::State> states_(core::global_heap_allocator(), 131072);

    core::Queue<Resource *> to_be_loaded_(core::global_heap_allocator(), 16384);
    core::Queue<Resource *> to_be_unloaded_(core::global_heap_allocator(), 16384);

    core::Queue<Resource *> to_be_brought_online_(core::global_heap_allocator(), 16384);
    core::Queue<Resource *> to_be_brought_offline_(core::global_heap_allocator(), 16384);

    // Internal queues that the management thread copies the above queues to,
    // so as not to block other threads while loading and unloading resources.
    core::Queue<Resource *> loading_(core::global_heap_allocator(), 16384);
    core::Queue<Resource *> unloading_(core::global_heap_allocator(), 16384);

    // Additional internal queues that the management thread uses to defer
    // mutation of state.
    core::Queue<Resource *> loaded_(core::global_heap_allocator(), 16384);
    core::Queue<Resource *> unloaded_(core::global_heap_allocator(), 16384);
  }

  static void management_thread(void *);

  // Loads a resource.
  static void load_a_resource(Resource *resource);

  // Unloads a resource.
  static void unload_a_resource(Resource *resource);
}

ResourceDatabase *resource_manager::database() {
  return database_;
}

void resource_manager::initialize(const Config &config) {
  database_ = ResourceDatabase::open(config.database);

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  autoload_ = config.autoload;
#else
  autoload_ = false;
#endif

  if (autoload_)
    core::logf(core::log::RESOURCE_MANAGER, core::log::DEBUG, "Automatically loading resources on demand.");

  core::Thread::Options management_thread_options;
  management_thread_options.name = "Resource Management";
  management_thread_options.affinity = ~0ull;
  management_thread_options.stack = 0x100000 /* 1MiB */;

  core::Thread::spawn(&resource_manager::management_thread, 0, management_thread_options)->detach();
}

void resource_manager::shutdown() {
}

bool resource_manager::autoloads() {
  return autoload_;
}

bool resource_manager::available(Resource::Id id) {
  if (autoload_) {
    return database_->exists(id);
  } else {
    YETI_SCOPED_LOCK_NON_EXCLUSIVE(lock_);
    return !!resources_.find(id);
  }
}

Resource *resource_manager::lookup(Resource::Id id) {
  if (autoload_) {
    return load(id);
  } else {
    YETI_SCOPED_LOCK_NON_EXCLUSIVE(lock_);

    if (Resource **resource = resources_.find(id)) {
      (*resource)->ref();
      return (*resource);
    }

    return NULL;
  }
}

Resource *resource_manager::load(Resource::Id id) {
  lock_.acquire(true);

  if (Resource **resource = resources_.find(id)) {
    // Already loaded!
    (*resource)->ref();
    return (*resource);
  }

  const Resource::Type *type;
  u32 name;

  resource::type_and_name_from_id(id, &type, &name);

  // Allocate space up front to prevent blocking.
  Resource *resource = type->prepare(id);

  resource->ref();

  // Track it.
  resources_.insert(id, resource);
  states_.insert(id, resource::UNLOADED);

  // Queue for load.
  to_be_loaded_.push(resource);

  lock_.release(true);

  // Let management thread know there's a resource that needs to be loaded.
  work_to_be_done_.signal();

  return resource;
}

void resource_manager::reload(Resource::Id id) {
  // TODO(mtwilliams): Resource hot loading.
  YETI_TRAP();
}

void resource_manager::unload(Resource *resource) {
  const Resource::Id id = resource->id();

  lock_.acquire(true);

  resources_.remove(id);
  states_.remove(id);

  // Queue for unload.
  to_be_unloaded_.push(resource);

  lock_.release(true);

  // Let management thread know there's a resource that needs to be unloaded.
  work_to_be_done_.signal();
}

Resource::State resource_manager::state(Resource::Id id) {
  YETI_SCOPED_LOCK_NON_EXCLUSIVE(lock_);

  if (const Resource::State *state = states_.find(id))
    return *state;

  // TODO(mtwilliams): Commute to `UNLOADED`?
  return Resource::UNKNOWN;
}

Resource::State resource_manager::state(Resource *resource) {
  return state(resource->id());
}

void resource_manager::management_thread(void *) {
  Resource *resource;

  while (true) {
    // Wait until there's resources that need to be loaded or unloaded.
    work_to_be_done_.wait();

    // Copy work to be done to internal queues.
    {
      YETI_SCOPED_LOCK_EXCLUSIVE(lock_);

      while (to_be_loaded_.pop(&resource))
        loading_.push(resource);

      while (to_be_unloaded_.pop(&resource))
        unloading_.push(resource);
    }

    // Do the work.
    {
      while (loading_.pop(&resource)) {
        load_a_resource(resource);
        loaded_.push(resource);
      }

      while (unloading_.pop(&resource)) {
        unload_a_resource(resource);
        unloaded_.push(resource);
      }
    }

    // Notify other threads.
    {
      YETI_SCOPED_LOCK_EXCLUSIVE(lock_);

      while (loaded_.pop(&resource)) {
        *states_.find(resource->id()) = resource::LOADED;
      }

      while (loaded_.pop(&resource)) {
        resources_.remove(resource->id());
        states_.remove(resource->id());
      }
    }
  }
}

void resource_manager::load_a_resource(Resource *resource) {
  const Resource::Id id = resource->id();

  const Resource::Type *type;
  u32 name;

  type_and_name_from_id(id, &type, &name);

  Resource::Data data;

  char memory_resident_data_path[256] = { 0, };
  sprintf(&memory_resident_data_path[0], "data/%016llx", id);
  data.memory_resident_data = core::fs::open(&memory_resident_data_path[0], core::File::READ);

  char streaming_data_path[256] = { 0, };
  sprintf(&streaming_data_path[0], "data/%016llx.streaming", id);
  data.streaming_data = core::fs::open(&streaming_data_path[0], core::File::READ);

  type->load(resource, data);
}

void resource_manager::unload_a_resource(Resource *resource) {
  const Resource::Id id = resource->id();

  const Resource::Type *type;
  u32 name;

  type_and_name_from_id(id, &type, &name);

  type->unload(resource);
}

} // yeti
