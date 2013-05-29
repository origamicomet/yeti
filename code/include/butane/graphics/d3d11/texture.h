// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_TEXTURE_H_
#define _BUTANE_D3D11_TEXTURE_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/texture.h>

namespace butane {
  class BUTANE_EXPORT D3D11Texture final
    : public Texture
  {
    __foundation_trait(D3D11Texture, non_copyable);

    public:
      void destroy() override;
  };
} // butane

#endif // _BUTANE_D3D11_TEXTURE_H_
