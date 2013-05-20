// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/run.h>

#include <butane/butane.h>
#include <butane/script.h>
#include <butane/lua/script.h>

namespace butane {
namespace Application {
  void run(
    const Array<const char*>& args )
  {
    Lua::Script script;
    butane::expose(script);
    log("Hello, World!\n");
    static const char code[] = "log('Hello, World!\\n')\n--Application.quit()";
    script.load("example", &code[0], sizeof(code) - 1);
    static const char sjson[] = "{ foo = \"bar\" fizz = \"buzz\" }";
    static const uint32_t blob_len = 4096;
    void* blob = (void*)Allocators::heap().alloc(blob_len);
    const sjson::Object* root;
    if (!(root = sjson::parse(Allocators::heap(), &sjson[0], blob, blob_len)))
      log("Failed to parse sjson!\n");
    else {
      const sjson::String* foo = (const sjson::String*)root->find("foo");
      log("foo = '%s'\n", foo->raw());
      const sjson::String* fizz = (const sjson::String*)root->find("fizz");
      log("fizz = '%s'\n", fizz->raw());
    }
  }
} // Application
} // butane
