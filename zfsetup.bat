@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\ZFFramework

call "%ZF_ROOT_PATH%\tools\common\timestamp_check.bat" "%WORK_DIR%\_tmp" _zfsetup_time_ 86400
if "!ERRORLEVEL!" == "0" (
    exit /b 0
)

if not exist "%ZF_ROOT_PATH%\tools" (
    rmdir /s/q "%ZF_ROOT_PATH%" >nul 2>&1
    git clone "https://github.com/ZFFramework/ZFFramework.git" "%ZF_ROOT_PATH%"
    if not "!ERRORLEVEL!" == "0" exit /b 1
)

call "%ZF_ROOT_PATH%\tools\common\zfsh_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_zf3rd_setup*.zfsh"
if not "!ERRORLEVEL!" == "0" exit /b 1
call "%ZF_ROOT_PATH%\tools\common\zfsh_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_zfmodule.zfsh"
if not "!ERRORLEVEL!" == "0" exit /b 1

for %%i in ("%ZF_ROOT_PATH%") do set ABS_ZF_ROOT_PATH=%%~fi
for %%i in ("%WORK_DIR%") do set ABS_WORK_DIR=%%~fi
if not %ABS_ZF_ROOT_PATH% == %ABS_WORK_DIR% (
    call "%ZF_ROOT_PATH%\tools\common\zfsh_recursive.bat" "%WORK_DIR%" "zfautoscript_zf3rd_setup*.zfsh"
    if not "!ERRORLEVEL!" == "0" exit /b 1
    call "%ZF_ROOT_PATH%\tools\common\zfsh_recursive.bat" "%WORK_DIR%" "zfautoscript_zfmodule.zfsh"
    if not "!ERRORLEVEL!" == "0" exit /b 1
)

call "%ZF_ROOT_PATH%\tools\common\timestamp_save.bat" "%WORK_DIR%\_tmp" _zfsetup_time_

