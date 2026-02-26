@echo off
setlocal enabledelayedexpansion

:: 设置变量
for /f "tokens=*" %%i in ('git describe --tags --always --dirty="+dev"') do set GIT_TAG=%%i
set OUT_DIR=output
set DST_DIR=dist
set APP_PKG=my-flappy-bird

:: 检查 cmake 是否安装
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo cmake could not be found
    exit /b 1
)

:: 显示 cmake 版本
cmake --version

:: 使用 cmake 生成 Visual Studio 项目
cmake -B %OUT_DIR% -G"Visual Studio 17 2022" --log-level=STATUS

:: 构建项目
cmake --build %OUT_DIR% --config Release

:: 创建输出目录
mkdir "%DST_DIR%" 2>nul

:: 复制构建结果到输出目录
if exist "%OUT_DIR%\Release\%APP_PKG%.exe" (
    copy "%OUT_DIR%\Release\%APP_PKG%.exe" "%DST_DIR%"
) else (
    echo Build failed: %APP_PKG%.exe not found
    exit /b 1
)

:: 清理临时目录
rmdir /s /q "%OUT_DIR%" 2>nul

echo Packaging completed successfully!
