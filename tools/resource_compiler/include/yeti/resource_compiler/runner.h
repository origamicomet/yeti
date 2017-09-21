//===-- yeti/resource_compiler/runner.h -----------------*- mode: C++11 -*-===//
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

#ifndef _YETI_RESOURCE_COMPILER_RUNNER_H_
#define _YETI_RESOURCE_COMPILER_RUNNER_H_

#include "yeti.h"

namespace yeti {
namespace resource_compiler {

class Runner {
 YETI_DISALLOW_COPYING(Runner)

 public:
  Runner();
  ~Runner();

 public:
  void setup(const char *args[], const u32 num_args);
  void run();

 private:
  ResourceDatabase *resource_database_;

  ResourceCompiler::Options resource_compiler_options_;
  ResourceCompiler *resource_compiler_;

  bool force_;
  bool watch_;
};

} // resource_compiler
} // yeti

#endif // _YETI_RESOURCE_COMPILER_RUNNER_H_
