//===-- yeti/resources/render_config_compiler_helpers.h ---*- mode: C++ -*-===//
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

#ifndef _YETI_RENDER_CONFIG_COMPILER_HELPERS_H_
#define _YETI_RENDER_CONFIG_COMPILER_HELPERS_H_

namespace yeti {

namespace render_config_compiler {
  class NameParser {
   private:
    static bool is_acceptable_name(const char *name);
  };

  /// Parses constant or variable binding.
  template <typename T>
  class BindingParser {};
}

} // yeti

#endif // _YETI_RENDER_CONFIG_COMPILER_HELPERS_H_
