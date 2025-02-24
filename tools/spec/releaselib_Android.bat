@echo off
setlocal

set WORK_DIR=%~dp0
set PROJ_NAME=%~1%
set PROJ_PATH=%~2%

if not defined PROJ_NAME goto :usage
if not defined PROJ_PATH goto :usage
goto :run
:usage
echo usage:
echo   releaselib_Android.bat PROJ_NAME PROJ_PATH
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..

call "%ZF_ROOT_PATH%\tools\common\file_exist.bat" "%ZF_ROOT_PATH%\_release\Android\module\%PROJ_NAME%\aar" "*.aar"
if "%errorlevel%" == "0" (
    exit /b 0
)

set _OLD_DIR=%cd%

cd /d "%PROJ_PATH%\Android\%PROJ_NAME%"
call "gradlew.bat" assembleRelease
set RESULT=%errorlevel%

cd /d "%_OLD_DIR%"

exit /b %RESULT%

