// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  Resource::Resource(
    const Resource::Type& type,
    const Resource::Id id
  ) : _resource_id(id)
    , _resource_type(type)
    , _resource_refs(0)
  {
  }

  Resource::~Resource()
  {
  }

  Resource* Resource::find(
    const Type& type,
    const char* path )
  {
    assert(path != nullptr);
    return Resource::find(type, Id(type, path));
  }

  Resource* Resource::find(
    const Type& type,
    Id id )
  {
    const LogScope log_scope("Resource::find");
    Resource* resource;
    if (Resources::loaded().find(id, resource) && resource) {
      resource->reference();
      return resource; }
    return nullptr;
  }

  Resource* Resource::load(
    const Type& type,
    const char* path )
  {
    assert(path != nullptr);
    return Resource::load(type, Id(type, path));
  }

  Resource* Resource::load(
    const Type& type,
    Id id )
  {
    const LogScope log_scope("Resource::load");
    Resource* resource;
    if (Resources::loaded().find(id, resource) && resource) {
      resource->reference();
      return resource; }
    resource = type.load(id, Stream(type, id));
    if (!Resources::loaded().insert(id, resource))
      fail("Unable to marked resource as loaded!");
    resource->reference();
    return resource;
  }

  void Resource::unload()
  {
    const LogScope log_scope("Resource::unload");
    Resources::loaded().remove(id());
    _resource_type.unload(this);
  }

  void Resource::reference()
  {
    __sync_fetch_and_add(&_resource_refs, 1);
  }

  void Resource::dereference()
  {
    if (__sync_fetch_and_sub(&_resource_refs, 1) <= 1)
      mark_for_unload();
  }

  static thread_safe::Queue<Resource*>& __marked_for_unload_initializer() {
    static thread_safe::Queue<Resource*> marked_for_unload(
      Allocators::heap(), BUTANE_BACKGROUND_RESOURCE_UNLOADING_QUEUE_SIZE);
    return marked_for_unload;
  }

  static const thread_safe::Static< thread_safe::Queue<Resource*> >
    __ts_marked_for_unload(&__marked_for_unload_initializer);

  static Thread::Return __background_unloading_thread(
    void* closure )
  {
    while (true) {
      Resource* resource = nullptr;
      __ts_marked_for_unload().dequeue(resource);
      resource->unload(); }

    __builtin_unreachable();
    return 0;
  }

  static void __start_background_unloading_thread()
  {
    static bool has_started = false;
    if (has_started) return;
    static Thread background_unloading_thread(&__background_unloading_thread);
    background_unloading_thread.detach();
    has_started = true;
  }

  void Resource::mark_for_unload()
  {
    // This is so a dereference() doesn't shit on performance sensitive code
    // if it holds the last reference, i.e., if a mesh is being rendered but
    // it is reloaded then all references to the old instance are replaced with
    // the new one, and this is unloaded (in a background thread) when it is
    // finally rendered.

    __start_background_unloading_thread();
    __ts_marked_for_unload().enqueue(this);
  }

  Resource::Variantion Resource::determine_variation_based_on_properties(
    const Array<Resource::Property>& properties )
  {
    if (properties.empty())
      return 0;
    return murmur_hash((void*)properties.raw(), properties.size() * sizeof(Resource::Property), 0);
  }
} // butane

namespace butane {
  namespace Resources {
    static HashTable<Resource::Id, Resource*>& __loaded_initializer() {
      static HashTable<Resource::Id, Resource*> loaded(
        Allocators::heap(), BUTANE_LOADED_RESOURCES_HASH_TABLE_INITAL_SIZE);
      return loaded;
    }

    static const thread_safe::Static< HashTable<Resource::Id, Resource*> >
      __ts_loaded(&__loaded_initializer);

    HashTable<Resource::Id, Resource*>& loaded() {
      return __ts_loaded();
    }
  } // Resources
} // butane
