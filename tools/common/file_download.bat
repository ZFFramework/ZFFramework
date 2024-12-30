@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set SRC_URL=%~1%
set DST_PATH=%~2%

if not defined SRC_URL goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   file_download.bat SRC_URL DST_PATH
exit /b 1
:run

for %%a in ("%DST_PATH%\..") do set _DST_PARENT=%%~fa
mkdir "%_DST_PARENT%" >nul 2>&1

call :DownloadFile "%SRC_URL%" "%DST_PATH%"

exit /b 0

:DownloadFile <SRC_URL> <DST_PATH>
set vbs=%WORK_DIR%\..\..\_tmp\file_download.vbs
if exist %vbs% del /f /q %vbs%
for %%a in ("%vbs%\..") do set _vbs_PARENT=%%~fa
mkdir "%_vbs_PARENT%" >nul 2>&1
>%vbs%  echo dim xHttp: Set xHttp = createobject("Microsoft.XMLHTTP")
>>%vbs% echo dim bStrm: Set bStrm = createobject("Adodb.Stream")
>>%vbs% echo xHttp.Open "GET", "%~1", False
>>%vbs% echo xHttp.Send
>>%vbs% echo with bStrm
>>%vbs% echo     .type = 1 '//binary
>>%vbs% echo     .open
>>%vbs% echo     .write xHttp.responseBody
>>%vbs% echo     .savetofile "%~2", 2 '//overwrite
>>%vbs% echo end with
cscript //nologo %vbs%
if exist %vbs% del /f /q %vbs%

