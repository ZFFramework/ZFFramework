@echo off
setlocal

set WORK_DIR=%~dp0
set SRC_PATH=%~1%
set DST_PATH=%~2%

if not defined SRC_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   copy_res.bat SRC_PATH DST_PATH
exit /b 1
:run

mkdir "%DST_PATH%" >nul 2>&1
xcopy /s/e/y/r/h "%SRC_PATH%" "%DST_PATH%" >nul 2>&1

for /f "tokens=*" %%i in ('dir /s/b/a-d "%DST_PATH%" 2^>nul') do (
    if "%%~ni" == "_repo" (
        rmdir /s/q "%%i" >nul 2>&1
    )
    if "%%~ni" == "_tmp" (
        rmdir /s/q "%%i" >nul 2>&1
    )
    if "%%~ni" == "ZF_PUT_RES_FILES_HERE" (
        del /f/s/q "%%i" >nul 2>&1
    )
)
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"
for /d /r "%DST_PATH%" %%a in (*) do dir /b/a "%%a" 2>nul | findstr . >nul || rmdir "%%a"

exit /b 0

