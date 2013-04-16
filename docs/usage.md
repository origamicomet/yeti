```
  usage: butane [options]
    runs the game engine

  or: butane compile [file, or directory]
    compiles a file, or a directory of resources

  options (default):
      -log-to-file [path] logs all console output to a file
        -log-to-file butane.log

      -log-to-network [ipv4:port] logs all console output to the remote console
        -log-to-network 127.0.0.1:4000

      -sr [width] [height] overrides config's screen resolution
        -sr 1920 1080

      -f [toggle] overrides config's fullscreen toggle
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

      -log-to-network [ipv4/6, or hostname] logs all compile output to a remote server over http
        -lot-to-network resource.build.voxkings.com
```