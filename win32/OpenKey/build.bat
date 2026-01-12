@echo off
echo ========================================
echo Building OpenKey Release x64
echo ========================================
echo.

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64

echo.
echo Building...
msbuild OpenKey.sln /p:Configuration=Release /p:Platform=x64 /t:Rebuild /m /v:minimal

if errorlevel 1 (
    echo.
    echo [ERROR] Build Failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build SUCCESS!
echo Copying file to: D:\DU AN UNIKEY\vsCode\OpenKey\BUILD_OUTPUT
echo ========================================
echo.

if not exist "..\..\BUILD_OUTPUT" mkdir "..\..\BUILD_OUTPUT"

copy /Y "x64\Release\OpenKey.exe" "..\..\BUILD_OUTPUT\OpenKey_FixFont.exe"

if exist "..\..\BUILD_OUTPUT\OpenKey_FixFont.exe" (
    echo [OK] File copied successfully!
    echo Location: D:\DU AN UNIKEY\vsCode\OpenKey\BUILD_OUTPUT\OpenKey_FixFont.exe
    
    echo.
    echo ========================================
    echo Cleaning up intermediate build files...
    echo ========================================
    
    REM Clean up Solution output folder
    if exist "x64" (
        echo Deleting x64...
        rmdir /s /q "x64"
    )
    
    REM Clean up Project output folder
    if exist "OpenKey\x64" (
        echo Deleting OpenKey\x64...
        rmdir /s /q "OpenKey\x64"
    )

    echo [OK] Cleanup completed!
) else (
    echo [ERROR] Could not copy file! Cleanup aborted.
)

echo.
echo Done.
pause
