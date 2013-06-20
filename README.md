# Butane

Butane is a simple, lightweight, modular, optimized, and flexible game engine with a focus on rapid iteration and flexibility.

Simple
---

Countless rewrites and refactors have produced a meticulously and rigorously designed engine and [tools](http://en.wikipedia.org/wiki/Unix_philosophy).  Backed by well articulated, easy to follow, and modular code that makes development with Butane effortless.

Lightweight
---

Minimalism and pragmatism dominate; Butane clocks in just under 22k lines of code.

Modular
---

Don't pay for what you don't use.  Butane's modular design allows for deep customization.

Optimized
---

Careful attention to algorithims, data layouts, threading, and processor pipelines has fine-tuned Butane's performance.

Flexible
---

A completely data-driven architecture allows for rapid iteration of visuals and logic--without having to restart, or write a line of code.

# Meta

Contributing
---

Fork and submit a pull request. Just make sure your pull requests:

  * Are scoped to one specific issue;
  * Update any relevant documentation or build scripts;
  * Don't break the build;
  * and roughly match the coding style.

Building
---

Make sure you have the following:

  * A brain;
  * A C++ compiler (gcc, clang, msvc);
  * Ruby, and Python;
  * [Tup](https://github.com/gittup/tup);
  * and the necessary dependencies.

Then shell-fu:

```
$ git submodule init
$ env.bat (Windows) or ./env.sh (POSIX)
$ ruby configure {debug,development,release} --renderer={d3d11,opengl,gles}
$ tup init
$ tup upd
$ butane compile data_src data
$ butane
```

**Note:** if you're using Windows you'll need to make sure you set `VCINSTALLDIR` to something like `C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC`.  The configure script should automagically handle the Windows SDK, if not, set `WindowsSdkDir` to something like `C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A`.

License
---

See [LICENSE.md](LICENSE.md) for more details.
