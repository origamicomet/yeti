// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/script.h>
#include <bt/application_if.h>

namespace bt {
  Script::Stack::Stack( Script* script )
    : _script(script)
  {
  }

  Script::Stack::~Stack()
  {
  }

  // ============================================================

  Script::Arguments::Arguments( Script* script )
    : _script(script)
  {
  }

  Script::Arguments::~Arguments()
  {
  }

  // ============================================================

  static void __on_error(
    Script* script,
    const char* error_msg,
    Callstack& cs )
  {

    log("\n===============================================================================\n");
    log("Script: %s\n  in %s\n", error_msg, script->to_string().to_ptr());
    log("-------------------------------------------------------------------------------\n");
    log("Callstack (Script):\n\n");

    if (cs.frames.size() > 0) {
      for (size_t f = 0; f < cs.frames.size(); ++f) {
        Callstack::Frame& frame = cs.frames[f];
        log(
          "  %s\n    in `%s` on line %u\n\n",
          &frame.name[0],
          &frame.file[0],
          frame.line
        );
      }
    } else {
      log("  failed!\n\n");
    }

    log("-------------------------------------------------------------------------------\n");
    log("Callstack:\n\n");

    cs.frames.resize(0);
    callstack(cs);

    if (cs.frames.size() > 0) {
      for (size_t f = 0; f < cs.frames.size(); ++f) {
        Callstack::Frame& frame = cs.frames[f];
        log(
          "  %s\n    in `%s` on line %u\n\n",
          &frame.name[0],
          &frame.file[0],
          frame.line
        );
      }
    } else {
      log("  failed!\n\n");
    }
  }

  Script::Script()
    : _on_error(&__on_error)
  {
  }

  Script::~Script()
  {
  }

  String Script::to_string()
  {
    return String(Allocator::scratch(),
      "#<Script:%016" PRIx64 ">",
      (uint64_t)this);
  }

  namespace ScriptInterface {
    void expose( Script* script ) {
      Application::expose(script);
    }
  } // ScriptInterface
} // bt