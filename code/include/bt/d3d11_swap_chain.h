// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_SWAP_CHAIN_H_
#define _BUTANE_D3D11_SWAP_CHAIN_H_

#include <bt/d3d11.h>
#include <bt/swap_chain.h>

namespace bt {
  class D3D11SwapChain
    : public SwapChain
  {
    friend class SwapChain;

    private:
      D3D11SwapChain();
      ~D3D11SwapChain();
      
    public:
      FOUNDATION_INLINE IDXGISwapChain* interface() const
      { return _interface; } 

    private:
      IDXGISwapChain* _interface;
  };
} // bt

#endif // _BUTANE_D3D11_SWAP_CHAIN_H_