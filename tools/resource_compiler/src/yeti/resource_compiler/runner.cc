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
  , resource_compiler_(NULL) {
}

Runner::~Runner() {
}

void Runner::setup(const char *args[], const u32 num_args) {
  YETI_UNUSED(args);
  YETI_UNUSED(num_args);

  ResourceCompiler::Path cwd;
  foundation::path::cwd(&cwd[0], sizeof(cwd));

  ResourceCompiler::Path resource_database_path;
  sprintf(&resource_database_path[0], "%s/resources.db", &cwd[0]);
  resource_compiler_opts_.db = (ResourceDatabase *)~NULL; // ResourceDatabase::open(&resource_database_path[0]);

  sprintf(&resource_compiler_opts_.data[0], "%s/data", &cwd[0]);
  sprintf(&resource_compiler_opts_.data_src[0], "%s/data_src", &cwd[0]);
}

void Runner::run() {
  resource_compiler_ = ResourceCompiler::start(resource_compiler_opts_);
  resource_compiler_->compile();
  resource_compiler_->shutdown();
  // resource_database_->close();
}

} // resource_compiler
} // yeti
