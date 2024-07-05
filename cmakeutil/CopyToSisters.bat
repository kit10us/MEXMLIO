setlocal
#@echo off

set SOURCE_DIR=%~dp0..\cmakeutil
set INTEROP_DIR=%~dp0..\..\interop
set MEXMLIO_DIR=%~dp0..\..\MEXMLIO

if EXIST "%INTEROP_DIR%" (
	if NOT EXIST "%INTEROP_DIR%" (
		mkdir "%INTEROP_DIR%"
	)
	copy "%SOURCE_DIR% "%INTEROP_DIR%"
)

if EXIST "%MEXMLIO_DIR%" (
	if NOT EXIST "%MEXMLIO_DIR%" (
		mkdir "%MEXMLIO_DIR%"
	)
	copy "%SOURCE_DIR%" "%MEXMLIO_DIR%"
)

endlocal