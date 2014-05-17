//===-- bitbyte/butane/application.h ----------------------------*- C++ -*-===//
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
///
/// \file
/// \brief Defines the base-class that is sub-classed to "run" the engine.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_APPLICATION_H_
#define _BITBYTE_BUTANE_APPLICATION_H_

//===----------------------------------------------------------------------===//

#include <bitbyte/butane/config.h>
#include <bitbyte/butane/export.h>
#include <bitbyte/butane/foundation.h>

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

/// \brief
///
class BITBYTE_BUTANE_EXPORT Application {
  //===--------------------------------------------------------------------===//

  protected:
    Application();

  //===--------------------------------------------------------------------===//

  private:
    Application(const Application &application);
    Application &operator=(const Application &application);

  //===--------------------------------------------------------------------===//

  public:
    virtual ~Application();

  //===--------------------------------------------------------------------===//

  protected:
    /// \brief Called before entering the update-then-render loop.
    ///
    virtual bool initialize();

    /// \brief Called every frame to update.
    ///
    virtual void update(const float delta_time);

    /// \brief Called every frame to render.
    ///
    virtual void render() const;

    /// \brief Called before terminating the application.
    ///
    virtual void shutdown();

  //===--------------------------------------------------------------------===//

  public:
    /// \brief Initializes then enters the update-then-render loop.
    ///
    void start();

    /// \brief The update-then-render loop.
    ///
    void run();

    /// \brief Shutsdown then terminates the application.
    ///
    void quit();

  //===--------------------------------------------------------------------===//
};

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // _BITBYTE_BUTANE_APPLICATION_H_

//===----------------------------------------------------------------------===//
