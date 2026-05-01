@echo off
C:\msys64\mingw64\bin\g++.exe ^
  -std=c++17 -O2 ^
  -IC:\msys64\mingw64\include\SDL2 ^
  -o "%~dp0orbitmed.exe" ^
  "%~dp0main.cpp" ^
  -LC:\msys64\mingw64\lib ^
  -lSDL2 -lSDL2_image -lSDL2_ttf ^
  -lmingw32 -lSDL2main ^
  -mwindows ^
  > "%~dp0build_log.txt" 2>&1
echo Exit code: %ERRORLEVEL% >> "%~dp0build_log.txt"
type "%~dp0build_log.txt"
