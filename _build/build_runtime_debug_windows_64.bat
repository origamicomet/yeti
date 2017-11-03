@echo OFF
@setlocal EnableDelayedExpansion

if defined VisualStudioVersion (
  set IDE=1
) else (
  set IDE=0
)

@rem TODO(mtwilliams): Cache environment.

if not defined TOOLCHAIN (
  if defined VisualStudioVersion (
    set TOOLCHAIN=%VisualStudioVersion%
  ) else (
    echo Using latest Visual Studio install... 1>&2
    set TOOLCHAIN=latest
  )
)

call %~dp0\scripts\vc.bat %TOOLCHAIN% windows x86_64

if not %ERRORLEVEL% EQU 0 (
  echo Could not setup environment for x86_64!
  exit /B 1
)

pushd %~dp0\..

mkdir _build\obj 2>NUL
mkdir _build\bin 2>NUL
mkdir _build\lib 2>NUL

:build

cl.exe /nologo /c /W4 /fp:except /favor:blend /Od /Oi ^
       /Gm- /GR- /EHa- /GS /MDd ^
       /Fo%_build\obj\runtime_debug_windows_64.obj ^
       /Zi /Fd%_build\runtime_debug_windows_64.pdb ^
       /DYETI_CONFIGURATION=YETI_CONFIGURATION_DEBUG ^
       /DYETI_LINKAGE=YETI_LINKAGE_STATIC ^
       /DLOOM_CONFIGURATION=LOOM_CONFIGURATION_DEBUG ^
       /DLOOM_LINKAGE=LOOM_LINKAGE_STATIC ^
       /DGALA_CONFIGURATION=GALA_CONFIGURATION_DEBUG ^
       /DGALA_LINKAGE=GALA_LINKAGE_STATIC ^
       /I_deps\luajit\include ^
       /I_deps\sqlite3\include ^
       /I_deps\loom\include ^
       /I_deps\gala\include ^
       /Iinclude /Isrc ^
       /Iruntime/include /Iruntime/src ^
       runtime\src\build.cc

if not %ERRORLEVEL% equ 0 (
  popd
  echo Compilation failed.
  exit /B 1
)

link.exe /nologo /machine:X64 /stack:0x400000,0x400000 ^
         /out:_build\bin\runtime_debug_windows_64.exe ^
         _build\obj\runtime_debug_windows_64.obj ^
         _build\lib\yeti_debug_windows_64.lib ^
         _deps\luajit\_build\lib\luajit_debug_windows_64.lib ^
         _deps\sqlite3\_build\lib\sqlite3_debug_windows_64.lib ^
         _deps\loom\_build\lib\loom_debug_windows_64.lib ^
         _deps\gala\_build\lib\gala_debug_windows_64.lib ^
         kernel32.lib user32.lib gdi32.lib ole32.lib advapi32.lib

if not %ERRORLEVEL% equ 0 (
  popd
  echo Linking failed.
  exit /B 1
)

echo Built `runtime_debug_windows_64.exe`.

popd
