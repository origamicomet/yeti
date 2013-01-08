Visual Studio
=============

1. Use [premake4](http://industriousone.com/premake) to generate the Vs2010 projects:

    $ cd build
    $ premake4 vs2010 --render-device=d3d11

2. Set the following configuration properties:

    i. C/C++ > Output Files:

        ASM List Location = $(IntDir)/\_/\_/\_/%(RelativeDir)
        Object File Name = $(IntDir)/\_/\_/\_/%(RelativeDir)
        XML Documentation File Name = $(IntDir)/\_/\_/\_/%(RelativeDir)

3. Build for debug, development, and release.