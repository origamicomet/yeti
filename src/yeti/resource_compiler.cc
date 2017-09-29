//===-- yeti/resource_compiler.cc -----------------------*- mode: C++11 -*-===//
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
#include "yeti/core/allocators/global_heap_allocator.h"

// For log forwarding.
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace yeti {

namespace core {
namespace log {

static const core::log::Category::Id RESOURCE_COMPILER =
  Category::add("resource_compiler", TOOLS);

} // log
} // core

ResourceCompiler::ResourceCompiler()
  : db_(NULL)
  , data_len_(0)
  , data_src_len_(0)
  , ignore_(core::global_heap_allocator())
  , debounce_(0)
  , backlog_(core::global_heap_allocator())
  , daemonized_(false)
  , stop_(false) {
}

ResourceCompiler::~ResourceCompiler() {
}

static void register_with_database(Resource::Type::Id id,
                                   const Resource::Type *type,
                                   void *db) {
  ((ResourceDatabase *)db)->register_a_type(type);
}

ResourceCompiler *ResourceCompiler::create(const ResourceCompiler::Options &options) {
  // TODO(mtwilliams): Check based on absolute paths.
  yeti_assert_debug(strcmp(options.data, options.data_src) != 0);

  ResourceDatabase *db = options.db;
  yeti_assert_debug(db != NULL);

  ResourceCompiler *resource_compiler = YETI_NEW(ResourceCompiler, core::global_heap_allocator());

  resource_compiler->db_ = db;

  yeti_assert_with_reason_development(resource_compiler->db_->sophisticated(),
                                      "Can only compile against a sophisticated resource database!");

  // Register types with database.
  resource::for_each_type(&register_with_database, (void *)db);

  strcpy(resource_compiler->data_, options.data);
  strcpy(resource_compiler->data_src_, options.data_src);

  resource_compiler->data_len_ = strlen(resource_compiler->data_);
  resource_compiler->data_src_len_ = strlen(resource_compiler->data_src_);

  resource_compiler->debounce_ = options.debounce;

  // Add ignore patterns from file, if it exists.
  resource_compiler->add_ignore_patterns(options.ignore);

  return resource_compiler;
}

void ResourceCompiler::destroy() {
  YETI_DELETE(ResourceCompiler, core::global_heap_allocator(), this);
}

void ResourceCompiler::add_ignore_patterns(const char *path) {
  if (core::File *file = core::fs::open(path, core::File::READ)) {
    core::PatternFileParser(file).parse(ignore_);
    core::fs::close(file);
  }
}

// TODO(mtwilliams): Cross reference with database, and remove compiled data
// if source data is removed.

void ResourceCompiler::run(bool force) {
  // We recursively walk source data directory to build a list of files that
  // may need to be compiled.
  core::fs::walk(data_src_, (core::fs::Walker)&ResourceCompiler::walker, (void *)this);

  // Then we compile any files in our list if they have been modified since
  // our last compilation or if forcing recompilation.
  for (const char **path = backlog_.begin(); path != backlog_.end(); ++path) {
    const ResourceCompiler::Result result = this->compile(*path, force);

    switch (result) {
      case resource_compiler::Results::IGNORED:
        core::logf(core::log::RESOURCE_COMPILER, core::log::INFO, "Ignored `%s`.", *path);
        break;
      case resource_compiler::Results::FORBIDDEN:
        core::logf(core::log::RESOURCE_COMPILER, core::log::ERROR, "Skipped `%s` due to path.", *path);
        break;
      case resource_compiler::Results::UNCOMPILABLE:
        core::logf(core::log::RESOURCE_COMPILER, core::log::ERROR, "Skipped `%s` due to type.", *path);
        break;
      case resource_compiler::Results::SKIPPED:
        core::logf(core::log::RESOURCE_COMPILER, core::log::INFO, "Skipped `%s` because it's up to date.", *path);
        break;
    }

    // TODO(mtwilliams): Move to a string class.
    core::global_heap_allocator().deallocate((void *)*path);
  }

  backlog_.clear();
}

void ResourceCompiler::daemon() {
  daemonized_ = true;

  // Setup our watcher. It will watch the source data directory to collect a
  // stream of creation, modification, and deletion events for files and
  // folders.
  core::fs::Watch *watch =
    core::fs::watch(data_src_, (core::fs::Watcher)&ResourceCompiler::watcher, (void *)this);

  // Setup a high-resolution timer so we time ourselves to debounce.
  core::Timer timer;

  while (!stop_) {
    // Collect creation, modification, and deletion events for a period of time.
    while (timer.msecs() <= debounce_)
      core::fs::poll(watch);

    // TODO(mtwilliams): Coalesce events.
    // TODO(mtwilliams): Handle events.

    timer.reset();
  }

  // Stop watching the source data directory.
  core::fs::unwatch(watch);
}

void ResourceCompiler::stop() {
  yeti_assert_debug(daemonized_);

  // See loop in `ResourceCompiler::daemon`.
  stop_ = true;
}

// PERF(mtwilliams): Use task scheduler to multi-thread resource compilation.

ResourceCompiler::Result ResourceCompiler::compile(const char *path, bool force) {
  yeti_assert_debug(path != NULL);

  if (this->ignorable(path))
    return resource_compiler::Results::IGNORED;

  if (!this->allowable(path))
    return resource_compiler::Results::FORBIDDEN;

  if (!this->compilable(path))
    return resource_compiler::Results::UNCOMPILABLE;

  Path source_file_path;
  sprintf(&source_file_path[0], "%s/%s", &data_src_[0], path);

  core::File::Info source_file_info;
  core::fs::info(source_file_path, &source_file_info);

  const Resource::File::Id source_file_id = db_->add_a_file(path);

  Resource::File previous_source_file_info;
  db_->info(source_file_id, &previous_source_file_info);

  const Resource::Type *type = resource::type_from_path(path);

  char name[256];
  name_from_path(path, &name[0], sizeof(name));

  const Resource::Id id = db_->add_a_resource(resource::id_from_type(type), name);

  if (!force)
    if (source_file_info.last_modified_at <= previous_source_file_info.timestamp)
      if (source_file_info.last_modified_at <= db_->built(id))
        // Already up to date.
        return resource_compiler::Results::SKIPPED;

  core::File *source_file_handle =
    core::fs::open(source_file_path, core::File::READ | core::File::EXCLUSIVE);

  char source_file_fingerprint[40];

  // Compute fingerprint.
  core::sha1::fingerprint(source_file_handle, &source_file_fingerprint[0]);

  // Rewind to beginning rather than reopening to prevent modifications.
  core::fs::seek(source_file_handle, core::File::ABSOLUTE, 0);

  db_->begin();

  // Reflect filesystem changes to database.
  db_->touch(source_file_id,
             source_file_info.last_modified_at,
             source_file_fingerprint);

  // Queue the build.
  const Resource::Build::Id build = db_->queue_a_build(id);

  db_->end();

  db_->start_a_build(build);

  ResourceCompiler::Environment env;
  env.compiler = this;
  env.id = id;
  env.build = build;
  env.info = &ResourceCompiler::info;
  env.warning = &ResourceCompiler::warning;
  env.error = &ResourceCompiler::error;

  env.info(&env, "Compiling `%s` into `%016llx`...\n", path, id);

  ResourceCompiler::Input input;
  input.root = &data_src_[0];
  input.path = path;
  input.source = source_file_handle;
  core::memory::copy((const void *)&source_file_fingerprint[0], (void *)&input.fingerprint[0], 40);

  ResourceCompiler::Output output;
  output.root = &data_[0];

  // TODO(mtwilliams): Atomic compilation?
  Path memory_resident_data_path = { 0, };
  sprintf(&memory_resident_data_path[0], "%s/%016llx", output.root, id);
  output.memory_resident_data =
    core::fs::create_or_open(&memory_resident_data_path[0],
                             core::File::WRITE | core::File::EXCLUSIVE);

  Path streaming_data_path = { 0, };
  sprintf(&streaming_data_path[0], "%s.streaming", &memory_resident_data_path[0]);
  output.streaming_data =
    core::fs::create_or_open(&streaming_data_path[0],
                             core::File::WRITE | core::File::EXCLUSIVE);

  if (!input.source)
    env.error(&env, "Could not open `%s` for reading!", source_file_path);

  if (!output.memory_resident_data)
    env.error(&env, "Could not open memory-resident data for writing!");

  if (!output.streaming_data)
    env.error(&env, "Could not open streaming data for writing!");

  bool success;

  if (input.source && output.memory_resident_data && output.streaming_data) {
    if (success = type->compile(&env, &input, &output))
      env.info(&env, "Compiled!");
    else
      env.error(&env, "Failed.");
  } else {
    success = false;
  }

  if (input.source)
    core::fs::close(input.source);
  if (output.memory_resident_data)
    core::fs::close(output.memory_resident_data);
  if (output.streaming_data)
    core::fs::close(output.streaming_data);

  db_->finish_a_build(build, success);

  return success ? resource_compiler::Results::SUCCEEDED
                 : resource_compiler::Results::FAILED;
}

void ResourceCompiler::canonicalize(char *path) const {
  yeti_assert_debug(path != NULL);

  // Lop off the root directory.
  if (strncmp(&data_src_[0], path, data_src_len_) == 0)
    memmove((void *)&path[0], (const void *)&path[data_src_len_ + 1], strlen(path) - data_src_len_);

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  core::path::unixify(&path[0]);
#endif
}

bool ResourceCompiler::ignorable(const char *path) const {
  yeti_assert_debug(path != NULL);

  // Ignore any and all dot files.
  if (core::path::file(path)[0] == '.')
    return true;

  // Ignore any files matching patterns specified in `.dataignore`.
  for (const char *const *pattern = ignore_.begin(); pattern != ignore_.end(); ++pattern)
    if (core::path::match(*pattern, path))
      return true;

  return false;
}

// We don't allow paths that contain characters other than the lowercase
// alphabetics, numerics, underscores, and path and extension delimiters. This
// prevents many woes encountered when dealing with filesystems across
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
    if (ch == '/')
      continue;
    if (ch == '.')
      continue;
    return false;
  }

  return true;
}

