@echo off
setlocal

set WORK_DIR=%~dp0
set CHECK_ONLY=%~1%
set ZF_ROOT_PATH=%WORK_DIR%\..\..

if not defined CHECK_ONLY goto :run
if not exist "%ZF_ROOT_PATH%\_release\cmake\all\lib" goto :run
exit /b 0
:run

call "%ZF_ROOT_PATH%\zfsetup.bat"
call "%ZF_ROOT_PATH%\tools\common\zfsh.bat" "%WORK_DIR%\release.zfsh" cmake

