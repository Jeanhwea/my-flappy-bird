@echo off
setlocal enabledelayedexpansion

echo Starting application packaging...

:: Get Git tag
git describe --tags --always --dirty="+dev" > git_tag.tmp
set /p GIT_TAG=<git_tag.tmp
del git_tag.tmp

:: Get operating system information
set OS_NAME=Windows
set OS_ARCH=x86_64

:: Set output directory
set OUT_DIR=output
set DST_DIR=dist
set APP_PKG=my-flappy-bird

:: Check if cmake is installed
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo cmake could not be found
    exit /b 1
)

:: Display cmake version
cmake --version

:: Generate Visual Studio project using cmake
cmake -B %OUT_DIR% -G"Visual Studio 17 2022" --log-level=STATUS

:: Build the project
cmake --build %OUT_DIR% --config Release

:: Create output directory
mkdir "%DST_DIR%" 2>nul

:: Copy build result to output directory
if exist "%OUT_DIR%\Release\%APP_PKG%.exe" (
    copy "%OUT_DIR%\Release\%APP_PKG%.exe" "%DST_DIR%"
) else (
    echo Build failed: %APP_PKG%.exe not found
    exit /b 1
)

:: Clean up temporary directory
rmdir /s /q "%OUT_DIR%" 2>nul

echo Packaging completed successfully!
