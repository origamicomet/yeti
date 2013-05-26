// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/vertex_declaration.h>

namespace butane {
  size_t VertexDeclaration::size() const {
    static const size_t cs[32] = {
      12,
      16, 16, 16, 16, 16, 16, 16, 16,
      12, 12, 12, 12, 12, 12, 12, 12,
      12, 12, 12,
      4, 4,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    size_t size = 0;
    uint32_t components = _components;
    while (components) {
      size += cs[__builtin_ctz(components)];
      components &= (components - 1); }
    return size;
  }
} // butane
