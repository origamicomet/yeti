//=== bitbyte/butane/console.h ===============================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

/// @file bitbyte/butane/console.h
/// @brief ...
///

#ifndef _BITBYTE_BUTANE_CONSOLE_H_
#define _BITBYTE_BUTANE_CONSOLE_H_

#include <bitbyte/butane/config.h>
#include <bitbyte/butane/export.h>
#include <bitbyte/butane/foundation.h>

namespace bitbyte {
namespace butane {

/// ...
extern BITBYTE_BUTANE_EXPORT class Console &console();

/// @brief
///
class BITBYTE_BUTANE_EXPORT Console {
  private: // constructors:
    Console();

  private: // copy-constructors:
    Console(const Console &);

  private: // assignment operators:
    Console &operator=(const Console &);

  private: // destructor:
    ~Console();

  public: // management:
    /// ...
    static void initialize();
};

} // butane
} // bitbyte

#endif // _BITBYTE_BUTANE_CONSOLE_H_
