// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SAMPLER_H_
#define _BUTANE_SAMPLER_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Sampler abstract {
    __foundation_trait(Sampler, non_copyable);

    public:
      class Filter {
        __foundation_trait(Filter, non_constructable);
        __foundation_trait(Filter, non_copyable);

        public:
          enum Value {
            MIN_MAG_MIP_POINT               = 0,
            MIN_MAG_POINT_MIP_LINEAR        = 1,
            MIN_POINT_MAG_LINEAR_MIP_POINT  = 2,
            MIN_POINT_MAG_MIP_LINEAR        = 3,
            MIN_LINEAR_MAG_MIP_POINT        = 4,
            MIN_LINEAR_MAG_POINT_MIP_LINEAR = 5,
            MIN_MAG_LINEAR_MIP_POINT        = 6,
            MIN_MAG_MIP_LINEAR              = 7,
            ANISOTROPIC                     = 8
          };
      };

      class TextureAddressingMode {
        __foundation_trait(TextureAddressingMode, non_constructable);
        __foundation_trait(TextureAddressingMode, non_copyable);

        public:
          enum Value {
            WRAP        = 1,
            MIRROR      = 2,
            CLAMP       = 3,
            BORDER      = 4,
            MIRROR_ONCE = 5
          };
      };

    public:
      struct Desc {
        Filter::Value filter;
        TextureAddressingMode::Value uvw[3];
      };

    protected:
      Sampler();
      virtual ~Sampler();

    public:
      static Sampler* create(
        const Desc& desc );

      virtual void destroy() = 0;
  };
} // butane

#endif // _BUTANE_SAMPLER_H_
