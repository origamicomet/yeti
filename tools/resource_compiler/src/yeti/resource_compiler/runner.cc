//===-- yeti/resource_compiler/runner.cc ------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#include "yeti/resource_compiler/runner.h"

namespace yeti {
namespace resource_compiler {

Runner::Runner()
  : resource_database_(NULL)
  , resource_compiler_(NULL)
  , force_(false)
  , watch_(false) {
}

Runner::~Runner() {
}

void Runner::setup(const char *args[], const u32 num_args) {
  ResourceCompiler::Path cwd;
  foundation::path::cwd(&cwd[0], sizeof(cwd));

  ResourceCompiler::Path resource_database_path;
  ResourceCompiler::Path data_path;
  ResourceCompiler::Path source_data_path;

  sprintf(&resource_database_path[0], "%s/resources.db", &cwd[0]);
  sprintf(&data_path[0], "%s/data", &cwd[0]);
  sprintf(&source_data_path[0], "%s/data_src", &cwd[0]);

  for (const char **arg = &args[0], **end = &args[num_args]; arg < end; ++arg) {
    if (strcmp(*arg, "--db") == 0)
      sprintf(&resource_database_path[0], "%s/%s", cwd, *++arg);
    else if (strcmp(*arg, "--data") == 0)
      sprintf(&data_path[0], "%s/%s", cwd, *++arg);
    else if (strcmp(*arg, "--data-src") == 0)
      sprintf(&source_data_path[0], "%s/%s", cwd, *++arg);
    else if (strcmp(*arg, "--force") == 0)
      force_ = true;
    else if (strcmp(*arg, "--watch") == 0)
      watch_ = true;
    else
      yeti_assertf(0, "Unknown command-line argument '%s'.", *arg);
  }

  foundation::path::unixify(&resource_database_path[0]);
  foundation::path::unixify(&data_path[0]);
  foundation::path::unixify(&source_data_path[0]);

  resource_database_ = ResourceDatabase::open_or_create(&resource_database_path[0]);;

  resource_compiler_opts_.db = resource_database_;
  memcpy((void *)&resource_compiler_opts_.data[0], (const void *)&data_path[0], sizeof(data_path));
  memcpy((void *)&resource_compiler_opts_.data_src[0], (const void *)&source_data_path[0], sizeof(source_data_path));
}

void Runner::run() {
  resource_compiler_ = ResourceCompiler::start(resource_compiler_opts_);

  if (watch_) {
    // TODO(mtwilliams): Install a signal handler to trap termination requests,
    // and call `ResourceCompiler::shutdown`.
    resource_compiler_->daemon();
  } else {
    resource_compiler_->compile(force_);
  }

  resource_compiler_->shutdown();
  resource_database_->close();
}

} // resource_compiler
} // yeti
