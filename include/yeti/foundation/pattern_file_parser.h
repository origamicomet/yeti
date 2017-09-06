//===-- yeti/foundation/pattern_file_parser.h -----------*- mode: C++11 -*-===//
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

#ifndef _YETI_FOUNDATION_PATTERN_FILE_PARSER_H_
#define _YETI_FOUNDATION_PATTERN_FILE_PARSER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"

#include "yeti/foundation/array.h"

namespace yeti {
namespace foundation {

namespace fs {
  // See `yeti/foundation/filesystem.h`.
  struct File;
}

class YETI_PUBLIC PatternFileParser {
 YETI_DISALLOW_COPYING(PatternFileParser)

 public:
  PatternFileParser(fs::File *file);

 public:
  // Extracts patterns from file.
  void parse(Array<const char *> &patterns);

 private:
  // Extracts pattern on current line, unless empty.
  const char *extract();

  // Skips to new line.
  void skip_to_next_line();

  // Get next character.
  char advance();

  // Get next character without advancing.
  char peek();

  // End of file?
  bool eof() const;

 private:
  Array<u8> buffer_;
  unsigned cursor_;
  char character_;
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_PATTERN_FILE_PARSER_H_