// TODO(mtwilliams): Use edit distance to detect potential mispellings.
bool ResourceCompiler::compilable(const char *path) const {
  yeti_assert_debug(path != NULL);
  return !!resource::type_from_path(path);
}

void ResourceCompiler::name_from_path(const char *path, char *name, size_t limit) {
  yeti_assert_debug(path != NULL);
  yeti_assert_debug(name != NULL);

  const char *extension = core::path::extension(path);

  // Path sans extension is name.
  const size_t length_sans_extension = extension - path - 1;

  yeti_assert_with_reason_debug(length_sans_extension < limit,
                                "Name derived from `%s` is too long!.",
                                path);

  strncpy(name, path, length_sans_extension);

  // Ensure name is null-terminated.
  name[length_sans_extension] = '\0';
}

bool ResourceCompiler::walk(const char *path, const core::File::Info *info) {
  yeti_assert_debug(path != NULL);
  yeti_assert_debug(info != NULL);

  switch (info->type) {
    case core::File::FILE: {
      // OPTIMIZE(mtwilliams): Use a buddy allocator.
      const size_t path_len = strlen(path);
      char *path_copy = (char *)core::global_heap_allocator().allocate(path_len + 1);
      memcpy((void *)path_copy, (const void *)path, path_len + 1);
      this->canonicalize(path_copy);

      backlog_.push(path_copy);
    } break;

    case core::File::DIRECTORY: {
      // TODO(mtwilliams): Refactor into a work queue so we don't blow the
      // stack if we end up recursing too deeply?
      core::fs::walk(path, (core::fs::Walker)&walker, (void *)this);
    } break;
  }

  return true;
}

