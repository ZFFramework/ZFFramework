@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set SRC_PATH=%~1%
set DST_PATH=%~2%

if not defined SRC_PATH goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   unzip.bat SRC_PATH DST_PATH
exit /b 1
:run

mkdir "%DST_PATH%" >nul 2>&1

call :UnZipFile "%SRC_PATH%" "%DST_PATH%"

exit /b 0

:UnZipFile <SRC_PATH> <DST_PATH>
set vbs=%WORK_DIR%\..\..\_tmp\unzip.vbs
if exist "%vbs%" del /f /q "%vbs%"
for %%a in ("%vbs%\..") do set _vbs_PARENT=%%~fa
mkdir "%_vbs_PARENT%" >nul 2>&1
for %%i in ("%SRC_PATH%") do set _SRC_PATH=%%~fi
for %%i in ("%DST_PATH%") do set _DST_PATH=%%~fi
rmdir /s/q "%_DST_PATH%" >nul 2>&1
>"%vbs%"  echo Set fso = CreateObject("Scripting.FileSystemObject")
>>"%vbs%" echo If NOT fso.FolderExists("%_DST_PATH%") Then
>>"%vbs%" echo fso.CreateFolder("%_DST_PATH%")
>>"%vbs%" echo End If
>>"%vbs%" echo Set fso = Nothing
>>"%vbs%" echo set objShell = CreateObject("Shell.Application")
>>"%vbs%" echo set filesInZip=objShell.NameSpace("%_SRC_PATH%").items
>>"%vbs%" echo objShell.NameSpace("%_DST_PATH%").CopyHere(filesInZip)
>>"%vbs%" echo Set objShell = Nothing
cscript //nologo "%vbs%"
if exist "%vbs%" del /f /q "%vbs%"

