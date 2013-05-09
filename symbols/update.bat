@echo off

set _=%CD%
cd %_%/../
echo Copying butane/*.pdb(s)
for %%x in (*.pdb) do (
  copy /Y "%%x" "%_%\%%x" )
cd %_%/../deps/foundation
echo Copying butane/deps/foundation*.pdb(s)
for %%x in (*.pdb) do (
  copy /Y "%%x" "%_%\%%x" )
cd %_%/../deps/lua
echo Copying butane/deps/lua*.pdb(s)
for %%x in (*.pdb) do (
  copy /Y "%%x" "%_%\%%x" )
cd %_%
