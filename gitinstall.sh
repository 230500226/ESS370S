@echo off
setlocal

:: Check if winget is installed
winget --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Winget is not installed. Please install it from https://github.com/microsoft/winget-cli/releases
    exit /b 1
)

:: Install GitHub CLI using winget
echo just a sec
winget install --id GitHub.cli

echo Iiim done.
endlocal
