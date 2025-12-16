@echo off
echo ========================================
echo    AMONG US GUI - BUILD SCRIPT
echo ========================================
echo.

REM Check if build directory exists
if not exist build (
    echo Creating build directory...
    mkdir build
)

echo Entering build directory...
cd build

echo Running CMake...
cmake .. -G "MinGW Makefiles"

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake configuration failed!
    echo Make sure you have CMake and MinGW installed.
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build .

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo    BUILD SUCCESSFUL!
echo ========================================
echo.
echo Executable location: build\bin\among_us_gui.exe
echo.
echo To run the game:
echo   cd build\bin
echo   among_us_gui.exe
echo.
pause
```

