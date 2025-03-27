@echo off
setlocal

:: Set variables
set REPO_OWNER=230500226
set REPO_NAME=ESS370S
set BRANCH=main
set ARDUINO_EXAMPLES_DIR=%USERPROFILE%\Documents\Arduino\libraries

:: Prompt user for filename
set /p FILENAME=Enter the filename to download from the GitHub repo (e.g., 12LEDS.py): 

:: Construct the GitHub raw URL
set GITHUB_URL=https://raw.githubusercontent.com/%REPO_OWNER%/%REPO_NAME%/%BRANCH%/%FILENAME%

:: Download the file using curl
echo Downloading %FILENAME% from %GITHUB_URL%...
curl -L -o "%ARDUINO_EXAMPLES_DIR%\%FILENAME%" %GITHUB_URL%

if %errorlevel% neq 0 (
    echo Failed to download the file. Please check the filename and try again.
    exit /b 1
)

echo Download complete. The file has been saved to %ARDUINO_EXAMPLES_DIR%\%FILENAME%.
endlocal
