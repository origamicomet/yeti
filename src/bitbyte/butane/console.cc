//=== bitbyte/butane/console.cc ==============================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

#include <bitbyte/butane/console.h>

#include <stdio.h>

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//
// Singleton:
//

static Console *console_ = NULL;
Console &console() {
  return *console_;
}

//===----------------------------------------------------------------------===//
// Constructors:
//

Console::Console() {}

//===----------------------------------------------------------------------===//
// Copy constructors:
//

#if 0
Console::Console(const Console &) {}
#endif

//===----------------------------------------------------------------------===//
// Assignment operators:
//

#if 0
Console &Console::operator=(const Console &) { return *this; }
#endif

//===----------------------------------------------------------------------===//
// Destructor:
//

Console::~Console() {}

//===----------------------------------------------------------------------===//
// Console::initialize
//

void Console::initialize()
{
  bitbyte_butane_assert(development, console_ == NULL);
  console_ = bitbyte_butane_new(foundation::heap(), Console)();
  foundation::loggers::add(
    [](const char *file, size_t line, const char *message, void *) -> void {
      (void)file; (void)line;
      ::fputs(message, stdout);
    });
}

} // butane
} // bitbyte
