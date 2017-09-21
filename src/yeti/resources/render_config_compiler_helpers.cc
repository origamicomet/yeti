//===-- yeti/resources/render_config_compiler_helpers.cc --*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "render_config_compiler_helpers.h"

#include "yeti/core.h"

#include <stdlib.h>
#include <string.h>

namespace yeti {

namespace render_config_compiler {
  // We only allow names composed of `a-z`, `0-9`, and `_` characters.
  bool NameParser::is_acceptable_name(const char *name) {
    // TODO(mtwilliams): Always check length?
    yeti_assert_debug(strlen(name) <= 255);

    while (const char ch = *name++) {
      if ((ch >= 'a' && ch <= 'z') ||
          (ch >= '0' && ch <= '9') ||
          (ch == '_'))
        continue;
      return false;
    }

    return true;
  }
}

} // yeti
