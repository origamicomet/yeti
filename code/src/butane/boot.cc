// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application.h>

#include <locale.h>

int main( int argc, const char* argv[] ) {
  const foundation::LogScope _("Engine");
  setlocale(LC_ALL, "en-US.UTF8");
  butane::Application::boot(argc, &argv[0]);
  return EXIT_FAILURE;
}
