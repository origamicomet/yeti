@rem Recursively walks directories to generate the contents of a unity build file.

@echo OFF

for %%D in (%*) do (
  for /F "usebackq tokens=*" %%F in (`dir /a-d /b /s %%D ^| findstr /L ".c .cc"`) do (
    echo #include "%%F"
  )
)
