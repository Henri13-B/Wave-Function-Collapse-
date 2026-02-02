@echo off
REM ============================================================
REM Script de lancement pour Wave Function Collapse (Windows)
REM ============================================================

echo.
echo ============================================================
echo    Wave Function Collapse - Lancement
echo ============================================================
echo.

REM Vérifier si l'exécutable existe
if not exist "build\WFC.exe" (
    echo [ERREUR] Executable non trouve : build\WFC.exe
    echo.
    echo Veuillez compiler le projet avec :
    echo   python build.py
    echo.
    pause
    exit /b 1
)

REM Lancer le programme
echo Lancement de WFC...
echo.
cd build
WFC.exe
cd ..

echo.
echo ============================================================
echo    Programme termine
echo ============================================================
echo.
pause