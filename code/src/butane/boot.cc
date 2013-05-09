// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application.h>

int main( int argc, const char* argv[] ) {
  foundation::LogScope log_scope("Engine");
  butane::Application::boot(argc, &argv[0]);
  return EXIT_FAILURE;
}
