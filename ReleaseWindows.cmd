@echo off

if exist ".env" (
    for /f "usebackq delims=" %%i in (`type ".env" ^| findstr /v "^#" ^| findstr /v "^$"`) do (
        set "%%i"
    )
)

if exist Release (rmdir /S /Q Release)
mkdir Release\Build
cd Release\Build

cmake ..\..\Sources -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="%QT_PREFIX%"
cmake --build . --target PACKAGE --config Release --parallel

move *-*.exe ..
cd ..\..

rmdir /S /Q Release\Build
