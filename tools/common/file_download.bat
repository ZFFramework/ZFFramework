@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set SRC_URL=%~1%
set DST_PATH=%~2%

if not defined SRC_URL goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   file_download.bat SRC_URL DST_PATH
exit /b 1
:run

for %%a in (%~f2\..) do set DST_PARENT=%%~fa
mkdir "%DST_PARENT%" >nul 2>&1

bitsadmin.exe /transfer "file_download.bat" "%SRC_URL%" "%DST_PATH%"

exit /b 0

