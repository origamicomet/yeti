// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_VERTEX_BUFFER_H_
#define _BUTANE_VERTEX_BUFFER_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT VertexBuffer {
    __foundation_trait(VertexBuffer, non_copyable);

    protected:
      VertexBuffer();
      virtual ~VertexBuffer();

    public:
      static VertexBuffer* create(
        const void* data,
        size_t data_len,
        bool immutable = true );

      virtual void destroy() = 0;

    public:
      virtual void update(
        const void* data,
        const size_t data_len ) = 0;
  };
} // butane

#endif // _BUTANE_VERTEX_BUFFER_H_
