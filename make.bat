@echo off

rem SOURCES
set SOURCES=../main.cpp
set SOURCES=%SOURCES% ../src/Chip8.cpp

rem INCLUDES
set INCLUDES=/I ../include
set INCLUDES=%INCLUDES% /I ../3rdparty/SDL3/include

rem LIBS
set LIBS=../3rdparty/SDL3/lib/x64/SDL3.lib

mkdir build
pushd build
cl -Zi /EHsc %SOURCES% %INCLUDES% /link %LIBS%
xcopy ..\3rdparty\SDL3\lib\x64\SDL3.dll . /d
popd build
