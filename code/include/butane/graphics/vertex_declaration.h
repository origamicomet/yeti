// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_VERTEX_DECLARATION_H_
#define _BUTANE_VERTEX_DECLARATION_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT VertexDeclaration final {
    public:
      enum Components {
        POSITION    = (1 << 0),
        COLOR0      = (1 << 1),
        COLOR1      = (1 << 2),
        COLOR2      = (1 << 3),
        COLOR3      = (1 << 4),
        COLOR4      = (1 << 5),
        COLOR5      = (1 << 6),
        COLOR6      = (1 << 7),
        COLOR7      = (1 << 8),
        TEXCOORD0   = (1 << 9),
        TEXCOORD1   = (1 << 10),
        TEXCOORD2   = (1 << 11),
        TEXCOORD3   = (1 << 12),
        TEXCOORD4   = (1 << 13),
        TEXCOORD5   = (1 << 14),
        TEXCOORD6   = (1 << 15),
        TEXCOORD7   = (1 << 16),
        NORMAL      = (1 << 17),
        TANGENT     = (1 << 18),
        BINORMAL    = (1 << 19),
        BONEINDICES = (1 << 20),
        BONEWEIGHTS = (1 << 21)
      };

    public:
      VertexDeclaration(
        const uint32_t components = 0
      ) : _components(components)
      {}

      VertexDeclaration(
        const VertexDeclaration& vd
      ) : _components(vd._components)
      {}

      VertexDeclaration& operator= (
        const VertexDeclaration& vd )
      { _components = vd._components; return *this; }

    public:
      FOUNDATION_INLINE operator uint32_t() const
      { return _components; }

    public:
      size_t size() const;

    public:
      FOUNDATION_INLINE uint32_t& components()
      { return _components; }

      FOUNDATION_INLINE const uint32_t& components() const
      { return _components; }

    private:
      uint32_t _components;
  };
} // butane

#endif // _BUTANE_VERTEX_DECLARATION_H_