bool ResourceCompiler::walker(const char *path,
                              const core::File::Info *info,
                              ResourceCompiler *resource_compiler) {
  return resource_compiler->walk(path, info);
}

void ResourceCompiler::watch(core::fs::Event event, const char *path) {
  yeti_assert_debug(path != NULL);

  switch (event) {
    case core::fs::CREATED:
      core::logf(core::log::RESOURCE_COMPILER, core::log::TRACE, "+ %s", path);
      break;
    case core::fs::MODIFIED:
      core::logf(core::log::RESOURCE_COMPILER, core::log::TRACE, "* %s", path);
      break;
    case core::fs::DESTROYED:
      core::logf(core::log::RESOURCE_COMPILER, core::log::TRACE, "- %s", path);
      break;
  }
}

void ResourceCompiler::watcher(core::fs::Event event,
                               const char *path,
                               ResourceCompiler *resource_compiler) {
  return resource_compiler->watch(event, path);
}

namespace {
  // Translates `Resource::Build::Log::Level` to `core::Log::Level`.
  static core::log::Level level_to_core_(const Resource::Build::Log::Level level) {
    switch (level) {
      case Resource::Build::Log::INFO: return core::log::INFO;
      case Resource::Build::Log::WARNING: return core::log::WARNING;
      case Resource::Build::Log::ERROR: return core::log::ERROR;
    }
  }
}

void ResourceCompiler::info(const Environment *env, const char *format, ...) {
  va_list va;
  va_start(va, format);
  env->compiler->forward_to_log(env, Resource::Build::Log::INFO, format, va);
  va_end(va);
}

void ResourceCompiler::warning(const Environment *env, const char *format, ...) {
  va_list va;
  va_start(va, format);
  env->compiler->forward_to_log(env, Resource::Build::Log::WARNING, format, va);
  va_end(va);
}

void ResourceCompiler::error(const Environment *env, const char *format, ...) {
  va_list va;
  va_start(va, format);
  env->compiler->forward_to_log(env, Resource::Build::Log::ERROR, format, va);
  va_end(va);
}

void ResourceCompiler::forward_to_log(const Environment *env,
                                      Resource::Build::Log::Level level,
                                      const char *format,
                                      va_list ap) {
  // Format.
  const int size = vsnprintf(NULL, 0, format, ap) + 1;
  char *message = (char *)alloca(size);
  vsnprintf(message, size, format, ap);

  // Copy to build log.
  db_->log(env->build, level, message);

  // Log to console.
  core::logf(core::log::RESOURCE_COMPILER, level_to_core_(level), message);
}

} // yeti
