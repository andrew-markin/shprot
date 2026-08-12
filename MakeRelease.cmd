@echo off

if exist Release (rmdir /S /Q Release)
mkdir Release\Build
cd Release\Build

cmake ..\..\Sources -G "Visual Studio 17 2022" -A x64
cmake --build . --target PACKAGE --config Release --parallel 8

move *-*.zip ..
cd ..\..

rmdir /S /Q Release\Build
