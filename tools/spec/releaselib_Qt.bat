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
echo   releaselib_Qt.bat PROJ_NAME PROJ_PATH
exit /b 1
:run

setlocal enabledelayedexpansion
if not defined ZF_QT_MAKE (
    where /q nmake
    if "!ERRORLEVEL!" == "0" (
        rem MSVC
        set CL=/MP
        set ZF_QT_MAKE=nmake
    ) else (
        rem mingw
        set ZF_QT_MAKE=mingw32-make -j4
    )
)

set ZF_ROOT_PATH=%WORK_DIR%\..\..

call "%ZF_ROOT_PATH%\tools\common\file_exist.bat" "%ZF_ROOT_PATH%\_release\Qt\module\%PROJ_NAME%\lib" "*%PROJ_NAME%*"
if "%ERRORLEVEL%" == "0" (
    goto :EOF
)

set _OLD_DIR=%cd%
mkdir "%ZF_ROOT_PATH%\_tmp\Qt\%PROJ_NAME%\build" >nul 2>&1
cd /d "%ZF_ROOT_PATH%\_tmp\Qt\%PROJ_NAME%\build"
del /f/s/q ".\*.dll" /f/s/q/a >nul 2>&1
del /f/s/q ".\*.a" /f/s/q/a >nul 2>&1
qmake "%PROJ_PATH%\Qt\%PROJ_NAME%\%PROJ_NAME%.pro" CONFIG+=release
%ZF_QT_MAKE%
set RESULT=%ERRORLEVEL%
cd /d "%_OLD_DIR%"

exit /b %RESULT%

