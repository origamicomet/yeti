// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/render_device.h>

#include <butane/graphics/swap_chain.h>
#include <butane/graphics/render_target.h>
#include <butane/graphics/depth_stencil_target.h>

namespace butane {
  static Allocator* __allocator_initializer() {
    return new ProxyAllocator("render device", Allocators::heap());
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  Allocator& RenderDevice::allocator() {
    return __ts_allocator();
  }

  RenderDevice::RenderDevice()
  {
  }

  RenderDevice::~RenderDevice()
  {
  }

  Task::Affinity RenderDevice::affinity()
  {
    return ((Task::Affinity)1u << (Task::Scheduler::num_of_worker_treads() - 1));
  }
} // butane
