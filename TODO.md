# Yeti

## Engine

Includes runtime.

### `TODO`

* Provide platform independent Unicode conversion helpers.
  * Convert between UTF-8 and UTF-16.
  * Convert between UTF-8 and UTF-32.
* Provide platform independent Unicode normalization helpers.
  * NFC and NFD
* Make allocation tracking thread-safe.
* Allow allocations to be tagged as "ignore" or "proxied."
* Provide a buddy allocator.
* Provide a dynamic string class `String` that uses a global buddy allocator.
* Expand path manipulation utilities.
  * Provide a path joining function.
* Improve random number generation.
  * Provide multiple strategies.
  * Provide multiple distributions.
* Expand cryptographic primitives.
  * Provide SHA-256.
  * Provide CRC32.
* Polyfill type traits.
* Track storage device connects and disconnects.
* Handle large files in 32-bit builds.
  * Specify `_FILE_OFFSET_BITS=64` on POSIX targets.
* Expose asynchronous I/O on files.
  * Tie into task scheduler, to allow offloading.
* Expose memory-mapped I/O on files.
  * Fallback for consoles?
* Track keyboard and mouse connects and disconnects.
  * Enumerate connected devices through `GetRawInputDeviceList` whenever a `WM_INPUT_DEVICE_CHANGE` arrives.
* Respect keyboard layout.
  * Handle different keyboard layouts through include file trickery?
  * Handle `WM_INPUTLANGCHANGE`.
  * Use `WM_CHAR` and friends for textual input.
* Extract mouse buttons and axes into an include file.
* Expose setting of system cursor, either to standard or custom cursors.
  * Also allow showing/hiding of system cursor.
* Target Windows 7 and later.
  * Don't want to target Windows XP, but may have to for Asia.
  * Specify in manfiest.
    * Add a manifest (and use it!)
  * Define `WINVER` and `_WIN32_WINNT`.
    * Refer to https://msdn.microsoft.com/en-us/library/6sehtctf.aspx for details.
* Inspect system to determine CPU, GPU, and RAM.
  * Provide a thorough `cpuid` implementation.
    * Build our own database of processors (and errata)?
* Implement smoothed time-step-policy.
  * Also support debt payback using `wall`.
* Gracefully handle corrupt or malformed configuration and settings.
  * Automatically removed (or replaced with default).
    * Rename corrupt, rather than deleting?
  * Allow application to handle with `Application.did_settings_load_nicely()`.
* Provide a default application icon.
  * Base off of Yeti logo.
* Allow user to provide a custom icon.
* Finish logging infrastructure.
  * Logging to network.
  * Logging to file.
* Support Lua 5.2 and 5.3.
  * Support registry with macro.
* Use our own allocator for Lua in 64-bit builds.
  * Allocator needs to allocate within the first 2GiB of VM.
    * Map some amount of memory in lower parts during startup.
* On Lua errors, capture Lua and C/C++ callstack (and frames), then forward to a user specified error handler.
* Expose Lua return values to C/C++.
* Install a sophisticated assertion and error handler.
* Allow waiting on multiple events.
* Resource overrides.
* Drive `ResourceCompiler::compile` through `ResourceCompiler::process`.
* Break `ResourceCompiler::compile` into smaller functions.
* Move `ResourceCompiler` to asynchronous compilation model.
 * Use worker threads to multi-thread compilation.
* Time compilation of resources.
* Skip compilation if fingerprints match.
  * Only touch a file when a build is started.
* Make resource compilation atomic.
  * Build to temporary files and move or rename if successful.
* Boxing and unboxing of temporaries.
* Hooks for components to update, cull, render, reflect, and so on.
* Allow scripts to use constants for keys and mouse buttons.
  * Still allow dynamic look ups.
    * Improve look up performance using a hash table.
* Generic `panic` function.
  * Replace asserts to make purpose clearer.

### `BUGS`

