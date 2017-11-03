@rem Forwards to `7z` if in PATH, or to a local copy of 7-Zip if not found.

@echo OFF
@setlocal EnableDelayedExpansion

where /Q 7z

if %ERRORLEVEL% equ 0 (
  @rem 7-Zip is already in PATH!
  7z %*
  exit /B !ERRORLEVEL!
)

@rem Not found.

set _7zip=%~dp0\..\tools\cache\7za.exe
set _7zipVer=16.04
set _7zipSource=https://github.com/origamicomet/barebones/releases/download/v0.1.0/7za.exe

if exist %_7zip% (
  @rem Already installed.
  goto forward_to_cached
)

if defined DO_NOT_FETCH_TOOLS (
  echo Could not find 7-Zip!
  exit /B 1
)

PowerShell.exe -NoProfile -NonInteractive -ExecutionPolicy Unrestricted -File %~dp0\download.ps1 %_7zipSource% %_7zip%

if not %ERRORLEVEL% equ 0 (
  echo Failed to fetch 7-Zip for Windows!
  exit /B 1
)

:forward_to_cached

%_7zip% %*
