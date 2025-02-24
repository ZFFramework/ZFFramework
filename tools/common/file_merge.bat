@echo off
setlocal

set WORK_DIR=%~dp0
set DST_PATH=%~1%
set FILE1_PATH=%~2%

if not defined DST_PATH goto :usage
if not defined FILE1_PATH goto :usage
goto :run
:usage
echo usage:
echo   file_merge.bat DST_PATH FILE1_PATH [FILE2_PATH ...]
exit /b 1
:run

for %%a in ("%DST_PATH%\..") do set _DST_PARENT=%%~fa
mkdir "%_DST_PARENT%" >nul 2>&1

del /q "%DST_PATH%" >nul 2>&1
>nul 2>&1 (
    copy /b "%FILE1_PATH%" "%DST_PATH%"
)

shift
:all_file
shift
set FILEN_PATH=%~1%
if defined FILEN_PATH (
    >nul 2>&1 (
        copy /b "%DST_PATH%"+"%FILEN_PATH%" "%DST_PATH%"
    )
    goto :all_file
)

