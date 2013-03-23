@echo off

:Parse
if "%1"=="" (
  goto End
)

if "%1"=="--config" (
  if "%2"=="debug" (
    echo Updating tup.config
    echo CONFIG_BUILD=debug > tup.config
    goto End
  )

  if "%2"=="development" (
    echo Updating tup.config
    echo CONFIG_BUILD=development > tup.config
    goto End
  )

  if "%2"=="release" (
    echo Updating tup.config
    echo CONFIG_BUILD=release > tup.config
    goto End
  )

  shift
  shift
  goto Parse
)

if "%1"=="clean" (
  goto Clean
)

if "%1"=="build" (
  goto Build
)

shift
goto Parse

:Build
call "%VCINSTALLDIR%\vcvarsall.bat"
echo Run `tup upd`
goto End

:Clean
for /r %%f in (*.o) do (
  if exist %%f del %%f )
for /r %%f in (*.pdb) do (
  if exist %%f del %%f )
for /r %%f in (*.lib) do (
  if exist %%f del %%f )
goto End

:End