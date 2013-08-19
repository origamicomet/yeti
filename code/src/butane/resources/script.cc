// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/script.h>

#include <butane/script.h>

namespace butane {
  static Allocator* __allocator_initializer() {
    return new ProxyAllocator("script resources", Allocators::heap());
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  static const Resource::Type* __type_initializer() {
    return new Resource::Type(
      "script", "script",
      (Resource::Type::Load)&ScriptResource::load,
      (Resource::Type::Unload)&ScriptResource::unload,
      (Resource::Type::Compile)&ScriptResource::compile);
  }

  static const thread_safe::Static< const Resource::Type >
    __ts_type(&__type_initializer);

  const Resource::Type& ScriptResource::type() {
    return __ts_type();
  }

  ScriptResource::ScriptResource(
    const Resource::Id id
  ) : butane::Resource(ScriptResource::type(), id)
    , _byte_code(allocator())
  {
  }

  ScriptResource::~ScriptResource()
  {
  }

  ScriptResource* ScriptResource::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    const LogScope _("ScriptResource::load");

    ScriptResource* script =
      make_new(ScriptResource, allocator())(id);

    script->_byte_code.resize(stream.memory_resident_data_len());

    copy(
      script->_byte_code.raw(),
      stream.memory_resident_data(),
      stream.memory_resident_data_len());

    return script;
  }

  void ScriptResource::unload(
    ScriptResource* script )
  {
    const LogScope _("ScriptResource::unload");

    assert(script != nullptr);
    make_delete(ScriptResource, allocator(), script);
  }

  struct CompiliationInfo {
    const Resource::Compiler::Input& input;
    const Resource::Compiler::Output& output;
  };

  static bool __write_to_memory_resident_data(
    void* closure,
    const void* bytes,
    size_t num_of_bytes )
  {
    const CompiliationInfo* ci = (const CompiliationInfo*)closure;
    return File::write(ci->output.memory_resident_data, bytes, num_of_bytes);
  }

  static void __on_compilation_error(
    void* closure,
    const char* error )
  {
    const CompiliationInfo* ci = (const CompiliationInfo*)closure;
    ci->output.log("Compilation failed:\n%s", error);
  }

  bool ScriptResource::compile(
    const Resource::Compiler::Input& input,
    const Resource::Compiler::Output& output )
  {
    const LogScope _("ScriptResource::compile");

    size_t code_len = 0;
    const char* code =
      (const char*)File::read(input.data, Allocators::heap(), &code_len);

    const CompiliationInfo ci = { input, output };
    if (!Script::compile(output.path, code, code_len, &__write_to_memory_resident_data, &__on_compilation_error, (void*)&ci))
      goto failure;

    Allocators::heap().free((void*)code);
    return true;

  failure:
    Allocators::heap().free((void*)code);
    return false;
  }
} // butane
