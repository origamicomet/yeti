//===-- yeti/resource_compiler.cc -------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/resource_compiler.h"

// TODO(mtwilliams): Pattern-based ignores.
// TODO(mtwilliams): Edit distance for detecting mispellings.

namespace yeti {

ResourceCompiler::ResourceCompiler()
  : backlog_(foundation::heap()) {
}

ResourceCompiler::~ResourceCompiler() {
}

ResourceCompiler *ResourceCompiler::start(const ResourceCompiler::Options &opts) {
  // TODO(mtwilliams): Move into a validation function.
  yeti_assert_debug(opts.db != NULL);
  // TODO(mtwilliams): Check based on absolute paths.
  yeti_assert_debug(strcmp(opts.data, opts.data_src) != 0);

  ResourceCompiler *resource_compiler = new (foundation::heap()) ResourceCompiler();

  resource_compiler->db_ = opts.db;

  strcpy(&resource_compiler->data_[0], opts.data);
  strcpy(&resource_compiler->data_src_[0], opts.data_src);

  resource_compiler->data_len_ = strlen(resource_compiler->data_);
  resource_compiler->data_src_len_ = strlen(resource_compiler->data_src_);

  return resource_compiler;
}

void ResourceCompiler::shutdown() {
  delete this;
}

bool ResourceCompiler::compilable(const char *path) const {
  yeti_assert_debug(path != NULL);
  return !!resource_manager::type_from_path(path);
}

bool ResourceCompiler::ignorable(const char *path) const {
  // TODO(mtwilliams): Implement a Git-like ignore dotfile.
  yeti_assert_debug(path != NULL);

  // Ignore any and all dot files.
  if (path[0] == '.')
    return true;

  return false;
}

void ResourceCompiler::compile() {
  // First, we recursively walk |data_src_| to build a list ("backlog") of
  // files that may need to be compiled.
  foundation::fs::walk(this->data_src_, (foundation::fs::Walker)&ResourceCompiler::walker, (void *)this);

  // Then we compile all of 'em if the respective |last_modified_at| is more
  // recent than whatever our |db| last saw. Refer to ResourceCompiler::compile/1 for details.
  for (const char **path = this->backlog_.first(); path <= this->backlog_.last(); ++path) {
    this->compile(*path);

    // TODO(mtwilliams): Create and use a foundation::String class.
    // TODO(mtwilliams): Use type-traits to allow non-POD in foundational data
    // structures, i.e. call destructors if required.
    foundation::heap().deallocate((uintptr_t)*path);
  }

  this->backlog_.clear();
}

void ResourceCompiler::compile(const char *path) {
  yeti_assert_debug(path != NULL);

  const Resource::Id id = Resource::id_from_path(path);
  const Resource::Type *type = resource_manager::type_from_path(path);

  // TODO(mtwilliams): Only compile if |last_modified_at| is more recent than
  // than whatever our |db| last saw.

  // TODO(mtwilliams): Update our |db| to reflect the compilation of this
  // resource.

  ResourceCompiler::Input input;
  input.root = &data_src_[0];
  input.path = path;
  input.source = foundation::fs::open(path, foundation::fs::READ | foundation::fs::EXCLUSIVE);

  ResourceCompiler::Output output;
  output.root = &data_[0];

  Path memory_resident_data_path = { 0, };
  sprintf(&memory_resident_data_path[0], "%s/%016llx", output.root, id);
  output.memory_resident_data = foundation::fs::open(&memory_resident_data_path[0], foundation::fs::WRITE | foundation::fs::EXCLUSIVE);

  Path streaming_data_path = { 0, };
  sprintf(&streaming_data_path[0], "%s.streaming", &memory_resident_data_path[0]);
  output.streaming_data = foundation::fs::open(&streaming_data_path[0], foundation::fs::WRITE | foundation::fs::EXCLUSIVE);

  type->compile(&input, &output);

  foundation::fs::close(input.source);
  foundation::fs::close(output.memory_resident_data);
  foundation::fs::close(output.streaming_data);
}

void ResourceCompiler::canonicalize(char *path) const {
  yeti_assert_debug(path != NULL);

  // Lop off the root directory.
  if (strncmp(&this->data_src_[0], path, data_src_len_) == 0)
    memmove((void *)&path[0], (const void *)&path[data_src_len_ + 1], strlen(path) - data_src_len_);

  // TODO(mtwilliams): Refactor into `foundation::path::unixy`?
#ifdef YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // Make sure we're using Unix-y path seperators.
  for (char *ch = path; *ch; ++ch)
    *ch = (*ch == '\\') ? '/' : *ch;
#endif
}

bool ResourceCompiler::walk(const char *path, const foundation::fs::Info *info) {
  yeti_assert_debug(path != NULL);
  yeti_assert_debug(info != NULL);

  switch (info->type) {
    case foundation::fs::FILE: {
      // TODO(mtwilliams): Track implicit dependencies like shader includes
      // so we know to recompile all dependents whenever they're modified.
      if (this->ignorable(path))
        break;

      // TODO(mtwilliams): Don't assert if a resource is not compilable?
      yeti_assert_development(this->compilable(path));

      // OPTIMIZE(mtwilliams): Use a buddy allocator.
      const size_t path_len = strlen(path);
      char *path_copy = (char *)foundation::heap().allocate(path_len + 1);
      memcpy((void *)path_copy, (const void *)path, path_len + 1);
      this->canonicalize(path_copy);

      backlog_.push(path_copy);
    } break;

    case foundation::fs::DIRECTORY: {
      // TODO(mtwilliams): Refactor into a work queue so we don't blow the stack
      // if we end up recursing too deeply?
      foundation::fs::walk(path, (foundation::fs::Walker)&walker, (void *)this);
    } break;
  }

  return true;
}

bool ResourceCompiler::walker(const char *path,
                              const foundation::fs::Info *info,
                              ResourceCompiler *resource_compiler) {
  return resource_compiler->walk(path, info);
}


} // yeti
