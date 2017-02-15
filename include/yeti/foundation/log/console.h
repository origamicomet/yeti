//===-- yeti/foundation/log/console.h ---------------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_LOG_CONSOLE_H_
#define _YETI_FOUNDATION_LOG_CONSOLE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/log.h"

namespace yeti {
namespace log {

namespace console {
  extern YETI_LOCAL void log(const Message *msg);
}

} // log
} // yeti

#endif // _YETI_FOUNDATION_LOG_CONSOLE_H_
