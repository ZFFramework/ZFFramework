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
echo   buildapp_Qt.bat PROJ_NAME PROJ_PATH
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
set _TMP_PATH=%PROJ_PATH%\..\..\_tmp\Qt\%PROJ_NAME%\release

set _OLD_DIR=%cd%
cd /d "%PROJ_PATH%/Qt/%PROJ_NAME%"
set _PROJ_PATH=%cd%
cd /d "%_OLD_DIR%"

set _OLD_DIR=%cd%
mkdir "%_TMP_PATH%" >nul 2>&1
cd /d "%_TMP_PATH%"
qmake "%_PROJ_PATH%\%PROJ_NAME%.pro" CONFIG+=release
%ZF_QT_MAKE%
set _RESULT=%ERRORLEVEL%
cd /d "%_OLD_DIR%"

exit /b %_RESULT%

