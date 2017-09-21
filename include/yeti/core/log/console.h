//===-- yeti/core/log/console.h -------------------------*- mode: C++11 -*-===//
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
/// \file
/// \brief Console backend for logs.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_LOG_CONSOLE_H_
#define _YETI_CORE_LOG_CONSOLE_H_

#include "yeti/core/log.h"

namespace yeti {
namespace core {

namespace log {

class YETI_PUBLIC ConsoleBackend : public Backend {
 YETI_DISALLOW_COPYING(ConsoleBackend)

 public:
  ConsoleBackend();
  ~ConsoleBackend();

 private:
  enum Color {
    BLACK   = 0,
    GRAY    = 1,
    WHITE   = 2,
    RED     = 3,
    GREEN   = 4,
    BLUE    = 5,
    YELLOW  = 6,
    CYAN    = 7,
    MAGENTA = 8
  };

  void set_foreground_color(Color color);
  void set_background_color(Color color);

  void reset_foreground_color();
  void reset_background_color();

 public:
  void log(const Message &message);

 private:
  struct Storage;
  Storage *storage_;
};

} // log

} // core
} // yeti

#endif // _YETI_CORE_LOG_CONSOLE_H_
