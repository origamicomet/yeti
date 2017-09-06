//===-- yeti/foundation/pattern_file_parser.cc ----------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/pattern_file_parser.h"

// Only forward declared by our header.
#include "yeti/foundation/filesystem.h"

// For buffer, and (temporarily) strings.
#include "yeti/foundation/global_heap_allocator.h"

#include <string.h>
#include <ctype.h>

namespace yeti {
namespace foundation {

PatternFileParser::PatternFileParser(fs::File *file)
  : buffer_(foundation::heap())
{
  fs::read_into_buffer(file, buffer_);

  cursor_ = 0;
  character_ = eof() ? '\0' : buffer_[cursor_];
}

void PatternFileParser::parse(Array<const char *> &patterns) {
  while (!eof()) {
    if (character_ == '#') {
      // Comment; skip.
      skip_to_next_line();
    } else {
      if (const char *line = extract())
        // Pattern.
        patterns.push(line);
    }
  }
}

const char *PatternFileParser::extract() {
  unsigned begin = cursor_;

  skip_to_next_line();

  unsigned end = cursor_;

  // Trim trailing whitespace, if any.
  while ((end > begin) && ::isspace(buffer_[--end - 1]));

  if (begin == end)
    // Empty line.
    return NULL;

  const size_t length = (end - begin);

  char *line = (char *)foundation::heap().allocate(length + 1);
  strncpy(line, (const char *)&buffer_[begin], length);
  line[length] = '\0';

  return line;
}

void PatternFileParser::skip_to_next_line() {
  while (!eof()) {
    if (advance() == '\n') {
      advance();
      break;
    }
  }
}

char PatternFileParser::advance() {
  return eof() ? '\0' : (character_ = buffer_[++cursor_]);
}

char PatternFileParser::peek() {
  return eof() ? '\0' : buffer_[cursor_ + 1];
}

bool PatternFileParser::eof() const {
  return (cursor_ == (buffer_.size() - 1));
}

} // foundation
} // yeti
