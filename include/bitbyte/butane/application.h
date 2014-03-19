//=== bitbyte/foundation/butane/application.h ================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

/// @file bitbyte/foundation/butane/application.h
/// @brief ...
///

#ifndef _BITBYTE_BUTANE_APPLICATION_H_
#define _BITBYTE_BUTANE_APPLICATION_H_

#include <bitbyte/butane/config.h>
#include <bitbyte/butane/export.h>
#include <bitbyte/butane/foundation.h>

namespace bitbyte {
namespace butane {

class BITBYTE_BUTANE_EXPORT Application {
  protected: // constructors:
    Application();

  protected: // copy-constructors:
    Application(const Application &application);

  protected: // assignment operators:
    Application &operator=(const Application &application);

  public: // destructor:
    virtual ~Application();

  public: // accessors:
    /// ...
    const char *platform() const;

    /// ...
    const char *build() const;

  public: // callbacks:
    /// ...
    virtual bool initialize();

    /// ...
    virtual void update(const float delta_time);

    /// ...
    virtual void render() const;

    /// ...
    virtual void shutdown();

  public: // methods:
    /// ...
    void run();

    /// ...
    void quit();
};

} // butane
} // bitbyte

#endif // _BITBYTE_BUTANE_APPLICATION_H_
