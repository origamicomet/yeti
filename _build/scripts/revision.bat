@echo OFF

for /F "usebackq tokens=1" %%r in (`call %~dp0\git.bat rev-list --count HEAD`) do (
  <NUL set /p=%%r
)

@rem Return value will indicate failure, due to the nasty hack used to get
@rem output without an implicit newline.

exit /B 0
