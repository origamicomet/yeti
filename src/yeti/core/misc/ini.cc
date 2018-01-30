//===-- yeti/core/misc/ini.c ----------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/misc/ini.h"

// Forward declared by header.
#include "yeti/core/platform/filesystem.h"

// Used to allocate a temporary buffer when parsing from file.
#include "yeti/core/allocators/global_heap_allocator.h"

#include <string.h>

namespace yeti {
namespace core {

namespace ini {
  namespace {
    static bool whitespace(const char ch) { return (ch == ' ') || (ch == '\t'); }
    static bool newline(const char ch) { return (ch == '\n'); }
    static bool identifer(char ch) { return (ch == '_') || (ch >= 'A' && ch <= 'z'); }

    static const char *next(const char *cursor, const char *limit, char buffer[4096]) {
      size_t n = 0;

      for (; cursor < limit; ++cursor)
        if (newline(*cursor))
          return buffer[n] = '\0', cursor + 1;
        else
          buffer[n++] = *cursor;

      return n ? cursor : NULL;
    }

    static const char *skip(const char *s) {
      while (whitespace(*s))
        ++s;
      return s;
    }

    static const char *trim(const char *start, const char *end) {
      const char *c = end;
      while (c >= start)
        if (!whitespace(*--c))
          break;
      return c;
    }

    static const char *trim(const char *string) {
      return trim(string, string + strlen(string));
    }

    static const char *find(const char *cursor, char ch) {
      for (; *cursor; ++cursor)
        if (*cursor == ';' || *cursor == '#')
          return NULL;
        else if (*cursor == ch)
          return cursor;
      return NULL;
    }

    static void copy(const char *first, const char *last, char *buffer) {
      const char *src = first; char *dst = buffer;
      while (src <= last)
        *dst++ = *src++;
      *dst = '\0';
    }

    // SMELL(mtwilliams): Unquoting logic can be improved.
    // REFACTOR(mtwilliams): Extract into generic string utility?
    static bool unquote(const char *first, const char *last, char *buffer) {
      if (*first != '"')
        return copy(first, last, buffer), true;
      else
        if (*last != '"')
          return false;

      first += 1;
      last -= 1;

      const char *src = first; char *dst = buffer;

      while (src <= last) {
        if (*src == '\\') {
          if (src == last) {
            return false;
          } else {
            switch (*++src) {
              case 'r': *dst++ = '\r'; break;
              case 'n': *dst++ = '\n'; break;
              case 't': *dst++ = '\t'; break;
              default: return false;
            }
          }
        } else {
          *dst++ = *src++;
        }
      }

      *dst = '\0';
    }
  }
}

bool ini::parse(const char *buffer, size_t length, Callback callback, void *context, u32 options) {
  yeti_assert_debug(buffer != NULL);
  yeti_assert_debug(callback != NULL);

  char line[4096], section[256], key[256], value[256];
  const char *cursor = buffer;
  const char *start, *end;

  cursor = next(cursor, buffer + length, line);

  do {
    if (strchr(";#", *(start = skip(line)))) {
      continue;
    } else if (*start == '[') {
      if (end = find(start + 1, ']'))
        copy(start + 1, end - 1, section);
      else
        return false;
    } else if (*start == ']') {
      return false;
    } else if (identifer(*start)) {
      if (end = find(start, '=')) {
        copy(start, trim(start, end), key);
        start = skip(end + 1);
        end = trim(start);
        if (unquote(start, end, value))
          if (callback(section, key, value, context))
            continue;
        if (options & CONTINUE_ON_ERROR)
          continue;
        return false;
      } else {
        if (options & CONTINUE_ON_ERROR)
          continue;
        return false;
      }
    } else {
      return false;
    }
  } while (cursor = next(cursor, buffer + length, line));

  return true;
}

bool ini::parse(File *file, Callback callback, void *context, u32 options) {
  Array<u8> buffer(core::global_heap_allocator());
  if (const size_t length = fs::read_into_buffer(file, buffer))
    return parse((const char *)buffer.raw(), length, callback, context, options);
  return false;
}

} // core
} // yeti
