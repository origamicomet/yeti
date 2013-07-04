// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_BLEND_STATE_H_
#define _BUTANE_BLEND_STATE_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT BlendState abstract {
    __foundation_trait(BlendState, non_copyable);

    public:
      class Blend {
        __foundation_trait(Blend, non_constructable);
        __foundation_trait(Blend, non_copyable);

        public:
          enum Value {
            ZERO           = 1,
            ONE            = 2,
            SRC_COLOR      = 3,
            INV_SRC_COLOR  = 4,
            SRC_ALPHA      = 5,
            INV_SRC_ALPHA  = 6,
            DEST_ALPHA     = 7,
            INV_DEST_ALPHA = 8,
            DEST_COLOR     = 9,
            INV_DEST_COLOR = 10,
            SRC_ALPHA_SAT  = 11
          };
      };

      class BlendOp {
        __foundation_trait(BlendOp, non_constructable);
        __foundation_trait(BlendOp, non_copyable);

        public:
          enum Value {
            ADD          = 1,
            SUBTRACT     = 2,
            REV_SUBTRACT = 3,
            MIN          = 4,
            MAX          = 5
          };
      };

      struct Desc {
        bool enabled;
        struct {
          Blend::Value src;
          Blend::Value dest;
          BlendOp::Value op;
        } color;
        struct {
          Blend::Value src;
          Blend::Value dest;
          BlendOp::Value op;
        } alpha;
      };

    protected:
      BlendState();
      virtual ~BlendState();

    public:
      static BlendState* create(
        const Desc& desc );

      virtual void destroy() = 0;
  };
} // butane

#endif // _BUTANE_BLEND_STATE_H_
