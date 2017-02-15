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

// TODO(mtwilliams): Drop dependence on global heap allocator.

#include "yeti/foundation/global_heap_allocator.h"
#include "yeti/foundation/path.h"

// TODO(mtwilliams): Pattern-based ignores.
// TODO(mtwilliams): Edit distance for detecting mispellings.

namespace yeti {

ResourceCompiler::ResourceCompiler()
  : db_(NULL)
  , data_len_(0)
  , data_src_len_(0)
  , debounce_(0)
  , backlog_(foundation::heap()) {
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

  resource_compiler->debounce_ = opts.debounce;

  return resource_compiler;
}

void ResourceCompiler::shutdown() {
  delete this;
}

bool ResourceCompiler::ignorable(const char *path) const {
  // TODO(mtwilliams): Implement a Git-like ignore dotfile.
  yeti_assert_debug(path != NULL);

  // Ignore any and all dot files.
  if (foundation::path::file(path)[0] == '.')
    return true;

  return false;
}

// We don't allow paths that contain characters other than the lowercase
// alphabet, numerics, underscores, dashes, path and file delimiters. This
// prevents many woes encountered when dealing with file systems across
// different platforms and tools.
bool ResourceCompiler::allowable(const char *path) const {
  yeti_assert_debug(path != NULL);

  while (char ch = *path++) {
    if ((ch >= 'a') && (ch <= 'z'))
      continue;
    if ((ch >= '0') && (ch <= '9'))
      continue;
    if (ch == '_' || ch == '_')
      continue;
    if (ch == '/' || ch == '\\')
      continue;
    if (ch == '.')
      continue;
    return false;
  }

  return true;
}

bool ResourceCompiler::compilable(const char *path) const {
  yeti_assert_debug(path != NULL);
  return !!resource_manager::type_from_path(path);
}

void ResourceCompiler::compile(bool force) {
  // First, we recursively walk |data_src_| to build a list ("backlog") of
  // files that may need to be compiled.
  foundation::fs::walk(data_src_, (foundation::fs::Walker)&ResourceCompiler::walker, (void *)this);

  // TODO(mtwilliams): Track implicit dependencies like shader includes
  // so we know to recompile all dependents whenever they're modified. This
  // means ignoring a file after we determine it's not a hard dependency.

  // Second, we iterate over our "backlog" to identify any unallowed paths or
  // uncompilable unignored source files. If we find any, we fail.
  for (const char **path = backlog_.first(); path <= backlog_.last(); ++path) {
    if (this->ignorable(*path))
      break;

    // TODO(mtwilliams): Don't assert if |*path| is not allowed or compilable,
    // instead, fail the compilation.
    yeti_assert_development(this->allowable(*path));
    yeti_assert_development(this->compilable(*path));
  }

  // Finally, we compile any files in our backlog if they have been modified
  // since our last compilation or |force| is true. Refer to
  // ResourceCompiler::compile/2 for details.
  for (const char **path = backlog_.first(); path <= backlog_.last(); ++path) {
    if (this->ignorable(*path))
      break;

    this->compile(*path, force);

    // TODO(mtwilliams): Create and use a foundation::String class.
    // TODO(mtwilliams): Use type-traits to allow non-POD in foundational data
    // structures, i.e. call destructors if required.
    foundation::heap().deallocate((uintptr_t)*path);
  }

  backlog_.clear();
}

void ResourceCompiler::compile(const char *path, bool force) {
  yeti_assert_debug(path != NULL);

  const Resource::Id id = Resource::id_from_path(path);
  const Resource::Type *type = resource_manager::type_from_path(path);

  // TODO(mtwilliams): Only compile if |last_modified_at| is more recent than
  // than whatever our |db| last saw, or if |hash| is different from whatever
  // we've seen, and |force| is false.

  // TODO(mtwilliams): Update our |db| to reflect the compilation of this
  // resource.

  fprintf(stdout, "Compiling '%s'...\n", path);
  fprintf(stdout, " type=%s\n", type->name);

  ResourceCompiler::Input input;
  input.root = &data_src_[0];
  input.path = path;

  Path input_path;
  sprintf(&input_path[0], "%s/%s", input.root, path);
  input.source = foundation::fs::open(&input_path[0], foundation::fs::READ | foundation::fs::EXCLUSIVE);
  yeti_assert(input.source != NULL);

  ResourceCompiler::Output output;
  output.root = &data_[0];

  Path memory_resident_data_path = { 0, };
  sprintf(&memory_resident_data_path[0], "%s/%016llx", output.root, id);
  output.memory_resident_data = foundation::fs::create_or_open(&memory_resident_data_path[0], foundation::fs::WRITE | foundation::fs::EXCLUSIVE);
  yeti_assert(output.memory_resident_data != NULL);

  Path streaming_data_path = { 0, };
  sprintf(&streaming_data_path[0], "%s.streaming", &memory_resident_data_path[0]);
  output.streaming_data = foundation::fs::create_or_open(&streaming_data_path[0], foundation::fs::WRITE | foundation::fs::EXCLUSIVE);
  yeti_assert(output.streaming_data != NULL);

  const bool success = type->compile(&input, &output);

  // TODO(mtwilliams): Properly track resource compilation.
  yeti_assert_development(success);

  foundation::fs::close(input.source);
  foundation::fs::close(output.memory_resident_data);
  foundation::fs::close(output.streaming_data);
}

// TODO(mtwilliams): (Reuse) our task scheduler to multithread resource
// compilation.

void ResourceCompiler::daemon() {
  // Setup our watcher. It will watch |data_src_| to collect a stream of
  // creation, modification, and deletion events for files and folders.
  foundation::fs::Watch *watch =
    foundation::fs::watch(data_src_, (foundation::fs::Watcher)&ResourceCompiler::watcher, (void *)this);

  // Setup a high-resolution timer so we time ourselves to debounce.
  foundation::HighResolutionTimer *timer =
    foundation::HighResolutionTimer::create();

  for (;;) {
    // Collect creation, modification, and deletion events for a period of time.
    while (timer->msecs() < debounce_)
      foundation::fs::poll(watch);

    // TODO(mtwilliams): Coalesce events.
    // TODO(mtwilliams): Handle events.

    timer->reset();
  }
}

void ResourceCompiler::canonicalize(char *path) const {
  yeti_assert_debug(path != NULL);

  // Lop off the root directory.
  if (strncmp(&data_src_[0], path, data_src_len_) == 0)
    memmove((void *)&path[0], (const void *)&path[data_src_len_ + 1], strlen(path) - data_src_len_);

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  foundation::path::unixify(&path[0]);
#endif
}

bool ResourceCompiler::walk(const char *path, const foundation::fs::Info *info) {
  yeti_assert_debug(path != NULL);
  yeti_assert_debug(info != NULL);

  switch (info->type) {
    case foundation::fs::FILE: {
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

void ResourceCompiler::watch(foundation::fs::Event event, const char *path) {
  yeti_assert_debug(path != NULL);

  switch (event) {
    case foundation::fs::CREATED:
      log::printf(YETI_LOG_GENERAL, log::TRACE, "+ %s", path);
      break;
    case foundation::fs::MODIFIED:
      log::printf(YETI_LOG_GENERAL, log::TRACE, "* %s", path);
      break;
    case foundation::fs::DESTROYED:
      log::printf(YETI_LOG_GENERAL, log::TRACE, "- %s", path);
      break;
  }
}

void ResourceCompiler::watcher(foundation::fs::Event event,
                               const char *path,
                               ResourceCompiler *resource_compiler) {
  return resource_compiler->watch(event, path);
}

} // yeti
