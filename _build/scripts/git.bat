@rem Forwards to `git` if in PATH, or to a local copy of Git for Windows if
@rem not found.

@echo OFF
@setlocal EnableDelayedExpansion

where /Q git

if %ERRORLEVEL% equ 0 (
  @rem Git is already in PATH!
  git %*
  exit /B !ERRORLEVEL!
)

@rem Not found.

set _GitForWindows=%~dp0\..\tools\cache\git-for-windows
set _GitForWindowsVer=2.14.2
set _GitForWindowsSource=https://github.com/git-for-windows/git/releases/download/v%_GitForWindowsVer%.windows.1/PortableGit-%_GitForWindowsVer%-32-bit.7z.exe

if exist %_GitForWindows%\cmd\git.exe (
  @rem Already installed.
  goto forward_to_cached
)

if defined DO_NOT_FETCH_TOOLS (
  echo Could not find Git!
  exit /B 1
)

@rem Download.

PowerShell.exe -NoProfile -NonInteractive -ExecutionPolicy Unrestricted -File %~dp0\download.ps1 %_GitForWindowsSource% %_GitForWindows%.7z.exe

if not %ERRORLEVEL% equ 0 (
  echo Failed to fetch Git for Windows from Github!
  exit /B 1
)

@rem Extract.

%~dp0\7z.bat e -o%_GitForWindows% %_GitForWindows%.7z.exe

@rem Run post install script, otherwise Git for Windows won't work.

start "git-bash.exe --no-needs-console --hide --no-cd --command=%_GitForWindows%\post-install.cmd"

@rem Give it some time, then proceed.

timeout /t 5 /nobreak > NUL

:forward_to_cached

set gitdir=%_GitForWindows%

"%_GitForWindows%\cmd\git.exe" %*
