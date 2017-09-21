//===-- yeti/core/log/console.cc ------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/log/console.h"

// To allocate storage.
#include "yeti/core/allocators/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

namespace yeti {
namespace core {

namespace log {

struct ConsoleBackend::Storage {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  HANDLE handle_for_stdout;
  HANDLE handle_for_stderr;
  WORD   attributes;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
};

ConsoleBackend::ConsoleBackend()
  : Backend("console")
{
  storage_ =
    (ConsoleBackend::Storage *)global_heap_allocator().allocate(sizeof(ConsoleBackend::Storage),
                                                                alignof(ConsoleBackend::Storage));

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  storage_->handle_for_stdout = ::GetStdHandle(STD_OUTPUT_HANDLE);
  storage_->handle_for_stderr = ::GetStdHandle(STD_ERROR_HANDLE);

  // HACK(mtwilliams): Assume standard output and error use the same console.
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  ::GetConsoleScreenBufferInfo(storage_->handle_for_stdout, &csbi);

  storage_->attributes = csbi.wAttributes;

  this->set_foreground_color(WHITE);
  this->set_background_color(BLACK);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

ConsoleBackend::~ConsoleBackend() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::CloseHandle(storage_->handle_for_stdout);
  ::CloseHandle(storage_->handle_for_stderr);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

  core::global_heap_allocator().deallocate((void *)storage_);
}

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  static WORD COLOR_TO_ATTRIBTUES[9] = {
    ( 0x0 | 0x0 | 0x0 | 0x0 ), // Black
    ( 0x0 | 0x3 | 0x2 | 0x1 ), // Gray
    ( 0x4 | 0x3 | 0x2 | 0x1 ), // White
    ( 0x4 | 0x3 | 0x0 | 0x0 ), // Red
    ( 0x4 | 0x0 | 0x2 | 0x0 ), // Green
    ( 0x4 | 0x0 | 0x0 | 0x1 ), // Blue
    ( 0x4 | 0x3 | 0x2 | 0x0 ), // Yellow
    ( 0x4 | 0x0 | 0x2 | 0x1 ), // Cyan
    ( 0x4 | 0x3 | 0x0 | 0x1 )  // Magenta
  };
#endif

void ConsoleBackend::set_foreground_color(Color color) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const WORD foreground_color_attributes = COLOR_TO_ATTRIBTUES[color];
  storage_->attributes = (storage_->attributes & ~0x0F) | foreground_color_attributes;
  ::SetConsoleTextAttribute(storage_->handle_for_stdout, storage_->attributes);
  ::SetConsoleTextAttribute(storage_->handle_for_stderr, storage_->attributes);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void ConsoleBackend::set_background_color(Color color) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const WORD background_color_attributes = COLOR_TO_ATTRIBTUES[color] << 4;
  storage_->attributes = (storage_->attributes & ~0xF0) | background_color_attributes;
  ::SetConsoleTextAttribute(storage_->handle_for_stdout, storage_->attributes);
  ::SetConsoleTextAttribute(storage_->handle_for_stderr, storage_->attributes);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void ConsoleBackend::log(const Message &message) {
  // TODO(mtwilliams): Use `message.when`..
  // TODO(mtwilliams): Millisecond resolution.
  char timestamp[24];
  time_t t;
  time(&t);
  strftime(&timestamp[0], 24, "%Y/%m/%d %H:%M:%S.000", gmtime(&t));

  FILE *stream;
  const char *level;
  Color color;

  switch (message.meta.level) {
    case TRACE:   stream = stdout; level = "TRACE"; color = MAGENTA; break;
    case DEBUG:   stream = stdout; level = "DEBUG"; color = CYAN;    break;
    case INFO:    stream = stdout; level = "INFO";  color = WHITE;   break;
    case WARNING: stream = stdout; level = "WARN";  color = YELLOW;  break;
    case ERROR:   stream = stderr; level = "ERROR"; color = RED;     break;
    case FATAL:   stream = stderr; level = "FATAL"; color = RED;     break;
  }

  this->set_foreground_color(color);

  const char *cursor = (const char *)&message.content[0],
             *line   = (const char *)&message.content[0];

  while (const char character = *cursor++) {
    switch (character) {
      case '\n':
        ::fputs(timestamp, stream);
        ::fputc(' ', stream);
        ::fprintf(stream, "[%-5s]", level);
        ::fputc(' ', stream);
        ::fwrite((const void *)line, 1, cursor - line, stream);

        line = cursor;
    }
  }

  if (*line) {
    ::fputs(timestamp, stream);
    ::fputc(' ', stream);
    ::fprintf(stream, "[%-5s]", level);
    ::fputc(' ', stream);
    ::fwrite((const void *)line, 1, cursor - line, stream);
  }

  this->set_foreground_color(WHITE);
}

} // log

} // core
} // yeti
