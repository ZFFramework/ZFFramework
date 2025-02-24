@echo off
setlocal

set WORK_DIR=%~dp0
set ZF_ROOT_PATH=%WORK_DIR%\..\ZFFramework

if not exist "%ZF_ROOT_PATH%\tools" (
    rmdir /s/q "%ZF_ROOT_PATH%" >nul 2>&1
    git clone "https://github.com/ZFFramework/ZFFramework.git" "%ZF_ROOT_PATH%"
)

call "%ZF_ROOT_PATH%\tools\common\zfsh_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_zf3rd_setup*.zfsh"
call "%ZF_ROOT_PATH%\tools\common\zfsh_recursive.bat" "%ZF_ROOT_PATH%" "zfautoscript_zfmodule.zfsh"

for %%i in ("%ZF_ROOT_PATH%") do set ABS_ZF_ROOT_PATH=%%~fi
for %%i in ("%WORK_DIR%") do set ABS_WORK_DIR=%%~fi
if not %ABS_ZF_ROOT_PATH% == %ABS_WORK_DIR% (
    call "%ZF_ROOT_PATH%\tools\common\zfsh_recursive.bat" "%WORK_DIR%" "zfautoscript_zf3rd_setup*.zfsh"
    call "%ZF_ROOT_PATH%\tools\common\zfsh_recursive.bat" "%WORK_DIR%" "zfautoscript_zfmodule.zfsh"
)

