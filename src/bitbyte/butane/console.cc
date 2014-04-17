//===-- bitbyte/butane/console.cc -------------------------------*- C++ -*-===//
//
//  Butane, a data-driven game engine.
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mwilliams@bitbyte.ca>
//
//===----------------------------------------------------------------------===//

#include <bitbyte/butane/console.h>

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

namespace Console {
  //===--------------------------------------------------------------------===//

  void install()
  {
    foundation::loggers::add(
      [](const char *file, size_t line, const char *message, void *) -> void {
        (void)file; (void)line;
        ::fputs(message, stdout);
      });
  }

  //===--------------------------------------------------------------------===//
}

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//
