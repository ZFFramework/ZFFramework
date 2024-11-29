@echo off
setlocal

set WORK_DIR=%~dp0
set DST_PATH=%~1%
set SRC_PATH=%~2%
set KEY_NAME=%~3%
set VALUE_FILE_PATH=%~4%

if not defined DST_PATH goto :usage
if not defined SRC_PATH goto :usage
if not defined KEY_NAME goto :usage
if not defined VALUE_FILE_PATH goto :usage
goto :run
:usage
echo usage:
echo   file_replace.bat DST_PATH SRC_PATH KEY_NAME VALUE_FILE_PATH
exit /b 1
:run

set /p _VALUE=<%VALUE_FILE_PATH%
if not defined _VALUE (
    echo no value in: %VALUE_FILE_PATH%
    exit /b 1
)

setlocal enabledelayedexpansion
del /f/s/q "%DST_PATH%" >nul 2>&1
for /f "tokens=*" %%a in ('type "%SRC_PATH%"') do (
    set line=%%a
    set line=!line:%KEY_NAME%=%_VALUE%!

    echo !line!>>"%DST_PATH%"
)

