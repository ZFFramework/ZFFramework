@echo off
setlocal
setlocal enabledelayedexpansion

set WORK_DIR=%~dp0
set SRC_URL=%~1%
set DST_PATH=%~2%
set FORCE=%~3%

if not defined SRC_URL goto :usage
if not defined DST_PATH goto :usage
goto :run
:usage
echo usage:
echo   file_download.bat SRC_URL DST_PATH [FORCE]
exit /b 1
:run

if exist "%DST_PATH%" (
    if not "%FORCE%" == "1" (
        exit /b 0
    )
)

for %%a in ("%DST_PATH%\..") do set _DST_PARENT=%%~fa
mkdir "%_DST_PARENT%" >nul 2>&1

call :DownloadFile "%SRC_URL%" "%DST_PATH%.tmp"
set _RESULT=%ERRORLEVEL%
if not "%_RESULT%" == "0" (
    del /f/s/q "%DST_PATH%.tmp" >nul 2>&1
    exit /b %_RESULT%
)
move /y "%DST_PATH%.tmp" "%DST_PATH%" >nul 2>&1
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
>>%vbs% echo if xHttp.Status = 200 then
>>%vbs% echo     WScript.Quit 0
>>%vbs% echo else
>>%vbs% echo     WScript.Quit 1
>>%vbs% echo end if
cscript //nologo %vbs%
set _vbs_RESULT=%ERRORLEVEL%
if exist %vbs% del /f /q %vbs%
exit /b %_vbs_RESULT%

