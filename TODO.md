# Yeti

### Global

- Move to Sherpa for managing dependencies.
- Use `yeti_assert(!"<message>")` instead of `yeti_assertf(0, "<message>")`.
- Move to C++ style casts?
- Target Windows XP or later by specifying `WINVER`.

### Foundation

- Extract from engine into `origamicomet/ocf`.
- Refactor `yeti::foundation::fs` into an interface?
- Handle multiple read/write calls internally.
- Asynchronous I/O on files.
- Memory-mapped I/O on files.
- Expose a platform independent way to watch a directory (and all its children) for modifications.
- Create a `path::join` function.
- Rename `thread_safe::ScratchAllocator` to `thread_safe::BlockAllocator`.
- Support up to 64 processors.
- A thorough `cpuid` implementation.
  - Build our own database of processors (and errata)?
- Implement `Array<T>` sort and search.
- Move to type-safe `Buffer<T>` and `Slice<T`>.
- Polyfill type-traits.
- Call destructors for non-POD types for collections.
- Special case array types passed to collections.
- Implement a lock-free multi-producer, single consumer queue, `thread_safe::MpMcQueue<T>`.
- Implement a random number generator.
- Implement a lock-free random number generator.
- Make sure distribution of constrained random numbers is even.
- Expose platform independent signalling events.
- Expose platform independent condition variables.
- Allow allocations to be tagged as "ignore" or "proxied."
- Respect alignment in all allocators.
- Improve `Array<T>` growth and shrink profile.
- Identify storage connect/disconnects.
- Tag custom new/delete so compiler doesn't placement new?
- Implement a `BuddyAllocator`.
- Provide a dynamic string class `String` that uses said buddy allocator.
- Bump default allocation alignment to 16 bytes across the board.

### Engine

- Extract mouse buttons and mouse axes into an include file.
- Respect keyboard layouts.
  - Handle `WM_INPUTLANGCHANGE` messages.
  - Specify keyboard layouts via include file trickery.
- Use `WM_CHAR` and friends for textual input.
- Implement `Keyboard::connected()` and `Mouse::connected()`.
  - Handle `WM_INPUT_DEVICE_CHANGE` messages.
- Implement `Window::mouse_focus()` and `Window::set_mouse_focus()`.
- Actually generate events in `Window::update()` for consumption by `event_handler`.
  - Remove window from `yeti::Application` when closed.
  - Quit if the main window is closed.
- Implement standard and custom cursors via `Window::cursor()` and `Window::set_cursor()`.
- Update `MouseAxes::ABSOLUTE` and `MouseAxes::RELATIVE` upon raw-input? We'll have to manually translate `GetCursorPos` to a window-relative point.
- Make allocation tracking thread-safe.
- Optimize allocation tracking via a different strategy. Refer to the EASTL talk that describes the use arenas.
- Switch time-step policies by updating `TimeStepPolicy::desc_` and `TimeStepPolicy::state_` rather than creating a new `TimeStepPolicy`.
- On Lua errors, capture Lua and C/C++ callstack (and frames), then forward to a user specifiable error handler.
- Expose Lua return values to C/C++.
- Lua 5.2 and 5.3 support.
- Support up to 63 worker threads.
- Implement a smoothed time-step policy.
- Profile round-robin versus random queue assignment for tasks.
- Investigate performance benefits of intrusive task permits.
- Audit and eliminate (or minimize) false sharing.
  - In task scheduler.
  - In logging.
- Implement work stealing.
- Implement `task_scheduler::do_work_while_waiting_for`.
- Move to 100% task-based multithreading, i.e. no longer maintain a main thread.
- Implement and test mouse and keyboard focus tracking.
  - Expose to Lua.
- Implement focus grabbing.
  - Expose to Lua.
- Finish logging infrastructure.
  - Log to console.
  - Log to file.
  - Log to network.
- Implement a SQLite3-powered `ResourceDatabase`.
- Use our own allocator for Lua on 64-bit.
  - Provide an allocator that alloctates within the first 2GiB of VM.
- Optimize `lua_State *` -> `Script *` recovery.
  - Use a bump pointer, i.e. store a pointer to self after `lua_State *`.
- Gracefully terminate worker threads, to prevent deadlocks.

### Runtime

- Load application manifest.
- Default application icon based on the Yeti logo.
- Determine the best approach to using a user-provided icon. Check my Github stars for that resource editor/embedding library. Or just load based on a manifest setting?
- Install a sophisticated assertion handler.
- Install a sophisticated error handler for exceptions, signals, and such.

### Resource Compiler

- Allow more than one source data directory.
  - Rather than "layering" by building `core` and then building `vanguard`, we should be able to build `core` and `vanguard` together.
- Interface with `ResourceDatabase`.
  - We also need to be able to pop out an `OptimizedResourceDatabase`.
- Daemonizable. Should watch source data directory for changes, debounce for a user configurable amount of time, and then build any new or modified resources.
- Don't use assertions for error handling.
- Detailed logging.
- Pattern-based ignore rules.
- Use edit-distance to detect misspellings.
- Properly track compilation success or failure.
- Use buddy-allocator backed strings for paths.

### Resource Package & Bundle Manager

- Scaffold.

# Milestones

## 0.3

### Console

- Scaffold.

### Profiler

- Scaffold.

### Launcher

- Scaffold.

### Crash Reporter

- Scaffold.

## 0.6

### Script Editor & Debugger

- Scaffold.

### Dia Editor

- Scaffold.

### Level Editor

- Scaffold.

### Material Editor

- Scaffold.

### Unit/Entity Editor

- Scaffold.

### Particle Editor

- Scaffold.

### UI Editor

- Scaffold.

### Sound Editor

- Scaffold. Extract into `origamicomet/gorilla` and `origamicomet/jungle`.

### Localization Editor

- Scaffold.

### Cinematic Editor

- Scaffold.

## 1.0

### Neighborhood

- Scaffold.

### Steam Plugin

- Scaffold plugin.

### PlayStation Network Plugin

- Scaffold plugin.

### Xbox Live Plugin

- Scaffold plugin.

### Analytics Plugin

- Scaffold plugin.
- Hook up to Eventlytics.

# Vanguard

### Coat of Arms

- Render vector graphic to texture atlas(es)?

### Gore Tech

- Scaffold plugin.

### Destruction Tech

- Scaffold plugin.

### Quartermaster

- Scaffold plugin.
- Build launcher that handles to a URI scheme.
  - Looks up randomly generated token passed via URI scheme?