* Containers do not call destructors for non-POD types.

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
  * Replace `Script::recover(L)` with upvalues.
    * Check for upvalue first?
* Support bulk operations on resources.
* Bundle resources then map `memory_resident_data` wholesale, providing slices instead of direct handles.
* Improve `Array<T>` memory profile during growth and shrinking.
* Provide an optimized resource database.
  * Qualities
    * Read-only
    * Fast to query
* Reducing binding glue when binding components to Lua.
* Investigate performance impact of using cdata for entity and component handles.
  * Nominally 4+4 (x86) or 8+4 (x86_64) bytes.
  * Could allocate from C.
    * Garbage collection to push onto free-list.
  * Type safety can be ensured, perhaps by using a lookaside.
    * Convert pointer to index, and lookup sister array.

### `REFACTOR`

* Extract core into a library.
  * Move source tree to `origamicomet/ocf`.
* Convert `yeti::core::fs` to an interface?
  * Transparent virtual file-systems.
* Move to type-safe `Slice<T>`.
* Prefer `void *` over `uintptr_t`.
* Switch time-step policies by updating `desc_` and `state_` rather than creating a new `TimeStepPolicy`?
* Abstract `Resoruce::Data` I/O.
  * Future proofing for resource bundles.
* Update `absolute` and `relative` mouse axes upon raw-input?
  * Manually translate `GetCursorPos` to a window-relative point?
* Provide a common `CommandLineParser` utility.
* Replace template specialization with namespace methods, as they cause issues with unity builds.

### `SMELL`

* Returning pointers from `yeti::core::Map<K,V>`.
* Namespace entities, components, and systems under `yeti::ecs`.
* Replace `YETI_CONFIGURATION == YETI_CONFIGURATION_XXX` with `YETI_IS_XXX`.
* Replace `YETI_PLATFORM == YETI_PLATFORM_XXX` with `YETI_ON_XXX`.

### `CRAZY`

* Move to 100% task-based multi-threading, i.e. only have worker threads.
  * Introduce a separate multi-producer, single-consumer queue for thread specific work, like message pump.
  * Use frames at a high level.
    * Allocate memory from a transient frame specific heap.
    * Update using a `yeti::Frame`.
      * Results from previous frames (up to a limit) can be accessed.
    * Explicit pipeline with stages:
      * Update
      * Reflect
      * Render and Audio
* Move to fibers a la Naughty Dog?
  * Overkill, probably. Mykola has been stupid enough to try, and he keeps having issues.
* Allow wholesale switch over to fixed-point math.
  * Might be necessary for determinism.
* Move to C++ style casts?
* Drop global heap allocator for a global page allocator.
  * Everything will need to be sub-allocated within page boundaries.
* Store transform, rotation, and scale components of local poses separately.
  * Off-the-cuff introduces a ~12% overhead when computing local-to-world.
    * We spend little time here anyway so it would have a small impact relative to everything else...

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

* Use a lock file to prevent users from running multiple resource compilers.
* Allow more than one source data directory.
  * Rather than "layering" by building `core` and then building `vanguard`, we should build all directories together.
* Daemonization.
  * Watch source data directories for changes, debounce for a user configurable amount of time, and then build any new or modified resources.
* Use edit-distance to detect misspellings.
* Allow user to specify compilation preferences.
  * Mark certain resources (or types of resources?) to only be built manually.
    * By default, mark anything that takes "the entire graph" as input as `BUILD_ON_REQUEST`. For example, global illumination.
    * Provide hints when we detect lots of shaders (or permutations)?
      * Anything with combinatorial explosion.
* Use a pool of sub-processes to compile resources.
  * On crash, mark the build as a failure.
* Orphan previously queued builds.
* Specifiable target platform.
  * Default to current.
  * Allow specification on command line with `--target windows|mac|linux|android|ios|ps4|xb1`.

### `BUGS`

### `PERF`

* Use task scheduler to multi-thread resource compilation.

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
