@echo off

echo Copying runtime dependencies...

set _=%CD%
cd %_%/deps/foundation
echo Copying butane/deps/foundation*.dll(s)
for %%x in (*.dll) do (
  copy /Y "%%x" "%_%\%%x" )
cd %_%/deps/lua
echo Copying butane/deps/lua*.dll(s)
for %%x in (*.dll) do (
  copy /Y "%%x" "%_%\%%x" )
cd %_%

echo Updating symbols...

cd symbols
call update.bat
cd ..
