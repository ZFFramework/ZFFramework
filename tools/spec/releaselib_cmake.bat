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
echo   releaselib_cmake.bat PROJ_NAME PROJ_PATH
exit /b 1
:run

set ZF_ROOT_PATH=%WORK_DIR%\..\..
set _OLD_DIR=%cd%

call "%ZF_ROOT_PATH%\tools\common\file_exist.bat" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\lib" "*%PROJ_NAME%*"
if "%ERRORLEVEL%" == "0" (
    goto :EOF
)

cd /d "%PROJ_PATH%\cmake\%PROJ_NAME%"
set _PROJ_PATH=%cd%
cd /d "%_OLD_DIR%"

mkdir "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\build" >nul 2>&1
cd /d "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\build"
set _TMP_PATH=%cd%
cd /d "%_OLD_DIR%"

mkdir "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist" >nul 2>&1
cd /d "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist"
set _INSTALL_PATH=%cd%
cd /d "%_OLD_DIR%"

cd /d "%_TMP_PATH%"
cmake -G "Ninja" ^
    -DCMAKE_BUILD_TYPE=RelWithDebInfo ^
    -DCMAKE_INSTALL_PREFIX="%_INSTALL_PATH%" ^
    "%_PROJ_PATH%"
ninja
ninja install
set _RESULT=%ERRORLEVEL%
cd /d "%_OLD_DIR%"

call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%_INSTALL_PATH%\include\SDL*"

call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%_INSTALL_PATH%\lib\cmake"
call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%_INSTALL_PATH%\lib\pkgconfig"
call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%_INSTALL_PATH%\lib\*SDL*_test.*"

call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%_INSTALL_PATH%\include" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\include"
call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%_INSTALL_PATH%\lib" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\lib"
call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%_INSTALL_PATH%\bin" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\lib"
call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%_INSTALL_PATH%\bin\zfres" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\zfres"

call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%" "%ZF_ROOT_PATH%\_release\cmake\all"

exit /b %_RESULT%

