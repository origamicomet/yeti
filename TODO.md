# Yeti

## Engine

Includes runtime.

### `TODO`

* Inspect system to determine CPU, GPU, and RAM.
  * Provide a thorough `cpuid` implementation.
    * Build our own database of processors (and errata)?
* Add an `open_or_create` function to our fileystem abstraction layer.
  * Should open an existing file and append.
* Expose asynchronous I/O on files.
  * Tie into task scheduler, to allow offloading.
* Expose memory-mapped I/O on files.
  * Fallback for consoles?
* Provide platform independent Unicode conversion helpers.
  * Convert between UTF-8 and UTF-16.
  * Convert between UTF-8 and UTF-32.
* Provide platform independent Unicode normalization helpers.
  * NFC and NFD
* Provide a buddy allocator.
* Provide a dynamic string class `String` that a global buddy allocator.
* Handle large files in 32-bit builds.
  * Specify `_FILE_OFFSET_BITS=64` on POSIX targets.
* Improve random number generation.
  * Thread-localize random number generation.
  * Provide multiple strategies.
  * Provide multiple distributions.
* Expand cryptographic primitives.
  * Provide SHA-1 and SHA-256.
* Provide CRC32.
* Track storage device connects and disconnects.
* Track keyboard and mouse connects and disconnects.
  * Enumerate connected devices through `GetRawInputDeviceList` whenever a `WM_INPUT_DEVICE_CHANGE` arrives.
* Respect keyboard layout.
  * Handle different keyboard layouts through include file trickery?
  * Handle `WM_INPUTLANGCHANGE`.
  * Use `WM_CHAR` and friends for textual input.
* Extract mouse buttons and axes into an include file.
* Expose mouse grabbing.
* Expose setting of system cursor, either to standard or custom cursors.
* On Lua errors, capture Lua and C/C++ callstack (and frames), then forward to a user specified error handler.
* Expose Lua return values to C/C++.
* Support Lua 5.2 and 5.3.
  * Support registry with macro.
* Implement smoothed time-step-policy.
  * Also support debt payback using `wall`.
* Finish logging infrastructure.
  * Logging to console.
    * Colors!
  * Logging to network.
  * Logging to file.
* Finish `ResourceDatabase`.
* Use our own allocator for Lua in 64-bit builds.
  * Allocator needs to allocate within the first 2GiB of VM.
    * Map some amount of memory in lower parts during startup.
* Target Windows 7 and later.
  * Don't want to target Windows XP, but may have to for Asia.
  * Specify in manfiest.
    * Add a manifest (and use it!)
  * Define `WINVER` and `_WIN32_WINNT`.
    * Refer to https://msdn.microsoft.com/en-us/library/6sehtctf.aspx for details.
* Expand path manipulation utilities.
  * Provide a path joining function.
* Implement `Array<T>` sort and search.
* Polyfill type traits.
* Provide platform independent condition variables.
* Allow allocations to be tagged as "ignore" or "proxied."
* Actually load application manifest.
* Provide a default application icon.
  * Base off of Yeti logo.
* Allow user to provide a custom icon.
* Install a sophisticated assertion and error handler.
* Interface resource compiler with database.
* Make allocation tracking thread-safe.

### `BUGS`

* Global heap allocator doesn't respect alignment.
* Custom `new` and `delete` operators may not be working as expected.
  * Tag types with `YETI_ALLOCATOR_AWARE`?

### `PERF`

* Try to minimize impact on 32-bit builds.
  * Prefer (distinct) 32-bit implementations whever possible.
    * Random number generation.
    * Threads.
  * Audit fixed type usage.
    * Prefer `unsigned` and `size_t` where possible.
      * Especially loops.
* Audit and eliminate (or minimize) false sharing.
  * Especially in logging and profiling.
