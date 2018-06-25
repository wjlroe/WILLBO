@echo off

set PROJECT_DIR=%~dp0
cd %PROJECT_DIR%\cmake-build-debug
make
cd %PROJECT_DIR%
.\cmake-build-debug\WILLBO.exe
