# Butane

## Platforms

| Platform  | Toolchain               | Supported             |
|:--------- |:----------------------- |:---------------------:|
| Windows   | Microsoft Visual Studio |                       |
| Windows   | MinGW/MSYS              | <span>&#10003;</span> |
| Mac OSX   | GCC or Clang            |                       |
| Linux     | GCC or Clang            |                       |
| BSD       | GCC or Clang            |                       |
| Android   | GCC or Clang            |                       |
| iOS       | GCC or Clang            |                       |
| Web       | Native Client (NaCl)    |                       |
| Web       | emscripten              |                       |

## Building

### Windows

You have two options:

  * MinGW/MSYS;
  * or Microsoft Visual Studio.

Microsoft Visual Studio recommended for performance, debugging, and support reasons.

#### via MinGW

Before you run `make` you're going to need:

  * MinGW/MSYS;
  * Ruby (1.9.3);
  * GCC (preferably the latest and greatest);
  * and a brain.

##### 1. Open a MinGW/MSYS terminal

Assuming default install options, you should be able to do so by running `C:/MinGW/msys/1.0/msys.bat`.

##### 2. Configure

You don't need to specify anything; it should just work.

```
$ ./configure
```

##### 3. Build

```
$ make all
```

#### via Microsoft Visual Studio

Before you run `make` you're going to need:

  * MinGW/MSYS;
  * Ruby (1.9.3);
  * Microsoft Visual Studio;
  * and a brain.

##### 1. Open a MinGW/MSYS terminal

Assuming default install options, you should be able to do so by running `C:/MinGW/msys/1.0/msys.bat`.

##### 2. Configure

You're going to need to specify the `toolchain` and `vcvars` options, e.g.:

```
$ ./configure --toolchain=msvc --vcvars="C:/Program Files (x86)/Microsoft Visual Studio 11.0/VC/vcvarsall.bat"
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

#### via [Native Client (NaCl)](https://developers.google.com/native-client/)

TODO

#### via [emscripten](https://github.com/kripken/emscripten)

TODO

## Configuring

Butane's compile, link, and runtime behaviour is controlled by `./configure` switches and options, as well as various pre-processor defines (which are documented in `include/bt/config.h`).

### via switches and options

**--paranoid**: defines `BT_PARANOID`, which will enable paranoid checks.

**--config=debug**: defines `BT_DEBUG` and `BT_DEVELOPMENT` and `BT_RELEASE`, which will enable debugging, development, and release checks; disables optimizations; and enables symbol generation.

**--config=development**: defines `BT_DEVELOPMENT` and `BT_RELEASE`, which will enable development and release checks; enables optimizations; and enables symbol generation.

**--config=release**: defines `BT_RELEASE`, which will enable **only** release checks; enables optimizations; and disables symbol generation.

**--architecture=[x86,x86-64,arm]**: compiles for the specified architecture.

**--toolchain=[gcc,clang,msvc]**: uses specifed toolchain.

**--platform=[windows,macosx,linux]**: builds for the specified platform.

**--vcvars=[path]**: is required when using Microsoft Visual Studio.

## License

Copyright (c) Michael Williams <devbug@bitbyte.ca>

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. Neither the name of the nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
