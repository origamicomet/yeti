Table of Contents
===

 * § 1 - Running the Engine and Tools
  * § 1.1 - Command Line Arguments
  * § 1.2 - Deploying to Other Devices

§ 1 - Running the Engine and Tools
---

§ 1.1 - Command Line Arguments
---

If you want to run the game or compile resources from the command line, follow this:

```
  usage: butane [options]
    runs the game engine

  or: butane compile [source data directory] [data directory] [options]
    compiles a directory of resources

  options (default):
      -log-to-file [path] logs all console output to a file
        -log-to-file butane.log

      -log-to-network [address or hostname] logs all console output to a remote server over TCP
        -log-to-network logs.voxkings.com:5001

      -resolution -sr [width] [height] overrides config's screen resolution
        -sr 1920 1080

      -fullscreen -f [toggle] overrides config's fullscreen toggle
        -f 0 # windowed mode
        -f 1 # fullscreen mode
        -f 2 # fullscreen windowed mode

      -adapter [id] overrides config's adapter (gfx card used)
        -adapter 0 # use the default graphics adapter

      -vsync [toggle] overrides config's vsync toggle
        -vsync 0 # disable vertical sync
        -vsync 1 # enable vertical sync

  options (compile):
      -daemon runs the server as a daemon/service (and compiles on file system events)

      -log-to-network [address or hostname] logs all compile output to a remote server over TCP
        -log-to-network build.voxkings.com:5000
```

If you want to connect a remote console to an engine instance from the command line, follow this:

```
 usage: console [address:port] [options]
    connects to an engine instance and displays a Lua REPL

  options:
      -timeout -t [timeout] overrides the default timeout in seconds
        -t 10 # will stop trying to connect after ten seconds
```

If you want to connect a profiler to an engine instance from the command line, follow this:

```
 usage: profiler [address:port] [options]
    connects to an engine instance and profiles C/C++ and Lua code

  options:
      -timeout -t [timeout] overrides the default timeout in seconds
        -t 10 # will stop trying to connect after ten seconds
```

If you want to connect a Lua debugger to an engine instance from the command line, follow this:

```
 usage: lua_debugger [address:port] [options]
    connects to an engine instance and provides a Lua debugger

  options:
      -timeout -t [timeout] overrides the default timeout in seconds
        -t 10 # will stop trying to connect after ten seconds
```

If you want to deploy updated code and data to another device over the network from the command line, follow this:

```
 usage: deploy [project] [address:port] [options]
    deploys updated code and data (from the project directory) to a remote device over the network

  options:
      -run -r runs the engine after deploying

      -force-stop -fs stops the engine if it's running prior to deploying

      -timeout -t [timeout] overrides the default timeout in seconds
        -t 10 # will stop trying to connect after ten seconds
```

§ 1.2 - Deploying to Other Devices
---

