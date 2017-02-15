//===-- yeti/foundation/log/console.cc --------------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/log/console.h"

#include <time.h>

namespace yeti {
namespace log {

namespace console {
  void log(const log::Message *msg) {
    yeti_assert_debug(msg != NULL);
    yeti_assert_debug(msg->meta.level != OFF);
    yeti_assert_debug(msg->meta.structured == 0);

    // BUG(mtwilliams): Potential overflow.
    char *buf = (char *)alloca(msg->content_len + 8 * 36);

    // TODO(mtwilliams): Millisecond resolution.
    time_t t;
    time(&t);
    strftime(&buf[0], 24, "%Y/%m/%d %H:%M:%S.000", gmtime(&t));

    const char *level;
    switch (msg->meta.level) {
      case TRACE:   level = "TRACE"; break;
      case DEBUG:   level = "DEBUG"; break;
      case INFO:    level = "INFO";  break;
      case WARNING: level = "WARN";  break;
      case ERROR:   level = "ERROR"; break;
      case FATAL:   level = "FATAL"; break;
    }

    sprintf(&buf[23], " [%-7s]  ", level);

    char *copy = &buf[35];
    const char *text = (const char *)&msg->content[0];
    while (char ch = *text++) {
      switch (ch) {
        case '\n': {
          if (*text) {
            *copy++ = '\n';
            memcpy((void *)copy, (const void *)&buf[0], 35);
            copy += 35;
          }
        } break;
        default: {
          *copy++ = ch;
        } break;
      }
    }
    *copy++ = '\n';
    *copy = '\0';

    ::fputs(buf, stdout);
  }
}

} // log
} // yeti
