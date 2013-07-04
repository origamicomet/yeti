// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RASTERIZER_STATE_H_
#define _BUTANE_RASTERIZER_STATE_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT RasterizerState abstract {
    __foundation_trait(RasterizerState, non_copyable);

    public:
      class Fill {
        __foundation_trait(Fill, non_constructable);
        __foundation_trait(Fill, non_copyable);

        public:
          enum Value {
            SOLID     = 0,
            WIREFRAME = 1
          };
      };

      class Cull {
        __foundation_trait(Cull, non_constructable);
        __foundation_trait(Cull, non_copyable);

        public:
          enum Value {
            NONE  = 0,
            FRONT = 1,
            BACK  = 2
          };
      };

      struct Desc {
        Fill::Value fill;
        Cull::Value cull;
        bool scissor;
      };

    protected:
      RasterizerState();
      virtual ~RasterizerState();

    public:
      static RasterizerState* create(
        const Desc& desc );

      virtual void destroy() = 0;
  };
} // butane

#endif // _BUTANE_RASTERIZER_STATE_H_
