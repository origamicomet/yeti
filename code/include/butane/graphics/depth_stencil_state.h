// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_DEPTH_STENCIL_STATE_H_
#define _BUTANE_DEPTH_STENCIL_STATE_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT DepthStencilState abstract {
    __foundation_trait(DepthStencilState, non_copyable);

    public:
      class Comparison {
        __foundation_trait(Comparison, non_constructable);
        __foundation_trait(Comparison, non_copyable);

        public:
          enum Value {
            NEVER         = 1,
            LESS          = 2,
            EQUAL         = 3,
            LESS_EQUAL    = 4,
            GREATER       = 5,
            NOT_EQUAL     = 6,
            GREATER_EQUAL = 7,
            ALWAYS        = 8
          };
      };

      class StencilOp {
        __foundation_trait(StencilOp, non_constructable);
        __foundation_trait(StencilOp, non_copyable);

        public:
          enum Value {
            KEEP     = 1,
            ZERO     = 2,
            REPLACE  = 3,
            INCR_SAT = 4,
            DECR_SAT = 5,
            INVERT   = 6,
            INCR     = 7,
            DECR     = 8
          };
      };

      struct Desc {
        bool tests;
        bool writes;
        Comparison::Value func;
        bool stencil;
        struct {
          StencilOp::Value fail;
          StencilOp::Value depth_fail;
          StencilOp::Value pass;
          Comparison::Value func;
        } front;
        struct {
          StencilOp::Value fail;
          StencilOp::Value depth_fail;
          StencilOp::Value pass;
          Comparison::Value func;
        } back;
      };

    protected:
      DepthStencilState();
      virtual ~DepthStencilState();

    public:
      static DepthStencilState* create(
        const Desc& desc );

      virtual void destroy() = 0;
  };
} // butane

#endif // _BUTANE_DEPTH_STENCIL_STATE_H_
