# Butane

A data-driven game engine.

## Supports

| Platform  | Toolchain               | Supported             |
|:--------- |:----------------------- |:---------------------:|
| Windows   | Microsoft Visual Studio | <span>&#10003;</span> |
| Windows   | MinGW/MSYS              | <span>&#10003;</span> |
| Windows   | Cygwin                  |         Never         |
| Mac OSX   | GCC or Clang            |                       |
| Linux     | GCC or Clang            |                       |
| BSD       | GCC or Clang            |                       |
| Android   | GCC or Clang            |                       |
| iOS       | GCC or Clang            |                       |
| Web       | Native Client (PNaCl)   |                       |
| Web       | emscripten              |                       |

## Building

### Windows

You have two options:

  * MinGW/MSYS;
  * or Microsoft Visual Studio.

Microsoft Visual Studio is recommended for performance, debugging, and support reasons.

#### via MinGW/MSYS

Before you run `make` you're going to need:

  * MinGW/MSYS;
  * Ruby 1.9.3+;
  * and a brain.

##### 1. Open a MinGW/MSYS terminal

Assuming default install options, you should be able to do so by running `C:/MinGW/msys/1.0/msys.bat`.

##### 2. Configure

You don't need to specify anything; the defaults will work:

```
$ ./configure --paranoid --config=debug
```

##### 3. Build

```
$ make all
```

#### via Microsoft Visual Studio

Before you run `make` you're going to need:

  * MinGW/MSYS;
  * Ruby 1.9.3+;
  * Microsoft Visual Studio;
  * and a brain.

##### 1. Open a MinGW/MSYS terminal

Assuming default install options, you should be able to do so by running `C:/MinGW/msys/1.0/msys.bat`.

##### 2. Configure

You're going to need to specify the `toolchain`, `windows-sdk`, and `vs-path` options:

```
$ ./configure --paranoid --config=debug --toolchain=msvc \
  --windows-sdk="C:/Program Files/Microsoft SDKs/Windows/v7.1" \
  --vs-path="C:/Program Files (x86)/Microsoft Visual Studio 11.0" \
  --architecture=x86-64
```

##### 3. Build

```
$ make all
```

### Mac OSX

TODO

### Linux

TODO

### BSD

TODO

### Android

TODO

### iOS

TODO

### Web

#### via [Native Client (PNaCl)](https://developers.google.com/native-client/)

TODO

#### via [emscripten](https://github.com/kripken/emscripten)

TODO

## Configuring

Butanes's compile-, link-, and runtime behaviour is controlled by `./configure` switches and options, as well as various pre-processor defines (which are documented in `include/butane/config.h`).

### via switches and options

TODO

## License

Copyright (c) Michael Williams <devbug@bitbyte.ca>

Author(s):

  * Michael Williams <devbug@bitbyte.ca>

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. Neither the name of the nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
