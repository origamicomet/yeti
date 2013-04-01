// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/application.h>

namespace bt {
namespace Application {
  void boot(
    size_t num_args_,
    const char* args_[] )
  {
    String symbol_path = Path::absolute(String(Allocator::heap(), "./"));
    const char* symbol_paths[] = { symbol_path.to_ptr() };
    if (!load_debug_information(&symbol_paths[0], 0))
      log("Unable to load debug information (path=%s)!\n", symbol_paths[0]);
    install_crash_handler();

    Array<const char*> args(Allocator::heap(), max((size_t)1, num_args_) - 1);
    for (size_t i = 1; i < num_args_; ++i)
      args[i - 1] = args_[i];

    if (args.size() >= 1) {
      if (strcmp("compile", args[0]) == 0) {
        compile(args);
        return;
      }
    }

    run(args);
  }

  // static void update(
  //   Script* script,
  //   const float delta_time )
  // {
  //   script->push(delta_time);
  //   script->call("update", 1);
  // }

  // static void render(
  //   Script* script )
  // {
  //   script->call("render", 0);  
  // }

  void run(
    const Array<const char*>& args )
  {
    for (size_t i = 0; i < args.size(); ++i) {
      if (strcmp("-log-to-file", args[i]) == 0) {
        if ((args.size() - i - 1) < 1) {
          log(
            "Malformed command-line argument (-log-to-file).\n"
            "  Expected: -log-to-file [path]\n"
            "                           ^ not supplied\n");
          break;
        }

        install_file_logger(args[i + 1]);
        i += 1;
      } else if (strcmp("-sr", args[i]) == 0) {
        unsigned w, h;

        if ((args.size() - i - 1) < 2) {
          log(
            "Malformed command-line argument (-sr).\n"
            "  Expected: -sr [width] [height]\n"
            "                   ^-------^-- not supplied\n");
          break;
        }

        if (sscanf(args[i + 1], "%u", &w) == 0) {
          log(
            "Malformed command-line argument (-sr).\n"
            "  Expected: -sr [width] [height]\n"
            "                   ^ non-negative number\n");
          break;
        }

        if (sscanf(args[i + 2], "%u", &h) == 0) {
          log(
            "Malformed command-line argument (-sr).\n"
            "  Expected: -sr [width] [height]\n"
            "                           ^ non-negative number\n");
          break;
        }
        
        i += 2;
      } else if (strcmp("-f", args[i]) == 0) {
        unsigned f;

        if ((args.size() - i - 1) < 1) {
          log(
            "Malformed command-line argument (-f).\n"
            "  Expected: -f [toggle]\n"
            "                   ^ not supplied\n");
          break;
        }

        if ((sscanf(args[i + 1], "%u", &f) == 0) || (f > 2))  {
          log(
            "Malformed command-line argument (-f).\n"
            "  Expected: -f [toggle]\n"
            "                  ^ 0 (windowed), 1 (fullscreen), 2 (fullscreen windowed)\n");
          break;
        }

        i += 1;
      } else if ((strcmp("-adapter", args[i]) == 0)) {
        unsigned adapter;

        if ((args.size() - i - 1) < 1) {
          log(
            "Malformed command-line argument (-adapter).\n"
            "  Expected: -adapter [id]\n"
            "                      ^ not supplied\n");
          break;
        }

        if (sscanf(args[i + 1], "%u", &adapter) == 0)  {
          log(
            "Malformed command-line argument (-adapter).\n"
            "  Expected: -adapter [id]\n"
            "                      ^ a non-negative number\n");
          break;
        }

        i += 1;
      } else if (strcmp("-vsync", args[i]) == 0) {
        unsigned vsync;

        if ((args.size() - i - 1) < 1) {
          log(
            "Malformed command-line argument (-vsync).\n"
            "  Expected: -vsync [toggle]\n"
            "                      ^ not supplied\n");
          break;
        }

        if ((sscanf(args[i + 1], "%u", &vsync) == 0) || (vsync > 1)) {
          log(
            "Malformed command-line argument (-vsync).\n"
            "  Expected: -vsync [toggle]\n"
            "                      ^ 0 (off), 1 (on)\n");
          break;
        }

        i += 1;
      } else {
        log("Unknown command-line argument (%s).\n", args[i]);
        break;
      }
    }
  }

  void compile(
    const Array<const char*>& args )
  {
  }

  void quit()
  {
    exit(EXIT_SUCCESS);
  }
} // Application
} // bt

int main( int argc, const char* argv[] ) {
  bt::Application::boot(argc, &argv[0]);
  return EXIT_FAILURE;
}