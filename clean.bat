@echo off

for /r %%f in (*.o) do (
  if exist %%f del %%f )
for /r %%f in (*.obj) do (
  if exist %%f del %%f )
for /r %%f in (*.exp) do (
  if exist %%f del %%f )
for /r %%f in (*.res) do (
  if exist %%f del %%f )
for /r %%f in (*.pdb) do (
  if exist %%f del %%f )
for /r %%f in (*.exe) do (
  if exist %%f del %%f )
for /r %%f in (*.lib) do (
  if exist %%f del %%f )
for /r %%f in (*.dll) do (
  if exist %%f del %%f )
for /r %%f in (*.elf) do (
  if exist %%f del %%f )
for /r %%f in (*.a) do (
  if exist %%f del %%f )
for /r %%f in (*.so) do (
  if exist %%f del %%f )
for /r %%f in (*.macho) do (
  if exist %%f del %%f )
for /r %%f in (*.dwarf) do (
  if exist %%f del %%f )
for /r %%f in (*.a) do (
  if exist %%f del %%f )
for /r %%f in (*.dylib) do (
  if exist %%f del %%f )
