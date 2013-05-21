// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/run.h>

#include <butane/butane.h>
#include <butane/script.h>
#include <butane/lua/script.h>
#include <butane/resource.h>

namespace butane {
namespace Application {
  static void on_compile_log(
    void* closure,
    const char* format, ... )
  {
  }

  void run(
    const Array<const char*>& args )
  {
    Lua::Script script;
    butane::expose(script);
    log("Hello, World!");
    static const char code[] = "log('Hello, World!')\n--Application.quit()";
    script.load("example", &code[0], sizeof(code) - 1);
    // static const char sjson[] = "{ foo = \"bar\" fizz = \"buzz\" }";
    // static const uint32_t blob_len = 4096;
    // void* blob = (void*)Allocators::heap().alloc(blob_len);
    // const sjson::Object* root;
    // if (!(root = sjson::parse(Allocators::heap(), &sjson[0], blob, blob_len)))
    //   log("Failed to parse sjson!");
    // else {
    //   const sjson::String* foo = (const sjson::String*)root->find("foo");
    //   log("foo = '%s'", foo->raw());
    //   const sjson::String* fizz = (const sjson::String*)root->find("fizz");
    //   log("fizz = '%s'", fizz->raw());
    // }

    Resource::Compiler::compile("data", "data_src", "data_src/manifest.config", &on_compile_log);
  }
} // Application
} // butane
