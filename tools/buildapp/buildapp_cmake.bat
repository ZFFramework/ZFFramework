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
echo   buildapp_cmake.bat PROJ_NAME PROJ_PATH
exit /b 1
:run

if "%CMAKE_BUILD_TYPE%" == "" (
    set _CONFIG=RelWithDebInfo
) else (
    set _CONFIG=%CMAKE_BUILD_TYPE%
)

set ZF_ROOT_PATH=%WORK_DIR%\..\..
set _OLD_DIR=%cd%

cd /d "%PROJ_PATH%/cmake/%PROJ_NAME%"
set _PROJ_PATH=%cd%
cd /d "%_OLD_DIR%"

mkdir "%PROJ_PATH%\..\..\_tmp\cmake\%PROJ_NAME%\%_CONFIG%" >nul 2>&1
cd /d "%PROJ_PATH%\..\..\_tmp\cmake\%PROJ_NAME%\%_CONFIG%"
set _TMP_PATH=%cd%
cd /d "%_OLD_DIR%"

mkdir "%_TMP_PATH%\zfdist" >nul 2>&1
cd /d "%_TMP_PATH%\zfdist"
set _INSTALL_PATH=%cd%
cd /d "%_OLD_DIR%"

cd /d "%_TMP_PATH%"
if "%CMAKE_CXX_FLAGS%" == "" (
    cmake -G "Ninja" ^
        -DCMAKE_BUILD_TYPE=%_CONFIG% ^
        -DCMAKE_INSTALL_PREFIX=%_INSTALL_PATH% ^
        "%_PROJ_PATH%"
) else (
    cmake -G "Ninja" ^
        -DCMAKE_BUILD_TYPE=%_CONFIG% ^
        -DCMAKE_INSTALL_PREFIX=%_INSTALL_PATH% ^
        -DCMAKE_CXX_FLAGS=%CMAKE_CXX_FLAGS% ^
        "%_PROJ_PATH%"
)
ninja
ninja install
set _RESULT=%ERRORLEVEL%
cd /d "%_OLD_DIR%"

call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%_INSTALL_PATH%\lib\cmake"
call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%_INSTALL_PATH%\lib\pkgconfig"
call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%_INSTALL_PATH%\lib\*SDL*_test.*"

call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%_INSTALL_PATH%\bin" "%PROJ_PATH%\..\..\_tmp\cmake\%PROJ_NAME%\app"
call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%_INSTALL_PATH%\lib" "%PROJ_PATH%\..\..\_tmp\cmake\%PROJ_NAME%\app"
call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%_INSTALL_PATH%\zfres" "%PROJ_PATH%\..\..\_tmp\cmake\%PROJ_NAME%\app\zfres"

exit /b %_RESULT%

