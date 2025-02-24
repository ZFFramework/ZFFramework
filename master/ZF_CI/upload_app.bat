@echo off
setlocal

set WORK_DIR=%~dp0
set APP_PATH=%~1%
set REMOTE_FILE=%~2%

if not defined ZFCI_TOKEN (
    echo ZFCI_TOKEN not set
    exit /b 1
)

if not defined APP_PATH goto :usage
if not defined REMOTE_FILE goto :usage
goto :run
:usage
echo usage:
echo   upload_app.bat APP_PATH REMOTE_FILE
exit /b 1
:run

if not exist "%APP_PATH%" (
    echo file not exist: %APP_PATH%
    exit /b 1
)

echo "uploading app: %APP_PATH% => %REMOTE_FILE%"

set ZF_ROOT_PATH=%WORK_DIR%\..\..
set REPO_PATH=%ZF_ROOT_PATH%\_tmp\upload_app
set REPO_PATH_TMP=%ZF_ROOT_PATH%\_tmp\upload_app_tmp

call "%ZF_ROOT_PATH%\tools\common\git_check.bat" "https://github.com/ZFFramework/AppArchive" master "%REPO_PATH%"
set _OLD_DIR=%cd%
cd /d "%REPO_PATH%"
git config user.email "z@zsaber.com"
git config user.name "ZSaberLv0"
git config push.default "simple"
git checkout .
git reset --hard origin/master
git clean -xdf
git pull
cd /d "%_OLD_DIR%"

call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%REPO_PATH%" "%REPO_PATH_TMP%"
call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%REPO_PATH_TMP%\%REMOTE_FILE%"
call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%APP_PATH%" "%REPO_PATH_TMP%\%REMOTE_FILE%"

cd /d "%REPO_PATH%"
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch *' --prune-empty --tag-name-filter cat -- --all
cd /d "%_OLD_DIR%"

call "%ZF_ROOT_PATH%\tools\common\copy_check.bat" "%REPO_PATH_TMP%" "%REPO_PATH%"

cd /d "%REPO_PATH%"
git add -A
git commit -a -m "update by CI"
git push --force "https://ZSaberLv0:%ZFCI_TOKEN%@github.com/ZFFramework/AppArchive"
cd /d "%_OLD_DIR%"

REM call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%REPO_PATH%"
call "%ZF_ROOT_PATH%\tools\common\zfsh\rm.bat" "%REPO_PATH_TMP%"

