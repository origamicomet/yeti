# Yeti

### Global

- Move to Sherpa for managing dependencies.
- Use `yeti_assert(!"<message>")` instead of `yeti_assertf(0, "<message>")`.
- Move to C++ style casts?

### Foundation

- Extract from engine into `origamicomet/ocf`.
- Refactor `yeti::foundation::fs` into an interface?
- Asynchronous I/O on files.
- Memory-mapped I/O on files.

### Engine

- Extract mouse buttons and mouse axes into an include file?
- Respect keyboard layouts?
- Use `WM_CHAR` and friends for textual input.
- Implement `Keyboard::connected()` and `Mouse::connected()`.
- Implement `Window::mouse_focus()` and `Window::set_mouse_focus()`.
- Actually generate events in `Window::update()` for consumption by `event_handler`.
- Implement standard and custom cursors via `Window::cursor()` and `Window::set_cursor()`.
- Update `MouseAxes::ABSOLUTE` and `MouseAxes::RELATIVE` upon raw-input? We'll have to manually translate `GetCursorPos` to a window-relative point.
- Make allocation tracking thread-safe.
- Optimize allocation tracking via a different strategy. Refer to the EASTL talk that describes the use arenas.
- Switch time-step policies by updating `TimeStepPolicy::desc_` and `TimeStepPolicy::state_` rather than creating a new `TimeStepPolicy`.

### Runtime

- Default application icon based on the Yeti logo.
- Determine the best approach to using a user-provided icon. Check my Github stars for that resource editor/embedding library. Or just load based on a manifest setting?
- Install a sophisticated assertion handler.
- Install a sophisticated error handler for exceptions, signals, and such.

### Resource Compiler

- Scaffold.

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

### XboxLive Plugin

- Scaffold plugin.

### Analytics Plugin

- Scaffold plugin.
- Hook up to Eventlytics.

# Vanguard

### Gore Tech

- Scaffold plugin.

### Destruction Tech
- Scaffold plugin.
