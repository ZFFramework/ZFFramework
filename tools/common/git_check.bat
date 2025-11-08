@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set PROJ_GIT=%~1%
set GIT_BRANCH=%~2%
set DST_PATH=%~3%
for /f "tokens=4,* delims= " %%a in ("%*") do set CLONE_OPTION=%%a

if not defined PROJ_GIT goto :usage
if not defined GIT_BRANCH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   git_check.bat PROJ_GIT GIT_BRANCH DST_PATH [options]
exit /b 1
:run
if not defined CLONE_OPTION set CLONE_OPTION=--depth=1

set _OLD_DIR=%cd%
set _NEED_PULL=0

set _GIT_VALID=0
if exist "%DST_PATH%\.git" (
    set _GIT_PATH_DESIRED=%DST_PATH%\.git
    cd /d "%_GIT_PATH_DESIRED%" >nul 2>&1
    set _GIT_PATH_DESIRED=%CD%
    cd /d "%_OLD_DIR%"

    cd /d "%DST_PATH%" >nul 2>&1
    for /f "tokens=*" %%i in ('git rev-parse --show-toplevel 2^>nul') do set _GIT_PATH_EXIST=%%i\.git
    cd /d "%_OLD_DIR%"
    cd /d "%_GIT_PATH_EXIST%" >nul 2>&1
    set _GIT_PATH_EXIST=%CD%
    cd /d "%_OLD_DIR%"

    if "%_GIT_PATH_DESIRED%" == "%_GIT_PATH_EXIST%" (
        set _GIT_VALID=1
    )
)
if "%_GIT_VALID%" == "1" (
    call "%WORK_DIR%\timestamp_check.bat" "%DST_PATH%\.git"
    if not "!ERRORLEVEL!" == "0" (
        set _NEED_PULL=1
    )
) else (
    set _NEED_PULL=1
    rmdir /s/q "%DST_PATH%\.git_check_tmp" >nul 2>&1
    mkdir "%DST_PATH%" >nul 2>&1
    set _DST_HAS_CONTENT=0
    for /f "delims=" %%i in ('dir /b /a "%DST_PATH%" 2^>nul') do (
        set _DST_HAS_CONTENT=1
    )
    git clone -b "%GIT_BRANCH%" %CLONE_OPTION% "%PROJ_GIT%" "%DST_PATH%\.git_check_tmp"
    set _CLONE_SUCCESS=!ERRORLEVEL!
    if not "!_CLONE_SUCCESS!" == "0" (
        rmdir /s/q "%DST_PATH%\.git_check_tmp" >nul 2>&1
        exit /b !_CLONE_SUCCESS!
    )
    rmdir /s/q "%DST_PATH%\.git" >nul 2>&1
    robocopy "%DST_PATH%\.git_check_tmp\.git" "%DST_PATH%\.git" /E /MOVE >nul 2>&1
    rmdir /s/q "%DST_PATH%\.git_check_tmp" >nul 2>&1
    if "!_DST_HAS_CONTENT!" == "0" (
        cd "%DST_PATH%"
        git checkout .
        cd "%_OLD_DIR%"
    )
)

if "!_NEED_PULL!" == "1" (
    cd /d "%DST_PATH%"
    git stash
    git checkout .
    git reset --hard
    git fetch --all && git pull
    set _SUCCESS=!ERRORLEVEL!
    if "!ERRORLEVEL!" == "0" (
        if exist ".gitmodules" (
            git submodule update --init --recursive --depth=1
            set _SUCCESS=!ERRORLEVEL!
        )
    )
    set _SUCCESS=!ERRORLEVEL!
    git checkout "%GIT_BRANCH%"
    git stash pop
    cd /d "%_OLD_DIR%"

    if "!_SUCCESS!" == "0" (
        call "%WORK_DIR%\timestamp_save.bat" "%DST_PATH%\.git"
    )
    exit /b !_SUCCESS!
)
exit /b 0

