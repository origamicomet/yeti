@rem Collects various information about Yeti.

@echo OFF

for /f "tokens=2 delims==" %%r in ('wmic OS Get localdatetime /value') do (
  set T=%%r

  set YYYY=%T:~0,4%
  set MM=%T:~4,2%
  set DD=%T:~6,2%
  set HH=%T:~8,2%
  set MM=%T:~10,2%
  set SS=%T:~12,2%

  set TIMESTAMP=%YYYY%-%MM%-%DD% %HH%:%MM%:%SS%
)

set YETI_COPYRIGHT=Copyright 2012-%YYYY% Origami Comet Games, Inc.

for /f %%v in ('call %~dp0\scripts\version.bat') do (
  set YETI_VERSION=%%v
)

for /f %%v in ('call %~dp0\scripts\revision.bat') do (
  set YETI_REVISION=%%v
)
