//===-- yeti/foundation/uuid.cc -------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/uuid.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace foundation {

UUID::UUID() {
  this->generate(this);
}

UUID::UUID(const UUID &uuid) {
  memcpy((void *)raw, (const void *)uuid.raw, 16);
}

UUID &UUID::operator=(const UUID &uuid) {
  memmove((void *)raw, (const void *)uuid.raw, 16);
  return *this;
}

void UUID::generate(UUID *uuid) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // TODO(mtwilliams): Verify byte order.
  GUID guid;
  ::CoCreateGuid((GUID *)&guid);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // foundation
} // yeti
