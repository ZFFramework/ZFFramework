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

call "%ZF_ROOT_PATH%\tools\common\file_exist.bat" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\lib" "*%PROJ_NAME%*"
if "%ERRORLEVEL%" == "0" (
    goto :EOF
)

set _OLD_DIR=%cd%

mkdir "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\build" >nul 2>&1
cd /d "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\build"
cmake -G "Ninja" "%PROJ_PATH%\cmake\%PROJ_NAME%" -DCMAKE_BUILD_TYPE=RelWithDebInfo ^
    -DCMAKE_INSTALL_PREFIX="%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist"
ninja
ninja install
set RESULT=%ERRORLEVEL%

cd /d "%_OLD_DIR%"

call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist\include\SDL*"

call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist\lib\cmake"
call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist\lib\pkgconfig"
call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist\lib\*SDL*_test.*"

call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist\include" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\include"
call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist\lib" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\lib"
call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%ZF_ROOT_PATH%\_tmp\cmake\%PROJ_NAME%\zfdist\bin\zfres" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%\zfres"

call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%ZF_ROOT_PATH%\_release\cmake\module\%PROJ_NAME%" "%ZF_ROOT_PATH%\_release\cmake\all"
exit /b %RESULT%

