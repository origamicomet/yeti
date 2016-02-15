//===-- yeti/input/keyboard.h -----------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_INPUT_KEYBOARD_H_
#define _YETI_INPUT_KEYBOARD_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {

namespace Keys {
  enum _ {
    #define _(Id, Mnemonic, _) Mnemonic = Id,
      #include "yeti/input/keys.inl"
    #undef _

    _COUNT
  };
} typedef Keys::_ Key;

namespace Keyboard {

/// Returns wether a keyboard is connected and functioning.
extern YETI_PUBLIC bool connected();
extern YETI_PUBLIC bool disconnected();

/// Returns wether the @key was pressed this frame.
extern YETI_PUBLIC bool pressed(const Key key);

/// Returns wether the @key has been held down for more than one frame.
extern YETI_PUBLIC bool held(const Key key);

/// Returns wether the @key was released this frame.
extern YETI_PUBLIC bool released(const Key key);

/// \internal Marks the @key as released for this frame.
extern YETI_LOCAL void up(const Key key);

/// \internal Marks the @key as pressed for this frame.
extern YETI_LOCAL void down(const Key key);

/// \internal
extern YETI_LOCAL void update();

} // Keyboard

} // yeti

#endif // _YETI_INPUT_KEYBOARD_H_
