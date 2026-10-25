@echo off
cd /d "%~dp0"
title car log receiver + save  (Ctrl+C to stop)
for /f %%i in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set TS=%%i
echo log file: log_%TS%.txt
echo.
python car_log.py --out "log_%TS%.txt"
echo.
echo [stopped] press any key to close
pause >nul
