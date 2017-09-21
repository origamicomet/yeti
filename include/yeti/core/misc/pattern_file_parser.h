//===-- yeti/core/misc/pattern_file_parser.h ------------*- mode: C++11 -*-===//
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
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_MISC_PATTERN_FILE_PARSER_H_
#define _YETI_CORE_MISC_PATTERN_FILE_PARSER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

#include "yeti/core/containers/array.h"

namespace yeti {
namespace core {

// See `yeti/core/platform/filesystem.h`.
struct File;

/// \brief A pattern file parser.
///
/// \details Parses a file of patterns akin to a `.gitignore`.
///
class YETI_PUBLIC PatternFileParser {
 YETI_DISALLOW_COPYING(PatternFileParser)

 public:
  PatternFileParser(File *file);

 public:
  /// Extracts patterns from file.
  void parse(Array<const char *> &patterns);

 private:
  /// \internal Extracts pattern on current line, unless empty.
  const char *extract();

  /// \internal Skips to new line.
  void skip_to_next_line();

  /// \internal Get next character.
  char advance();

  /// \internal Get next character without advancing.
  char peek();

  /// \internal End of file?
  bool eof() const;

 private:
  Array<u8> buffer_;
  unsigned cursor_;
  char character_;
};

} // core
} // yeti

#endif // _YETI_CORE_MISC_PATTERN_FILE_PARSER_H_
