// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TIED_RESOURCES_H_
#define _BUTANE_TIED_RESOURCES_H_

#include <butane/butane.h>

namespace butane {
  class SwapChain;

  /*! */
  class BUTANE_EXPORT TiedResources final {
    __foundation_trait(TiedResources, non_copyable);

    private:
      /*! */
      static Allocator& allocator();

    private:
      TiedResources(
        SwapChain* swap_chain );

      ~TiedResources();

    public:
      /*! */
      static TiedResources* create(
        SwapChain* swap_chain );

      /*! */
      void create_or_update_global_resources();

      /*! */
      void destroy_global_resources();

      /*! */
      void destroy();

    public:
      FOUNDATION_INLINE SwapChain* swap_chain() const
      { return _swap_chain; }

      FOUNDATION_INLINE Array<void*>& globals()
      { return _globals; }

      FOUNDATION_INLINE const Array<void*>& globals() const
      { return _globals; }

    private:
      SwapChain* _swap_chain;
      Array<void*> _globals;
  };
} // butane

#endif // _BUTANE_TIED_RESOURCES_H_
