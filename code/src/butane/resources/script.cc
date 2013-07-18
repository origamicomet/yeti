// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/script.h>

#include <butane/script.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("script resources", Allocators::heap());
    return allocator;
  }

  static const Resource::Type& __type_initializer() {
    static const Resource::Type type(
      "script", "script",
      (Resource::Type::Load)&ScriptResource::load,
      (Resource::Type::Unload)&ScriptResource::unload,
      (Resource::Type::Compile)&ScriptResource::compile);
    return type;
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

    const MemoryResidentData* mrd =
      ((const MemoryResidentData*)stream.memory_resident_data());

    ScriptResource* script =
      make_new(ScriptResource, allocator())(id);

    script->_byte_code.resize(mrd->byte_code_len);

    copy(
      script->_byte_code.raw(),
      (const void*)script->_byte_code.raw(),
      mrd->byte_code_len);

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

    MemoryResidentData mrd; {
      int64_t offset = sizeof(MemoryResidentData);
      mrd.byte_code = relative_ptr<const char*>(offset - offsetof(MemoryResidentData, byte_code));
    }

    if (!File::write(output.memory_resident_data, (const void*)&mrd, sizeof(MemoryResidentData))) {
      output.log("Unable to write memory-resident data!");
      goto failure; }

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
