@rem Recursively walks directories to generate the contents of a unity build file.

for %%D in (%*) do (
  for /F "usebackq tokens=*" %%F in (`dir /a-d /b /s %%D ^| findstr ".c .cc"`) do (
    echo #include "%%F"
  )
)
