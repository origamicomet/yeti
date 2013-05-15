// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
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
    const LogScope log_scope("Resource::Load");
    return type.load(Stream(type, id));
  }

  void Resource::unload()
  {
    _resource_type.unload(this);
  }

  void Resource::reference()
  {
    __sync_fetch_and_add(&_resource_refs, 1);
  }

  void Resource::dereference()
  {
    if (__sync_fetch_and_sub(&_resource_refs, 1) <= 0)
      mark_for_unload();
  }

  static thread_safe::Queue<Resource*>& __marked_for_unload() {
    static thread_safe::Queue<Resource*> marked_for_unload(
      Allocators::heap(), BUTANE_BACKGROUND_RESOURCE_UNLOADING_QUEUE_SIZE);
    return marked_for_unload;
  }

  static Thread::Return __background_unloading_thread(
    Thread& thread,
    void* closure )
  {
    while (true) {
      Resource* resource = nullptr;
      __marked_for_unload().dequeue(resource);
      resource->unload();
    }
    
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
    __marked_for_unload().enqueue(this);
  }
} // butane
