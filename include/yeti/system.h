//===-- yeti/system.h -----------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_SYSTEM_H_
#define _YETI_SYSTEM_H_

#include "yeti/core.h"

// Inescapably coupled to components.
#include "yeti/component.h"

namespace yeti {

class YETI_PUBLIC SystemManager {
 YETI_DISALLOW_COPYING(SystemManager)

 public:
  SystemManager();
  ~SystemManager();

 public:
  /// Returns the system of type @T.
  ///
  /// \warning This isn't efficient. You should cache the returned system.
  ///
  template <typename T>
  T *get() {
    const char *name = T::component()->name;
    const Component::Id id = component::id_from_name(name);
    return (T *)this->get(id);
  }

 private:
  /// \internal Returns the system managing instances of type @T.
  void *get(Component::Id id);

 private:
  typedef core::Map<Component::Id,
                    void *,
                    core::map::IdentityHashFunction<u32>::hash
                   > Systems;

  // Systems managing instances of components.
  Systems systems_;

  // Components with systems under management.
  core::Array<const Component *> components_;
};

} // yeti

#endif // _YETI_SYSTEM_H_
