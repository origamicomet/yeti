#ifdef _MSC_VER
  // Force Visual C/C++ to shut up about non-standard extensions.
  #pragma warning(disable: 4200 4201)

  // Disable warnings about types marked public when building shared libraries.
  #pragma warning(disable: 4251)

  // Disable warnings about synchronous exception handling.
  #pragma warning(disable: 4577)
#endif

#if defined(_WIN32) || defined(_WIN64)
  // We target Windows 7 and later.
  #define WINVER 0x0601
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define WIN32_LEAN_AND_MEAN

  // There's so much crap...
  #define NOGDICAPMASKS
  #define NORASTEROPS
  #define NOCOLOR
  #define NODRAWTEXT
  #define NOMB
  #define NOMEMMGR
  #define NOMETAFILE
  #define NOMINMAX
  #define NOOPENFILE
  #define NOSCROLL
  #define NOSOUND
  #define NOCOMM
  #define NOKANJI
  #define NOHELP
  #define NODEFERWINDOWPOS
  #define NOMCX

  // Preemptively include `windows.h` and `windowsx.h` and unfuck afterwords.
  #include <windows.h>
  #include <windowsx.h>

  #undef ABSOLUTE
  #undef RELATIVE

  #undef NEAR
  #undef FAR

  #undef near
  #undef far

  #undef DELETE

  #undef ERROR
#endif

//
// Core
//

// String munging.
#include "yeti/core/support/strings.cc"

// Assertion handling.
#include "yeti/core/debug/assert.cc"

// Pluggable logging infrastructure.
#include "yeti/core/log.cc"

// Logging backends.
#include "yeti/core/log/console.cc"
#include "yeti/core/log/file.cc"
#include "yeti/core/log/network.cc"

// Common logic shared between allocators.
#include "yeti/core/allocator.cc"

// Application wide allocators.
#include "yeti/core/allocators/global_heap_allocator.cc"
#include "yeti/core/allocators/global_page_allocator.cc"

// Proxying allocator for tracking allocations by logical unit.
#include "yeti/core/allocators/proxy_allocator.cc"

// Various arenas.
#include "yeti/core/allocators/bump_allocator.cc"
#include "yeti/core/allocators/buddy_allocator.cc"

// Thread-safe variants.
#include "yeti/core/allocators/thread_safe/bump_allocator.cc"
#include "yeti/core/allocators/thread_safe/scratch_allocator.cc"

// Hashing algorithims, like FN1VA.
#include "yeti/core/algorithms/hash.cc"

// Digest algorithims, like SHA-1.
#include "yeti/core/algorithms/digest.cc"

// Pseudo-random number generator.
#include "yeti/core/algorithms/random.cc"

// System information.
// #include "yeti/core/platform/info.cc"

// Environment control.
// #include "yeti/core/platform/environment.cc"

// Thread and process control.
#include "yeti/core/platform/thread.cc"
#include "yeti/core/platform/process.cc"

// Synchronization primitives.
#include "yeti/core/platform/event.cc"
#include "yeti/core/platform/lock.cc"
#include "yeti/core/platform/reader_writer_lock.cc"

// Path handling.
#include "yeti/core/platform/path.cc"

// Filesystem abstraction.
#include "yeti/core/platform/filesystem.cc"

// Cryptographically secure entropy.
#include "yeti/core/platform/entropy.cc"

// High-resolution timing.
#include "yeti/core/platform/timer.cc"

// Generic data format parsers.
// #include "yeti/core/misc/ini.cc"
// #include "yeti/core/misc/sjson.cc"

// Git-esque pattern file parser.
#include "yeti/core/misc/pattern_file_parser.cc"

// Globally unique identifiers.
// #include "yeti/core/misc/uuid.cc"

//
// Math
//

#include "yeti/math/vec2.cc"
#include "yeti/math/vec3.cc"
#include "yeti/math/vec4.cc"

#include "yeti/math/quaternion.cc"

#include "yeti/math/mat4.cc"

//
// Engine
//

#include "yeti.cc"

#include "yeti/application.cc"
#include "yeti/application/time_step_policy.cc"

#include "yeti/entity.cc"
#include "yeti/component.cc"
#include "yeti/system.cc"

#include "yeti/components/transform.cc"
#include "yeti/components/camera.cc"
#include "yeti/components/light.cc"

#include "yeti/world.cc"

#include "yeti/level.cc"

#include "yeti/resource.cc"
#include "yeti/resource_package.cc"
#include "yeti/resource_bundle.cc"
#include "yeti/resource_manager.cc"
#include "yeti/resource_database.cc"
#include "yeti/optimized_resource_database.cc"
#include "yeti/sophisticated_resource_database.cc"
#include "yeti/resource_compiler.cc"

#include "yeti/resources/entity_resource.cc"
#include "yeti/resources/entity_compiler.cc"

// Prefabs are just glorified entities.
#include "yeti/resources/prefab_resource.cc"

#include "yeti/resources/entity_cache.cc"

#include "yeti/resources/level_resource.cc"
#include "yeti/resources/level_compiler.cc"

#include "yeti/resources/texture_resource.cc"
#include "yeti/resources/texture_compiler.cc"

#include "yeti/resources/mesh_resource.cc"
#include "yeti/resources/geometry_compiler.cc"

#include "yeti/resources/script_resource.cc"
#include "yeti/resources/script_compiler.cc"

#include "yeti/window.cc"

#include "yeti/input/keyboard.cc"
#include "yeti/input/mouse.cc"
#include "yeti/input/feeder.cc"

#include "yeti/script.cc"
#include "yeti/script/environment.cc"
#include "yeti/script/bindings/math_if.cc"
#include "yeti/script/bindings/application_if.cc"
#include "yeti/script/bindings/window_if.cc"
#include "yeti/script/bindings/viewport_if.cc"
#include "yeti/script/bindings/keyboard_if.cc"
#include "yeti/script/bindings/mouse_if.cc"
#include "yeti/script/bindings/world_if.cc"
#include "yeti/script/bindings/entity_if.cc"
#include "yeti/script/bindings/component_if.cc"
#include "yeti/script/bindings/transform_if.cc"
#include "yeti/script/bindings/camera_if.cc"
#include "yeti/script/bindings/light_if.cc"

#include "yeti/renderer.cc"

#include "yeti/task.cc"
#include "yeti/task_scheduler.cc"
