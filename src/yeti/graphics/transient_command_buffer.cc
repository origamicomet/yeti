//===-- yeti/graphics/transient_command_buffer.cc -------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/graphics/transient_command_buffer.h"

// Loosely coupled to `engine::submit`.
#include "yeti/graphics/engine.h"

namespace yeti {
namespace graphics {

// SMELL(mtwilliams): Drop namespace imports.

// OPTIMIZE(mtwilliams): We dedicate ~4MiB to transient command buffers. This
// may be excessive. We should determine a reasonable upper limit.

namespace transient_command_buffers {
  // The maximum number of blocks that can be in use at any point in time.
  static const size_t MAX = 64;

  // A chunk of memory.
  struct Block {
    static const size_t SIZE = 65535 /* 64KiB */;
    size_t occupied;
    u8 memory[SIZE];
  };

  static foundation::thread_safe::Pool<Block> blocks_(foundation::heap(), MAX);
}

static bool _forward_on_exhaustion(::gala_command_buffer_t *command_buffer) {
  TransientCommandBuffer *transient_command_buffer =
    (TransientCommandBuffer *)command_buffer;
  return transient_command_buffer->exhausted_();
}

static void _release_after_submission(void *context) {
  transient_command_buffers::Block *block =
    (transient_command_buffers::Block *)context;
  block->occupied = 0;
  transient_command_buffers::blocks_.release(block);
}

TransientCommandBuffer::TransientCommandBuffer() {
  // Only acquire a block upon the first write.
  start = end = current = NULL;
  exhausted = &_forward_on_exhaustion;
  block_ = NULL;
}

TransientCommandBuffer::~TransientCommandBuffer() {
  this->submit();
}

void TransientCommandBuffer::submit(bool force) {
  using namespace transient_command_buffers;

  if (block_ == NULL)
    return;

  Block *block = (Block *)block_;

  block->occupied = current - start;

  if (force)
    engine::submit(this, &_release_after_submission, (void *)block);
  else
    blocks_.release(block);

  start = end = current = NULL;
  block_ = NULL;
}

bool TransientCommandBuffer::exhausted_() {
  using namespace transient_command_buffers;

  this->submit(true);

  if (Block *block = blocks_.acquire()) {
    start = (uintptr_t)&block->memory[0];
    end = start + Block::SIZE;
    current = start + block->occupied;
    block_ = (void *)block;
    return true;
  }

  return false;
}

} // graphics
} // yeti
