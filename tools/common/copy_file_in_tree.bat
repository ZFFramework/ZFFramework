@echo off
setlocal

set WORK_DIR=%~dp0
set FILE_NAME=%~1%
set FROM_PATH=%~2%
set TO_PATH=%~3%

if not defined FILE_NAME goto :usage
if not defined FROM_PATH goto :usage
if not defined TO_PATH goto :usage
goto :run
:usage
echo usage:
echo   copy_file_in_tree.bat FILE_NAME FROM_PATH TO_PATH
echo .
echo FROM_PATH and TO_PATH must not end with '/'
exit /b 1
:run

xcopy /s/e/y/r/h "%FROM_PATH%\*%FILE_NAME%" "%TO_PATH%"