* Optimize `Script *` recovery from `lua_State *`.
* Support bulk operations on resources.
* Bundle resources then map `memory_resident_data` wholesale, providing slices instead of direct handles.
* Improve `Array<T>` memory profile during growth and shrinking.
* Provide an optimized resource database.
  * Qualities
    * Read-only
    * Fast to query
    * Perfect hashing in place of lookups for `path -> id`
  * Dispatch using a function pointer table that fits into one cache line, and 16-byte aligns each pointer (to use RIP relative addressing).
    * Dale Weiler had the neat idea of using a function that takes a pointer to this struct and returns it by value thereby forcing the compiler to load the function pointers into registers, at least on the x86_64 ABI. Care must be taken to prevent the compiler from inlining the function.

### `REFACTOR`

* Extract foundation into a library.
  * Move source tree to `origamicomet/ocf`.
* Convert `yeti::foundation::fs` to an interface?
  * Transparent virtual file-systems.
* Move to type-safe `Slice<T>`.
* Prefer `void *` over `uintptr_t`.
* Switch time-step policies by updating `desc_` and `state_` rather than creating a new `TimeStepPolicy`?
* Abstract `Resoruce::Data` I/O.
  * Future proofing for resource bundles.
* Allow hash used for hash tables to be overriden.
* Update `absolute` and `relative` mouse axes upon raw-input?
  * Manually translate `GetCursorPos` to a window-relative point?
* Pass `Task` to kernel and let kernel pull parameters?

### `SMELL`

* Containers do not call destructors for non-POD types.
* Log categories aren't under the `yeti` namespace.
* Returning pointers from `foundation::HashMap<K,V>`.

### `CRAZY`

* Move to 100% task-based multi-threading, i.e. only have worker threads.
  * Introduce a separate multi-producer, single-consumer queue for thread specific work, like message pump.
* Move to fibers a la Naughty Dog?
  * Overkill, probably. Mykola has been stupid enough to try, and he keeps having issues.
* Allow wholesale switch over to fixed-point math.
  * Might be necessary for determinism.
* Move to C++ style casts?

## Sherpa

* Move to Sherpa for managing dependencies.

## Console

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Resource Compiler

### `TODO`

* Allow more than one source data directory.
  * Rather than "layering" by building `core` and then building `vanguard`, we should build all directories together.
* Daemonization.
  * Watch source data directories for changes, debounce for a user configurable amount of time, and then build any new or modified resources.
* Detailed logging.
* Use edit-distance to detect misspellings.

### `BUGS`

### `PERF`

### `REFACTOR`

* Use `String` instead of munging ourselves.

### `SMELL`

## Resource Package & Bundle Manager

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Level Editor

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Entity & Prefab Editor

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Material Editor

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Particle Editor

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Script Editor & Debugger

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Profiler

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Audio Editor

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Cinematic Editor

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Localization Editor

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Neighborhood

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Launcher

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Crash Reporter

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

## Steam

### `TODO`

### `BUGS`

### `PERF`

### `REFACTOR`

### `SMELL`

---

# Vanguard

## Fighting Tech

* Determine if floating-point physics is deterministic enough.
* Damage modeling.

## Coat of Arms Tech

* Simple triangulator to render vector art.
* Cache to textures.

## Gore Tech

* Wounds
* Blood
* Gibs

## Destruction Tech

* Heightmap modification?
* Frostbite technique for buildings?
* How to simulate collapse?

## Quartermaster

* Build launcher that handles a URI.
  * Looks up randomly generated token passed via URI?
* Matchmaking.
* History.
* API

---

- Levels
  - Edit and play.
    - Compile a temporary level and package that includes unsaved changes.
    - Indicate, by setting an environment variable, that the application is running in the level editor.
    - Finally, forward the level editor's camera's position.

- Entities
  - Can inherit from other entities.
    - Mixes in all other components.
  - Can have (infinitely) nested children.
    - Levels are entities.
    - Prefabs are entities.
  - Compilation...
    - Merge changes at "source" level.
      - Lots of reads.
      - Every instantiation is unique. Means bloat.
        - Compiling a level is too slow for iteration.
          - Identify modified entities, map to existing instantiations, then update any inherited values.
        - Loading a level should be really quick though.
